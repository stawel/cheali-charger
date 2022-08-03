/*
    cheali-charger - open source firmware for a variety of LiPo chargers
    Copyright (C) 2013  Paweł Stawicki. All right reserved.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "atomic.h"
#include "Hardware.h"
#include "SMPS_PID.h"
#include "adc.h"
#include "Utils.h"
#include "memory.h"
#include "Settings.h"
#include "AnalogInputsPrivate.h"
#include "IO.h"
#include "SMPS.h"
#include "Discharger.h"
#include "irq_priority.h"

#include "adc.h"

/* ADC - measurement:
 * uses Timer1 to discharge Multiplexer Capacitor
 * program flow:
 *     ADC routine                          |  multiplexer routine
 * -----------------------------------------------------------------------------
 * start ADC (no MUX) conversion            |  1. switch MUX to op-amp (cell 6)
 *                                          |  2. start discharging C_adc (MUX capacitor), start Timer1
 *                                          |  3. wait 20us (timer1 50kHz)
 *                                          |  4. switch to MUX desired output, stop disch. C_adc
 *                                          |  (wait - for C_adc to charge)
 *                                          |
 *                                          |
 * start ADC (MUX) conversion               |
 *                                          |
 *                                          |
 *                                          |
 *                                          |
 *                                          |
 *                                          |
 * start ADC (no MUX) conversion            |  repeat 1..4
 * ...
 *
 * note: 1-4 are in setMuxAddress()
 * note: for each ADC pin (start ADC) we do 70 measurements,
 *       all in all we do 70*100=7000 measurements for a "fullMeasurement" per input.
 */


#define ADC_I_SMPS_PER_ROUND 4
//discharge ADC capacitor on Vb6 - there is an operational amplifier
#define ADC_CAPACITOR_DISCHARGE_ADDRESS MADDR_V_BALANSER6
#define ADC_CAPACITOR_DISCHARGE_DELAY_US 20
#define ADC_CLOCK_FREQUENCY 4000000UL



volatile uint8_t g_adcBurstCount = 0;
volatile uint8_t g_adcInputName = 0;
volatile uint8_t g_muxAddress = 0;
volatile uint8_t g_addSumToInput = 0;
volatile uint32_t g_adcSum = 0;
volatile uint32_t g_adcValue = 0;



namespace AnalogInputsADC {

static uint8_t current_input_;

void startConversion();



struct adc_correlation {
    int8_t mux_;
    uint8_t adc_pin_;
    AnalogInputs::Name ai_name_;
    bool trigger_PID_;
};

const adc_correlation order_analogInputs_on[] PROGMEM = {
    {MADDR_V_BALANSER_BATT_MINUS,   MUX0_Z_D_PIN,           AnalogInputs::Vb0_pin,         false},
    {-1,                            OUTPUT_VOLTAGE_MINUS_PIN,AnalogInputs::Vout_minus_pin, false},
    {MADDR_V_BALANSER1,             MUX0_Z_D_PIN,           AnalogInputs::Vb1_pin,         false},
    {-1,                            SMPS_CURRENT_PIN,       AnalogInputs::Ismps,           true},
    {MADDR_V_BALANSER2,             MUX0_Z_D_PIN,           AnalogInputs::Vb2_pin,         false},
    {-1,                            OUTPUT_VOLTAGE_PLUS_PIN,AnalogInputs::Vout_plus_pin,   false},
    {MADDR_V_BALANSER6,             MUX0_Z_D_PIN,           AnalogInputs::Vb6_pin,         false},
    {-1,                            SMPS_CURRENT_PIN,       AnalogInputs::Ismps,           true},
    {MADDR_V_BALANSER5,             MUX0_Z_D_PIN,           AnalogInputs::Vb5_pin,         false},
    {-1,                            DISCHARGE_CURRENT_PIN,  AnalogInputs::Idischarge,      false},
    {MADDR_V_BALANSER4,             MUX0_Z_D_PIN,           AnalogInputs::Vb4_pin,         false},
    {-1,                            SMPS_CURRENT_PIN,       AnalogInputs::Ismps,           true},
    {MADDR_V_BALANSER3,             MUX0_Z_D_PIN,           AnalogInputs::Vb3_pin,         false},
    {-1,                            V_IN_PIN,               AnalogInputs::Vin,             false},
    {-1,                            T_EXTERNAL_PIN,         AnalogInputs::Textern,         false},
    {-1,                            T_INTERNAL_PIN,         AnalogInputs::Tintern,         false},
    {-1,                            SMPS_CURRENT_PIN,       AnalogInputs::Ismps,           true},
};



inline uint8_t nextInput(uint8_t i) {
    i++;
    if(i >= sizeOfArray(order_analogInputs_on)) i=0;
    return i;
}


void setADC(uint8_t pin) {
    ADC_SET_INPUT_CHANNEL(ADC, 1 << IO::getADCChannel(pin));
}

void _setMuxAddress(int8_t address)
{
    IO::digitalWrite(MUX_ADR0_PIN, address&1);
    IO::digitalWrite(MUX_ADR1_PIN, address&2);
    IO::digitalWrite(MUX_ADR2_PIN, address&4);
}

void setMuxAddressAndDischarge(int8_t address)
{
    if(address < 0)
        return;
    g_muxAddress = address;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        //start mux ADC C discharge
        _setMuxAddress(ADC_CAPACITOR_DISCHARGE_ADDRESS);
        IO::disableFuncADC(IO::getADCChannel(MUX0_Z_D_PIN));
    }
    TIMER_Start(TIMER1);             /* Start counting */
}
extern "C" {
void TMR1_IRQHandler(void)
{
    /* Clear Timer1 time-out interrupt flag */
    TIMER_ClearIntFlag(TIMER1);

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        _setMuxAddress(g_muxAddress);
        //stop mux ADC C discharge
        IO::enableFuncADC(IO::getADCChannel(MUX0_Z_D_PIN));
    }
}
} //extern "C"


void initialize()
{
    IO::pinMode(MUX_ADR0_PIN, OUTPUT);
    IO::pinMode(MUX_ADR1_PIN, OUTPUT);
    IO::pinMode(MUX_ADR2_PIN, OUTPUT);

    IO::pinMode(V_IN_PIN, ANALOG_INPUT);
    IO::pinMode(OUTPUT_VOLTAGE_MINUS_PIN, ANALOG_INPUT);
    IO::pinMode(SMPS_CURRENT_PIN, ANALOG_INPUT);
    IO::pinMode(OUTPUT_VOLTAGE_PLUS_PIN, ANALOG_INPUT);
    IO::pinMode(DISCHARGE_CURRENT_PIN, ANALOG_INPUT);

    IO::pinMode(MUX0_Z_D_PIN, ANALOG_INPUT_DISCHARGE);
    IO::digitalWrite(MUX0_Z_D_PIN, 0);

    //initialize internal temperature sensor
    SYS->TEMPCR |= 1;

    //initialize TIMER 1 (mux ADC capacitor discharge)
    CLK_EnableModuleClock(TMR1_MODULE);
    CLK_SetModuleClock(TMR1_MODULE,CLK_CLKSEL1_TMR1_S_HCLK,CLK_CLKDIV_UART(1));
    //TODO: 50kHz ??
    TIMER_Open(TIMER1, TIMER_ONESHOT_MODE, 1000000 / ADC_CAPACITOR_DISCHARGE_DELAY_US);
    TIMER_EnableInt(TIMER1);
    NVIC_EnableIRQ(TMR1_IRQn);
    NVIC_SetPriority(TMR1_IRQn, ADC_C_DISCHARGE_IRQ_PRIORITY);

    //initialize ADC
    //init clock
    CLK_EnableModuleClock(ADC_MODULE);
    CLK_SetModuleClock(ADC_MODULE, CLK_CLKSEL1_ADC_S_HCLK, CLK_CLKDIV_ADC(CLK_GetHCLKFreq()/ADC_CLOCK_FREQUENCY));
            //__HXT/ADC_CLOCK_FREQUENCY));

    /* Set the ADC operation mode as burst, input mode as single-end and enable the analog input channel 2 */
    ADC_Open(ADC, ADC_ADCR_DIFFEN_SINGLE_END, ADC_ADCR_ADMD_BURST, 0x1 << 2);
    ADC_SET_DMOF(ADC, ADC_ADCR_DMOF_UNSIGNED_OUTPUT);

    /* Power on ADC module */
    ADC_POWER_ON(ADC);

    /* clear the A/D interrupt flag for safe */
    ADC_CLR_INT_FLAG(ADC, ADC_ADF_INT);

    /* Enable the ADC interrupt */
    ADC_EnableInt(ADC, ADC_ADF_INT);
    NVIC_EnableIRQ(ADC_IRQn);
    NVIC_SetPriority(ADC_IRQn, ADC_IRQ_PRIORITY);

    current_input_ = 0;
    startConversion();
}

void setNextMuxAddress()
{
    uint8_t next_input = nextInput(current_input_);
    int8_t mux = order_analogInputs_on[next_input].mux_;

    setMuxAddressAndDischarge(mux);
}


void startConversion()
{
    setNextMuxAddress();

    g_adcInputName = order_analogInputs_on[current_input_].ai_name_;
    g_adcBurstCount = 0;
    g_adcSum = 0;
    uint8_t adc_pin = order_analogInputs_on[current_input_].adc_pin_;
    setADC(adc_pin);
    if(adc_pin > 64) {
        ADC_CONFIG_CH7(ADC, (adc_pin >> 6) << ADC_ADCHER_PRESEL_Pos);
    } else {
        ADC_CONFIG_CH7(ADC, ADC_ADCHER_PRESEL_EXT_INPUT_SIGNAL);
    }
    ADC_START_CONV(ADC);
}

void finalizeMeasurement();

#define ADC_GET_CONVERSION_DATA2(adc, u32ChNum) ((inpw(&(ADC->ADDR[(u32ChNum)])) & ADC_ADDR_RSLT_Msk)>>ADC_ADDR_RSLT_Pos)
#define ADC_IS_BUSY2(adc) (inpw(&ADC->ADSR) & ADC_ADSR_BUSY_Msk ? 1 : 0)
#define ADC_IS_DATA_VALID2(adc, u32ChNum) (inpw(&ADC->ADSR) & (0x1<<(ADC_ADSR_VALID_Pos+u32ChNum)) ? 1 : 0)


void conversionDone()
{
    while(ADC_IS_BUSY2(ADC));
    while(ADC_IS_DATA_VALID2(ADC, 0)) ADC_GET_CONVERSION_DATA2(ADC, 0);

    current_input_ = nextInput(current_input_);

    if(current_input_ == 0) {
        finalizeMeasurement();
        g_addSumToInput = AnalogInputs::i_avrCount_ > 0;
    }
    startConversion();

    if(order_analogInputs_on[current_input_].trigger_PID_)
        SMPS_PID::update();


}

void finalizeMeasurement()
{
    AnalogInputs::i_adc_[AnalogInputs::IsmpsSet]        = SMPS::getValue();
    AnalogInputs::i_adc_[AnalogInputs::IdischargeSet]   = Discharger::getValue();

    if(g_addSumToInput) {
        AnalogInputs::i_avrSum_[AnalogInputs::IsmpsSet]        += SMPS::getValue() * ANALOG_INPUTS_ADC_BURST_COUNT;
        AnalogInputs::i_avrSum_[AnalogInputs::IdischargeSet]   += Discharger::getValue() * ANALOG_INPUTS_ADC_BURST_COUNT;
        if(AnalogInputs::i_avrCount_ == 1) {
            AnalogInputs::i_avrSum_[AnalogInputs::Ismps]          /= ADC_I_SMPS_PER_ROUND;
        }
        //TODO: maybe intterruptFinalizeMeasurement should be removed
        AnalogInputs::intterruptFinalizeMeasurement();
    }

}


} // namespace AnalogInputsADC

namespace adc {
void debug() {}
}

extern "C" {

    void ADC_IRQHandler(void)
    {
        while(ADC_IS_DATA_VALID2(ADC, 0)) /* Check the VALID bits */
        {
            /* In burst mode, the software always gets the conversion result of the specified channel from channel 0 */
            g_adcValue = ADC_GET_CONVERSION_DATA2(ADC, 0);
            if(g_adcBurstCount > 1) {
                g_adcSum += g_adcValue;
            }
            if(++g_adcBurstCount > ANALOG_INPUTS_ADC_BURST_COUNT+1) {
                ADC_STOP_CONV(ADC);
                // pretend 16bit adc
                AnalogInputs::i_adc_[g_adcInputName] = g_adcValue << 4;
                if(g_addSumToInput)
                    AnalogInputs::i_avrSum_[g_adcInputName] += g_adcSum << 4;
                AnalogInputsADC::conversionDone();
                break;
            }
        }

        ADC_CLR_INT_FLAG(ADC0, ADC_ADF_INT);
    }
} //extern "C"
