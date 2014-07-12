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
#include "imaxB6-pins.h"
#include "SMPS_PID.h"
#include "adc.h"
#include "Utils.h"
#include "memory.h"
#include "Settings.h"
#include "AnalogInputsPrivate.h"
#include "IO.h"
#include "SMPS.h"
#include "Discharger.h"

#include "adc.h"

/* ADC - measurement:
 * uses Timer0 to trigger conversion
 * program flow:
 *     ADC routine                          |  multiplexer routine
 * -----------------------------------------------------------------------------
 * Timer0: start ADC (no MUX) conversion    |  1. switch MUX to Op-amp
 *                                          |  2. start discharging C_adc (MUX capacitor)
 *                                          |  3. wait 10us
 *                                          |  4. switch to MUX desired output, stop disch. C_adc
 *                                          |  (wait - for C_adc to charge)
 * ADC_vect:   switch ADC to MUX            |                             |
 *                                          |
 * Timer0: start ADC (MUX) conversion       |
 *                                          |
 *                                          |
 *                                          |
 *                                          |
 * ADC_vect:   switch ADC to no MUX         |
 *                                          |
 * Timer0: start ADC (MUX) conversion       |  repeat 1..4
 * ...
 *
 * note: 1-4 are in setMuxAddress()
 */


//discharge ADC capacitor on Vb6 - there is an operational amplifier
#define ADC_CAPACITOR_DISCHARGE_ADDRESS MADDR_V_BALANSER6
#define ADC_CAPACITOR_DISCHARGE_DELAY_US 10


volatile uint8_t g_adcBurstCount = 0;
volatile uint8_t g_adcInputName = 0;
volatile uint8_t g_muxAddress = 0;
volatile uint8_t g_addSumToInput = 0;
volatile uint32_t g_adcSum = 0;
volatile uint32_t g_adcValue = 0;



namespace adc {

static uint8_t current_input_;

void startConversion();



struct adc_correlation {
    int8_t mux_;
    uint8_t adc_;
    AnalogInputs::Name ai_name_;
    bool trigger_PID_;
    bool slow_input_;
};

const adc_correlation order_analogInputs_on[] PROGMEM = {
	{MADDR_V_BALANSER_BATT_MINUS,   MUX0_Z_D_PIN,           AnalogInputs::Vb0_pin,         false, true},
	{-1,                            OUTPUT_VOLTAGE_MINUS_PIN,AnalogInputs::Vout_minus_pin, false, false},
	{MADDR_V_BALANSER1,             MUX0_Z_D_PIN,           AnalogInputs::Vb1_pin,         false, true},
	{-1,                            SMPS_CURRENT_PIN,       AnalogInputs::Ismps,           true,  false},
	{MADDR_V_BALANSER2,             MUX0_Z_D_PIN,           AnalogInputs::Vb2_pin,         false, true},
	{-1,                            OUTPUT_VOLTAGE_PLUS_PIN,AnalogInputs::Vout_plus_pin,   false, false},
	{MADDR_V_BALANSER6,             MUX0_Z_D_PIN,           AnalogInputs::Vb6_pin,         false, false},
	{-1,                            SMPS_CURRENT_PIN,       AnalogInputs::Ismps,           true,  false},
	{MADDR_V_BALANSER5,             MUX0_Z_D_PIN,           AnalogInputs::Vb5_pin,         false, false},
	{-1,                            DISCHARGE_CURRENT_PIN,  AnalogInputs::Idischarge,      false, false},
	{MADDR_V_BALANSER4,             MUX0_Z_D_PIN,           AnalogInputs::Vb4_pin,         false, false},
	{-1,                            SMPS_CURRENT_PIN,       AnalogInputs::Ismps,           true,  false},
	{MADDR_V_BALANSER3,             MUX0_Z_D_PIN,           AnalogInputs::Vb3_pin,         false, false},
	{-1,                            V_IN_PIN,               AnalogInputs::Vin,             false, false},
	{MADDR_T_EXTERN,                MUX0_Z_D_PIN,           AnalogInputs::Textern,         false, false},
	{-1,                            SMPS_CURRENT_PIN,       AnalogInputs::Ismps,           true,  false},

};


inline uint8_t nextInput(uint8_t i) {
    if(++i >= sizeOfArray(order_analogInputs_on)) i=0;
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
    	_setMuxAddress(MADDR_V_BALANSER6);
    	IO::disableFuncADC(IO::getPinBit(MUX0_Z_D_PIN));

//    	_setMuxAddress(address);
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
    	IO::enableFuncADC(IO::getPinBit(MUX0_Z_D_PIN));
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
    IO::pinMode(UART_TX_PIN, ANALOG_INPUT);


    IO::pinMode(MUX0_Z_D_PIN, ANALOG_INPUT_DISCHARGE);
    IO::digitalWrite(MUX0_Z_D_PIN, 0);

    //initialize TIMER 3 (mux ADC capacitor discharge)
	CLK_EnableModuleClock(TMR1_MODULE);
	CLK_SetModuleClock(TMR1_MODULE,CLK_CLKSEL1_TMR1_S_HCLK,CLK_CLKDIV_UART(1));
	//TODO: 50kHz ??
    TIMER_Open(TIMER1, TIMER_ONESHOT_MODE, 50000);//2000);
    TIMER_EnableInt(TIMER1);
    NVIC_EnableIRQ(TMR1_IRQn);

    //initialize ADC
    //init clock
    CLK_EnableModuleClock(ADC_MODULE);
    CLK_SetModuleClock(ADC_MODULE, CLK_CLKSEL1_ADC_S_HXT, CLK_CLKDIV_ADC(4));

    /* Set the ADC operation mode as burst, input mode as single-end and enable the analog input channel 2 */
    ADC_Open(ADC, ADC_ADCR_DIFFEN_SINGLE_END, ADC_ADCR_ADMD_BURST, 0x1 << 2);

    /* Power on ADC module */
    ADC_POWER_ON(ADC);

    /* clear the A/D interrupt flag for safe */
    ADC_CLR_INT_FLAG(ADC, ADC_ADF_INT);

    /* Enable the ADC interrupt */
    ADC_EnableInt(ADC, ADC_ADF_INT);
    NVIC_EnableIRQ(ADC_IRQn);
    NVIC_SetPriority(ADC_IRQn, 0);// (1<<__NVIC_PRIO_BITS) - 1);

    current_input_ = 0;
    startConversion();
}

void setNextMuxAddress()
{
    uint8_t next_input = nextInput(current_input_);
    int8_t mux = order_analogInputs_on[next_input].mux_;
    //TODO: disable temperature
    if(settings.UART_ != Settings::Disabled && mux == MADDR_T_EXTERN)
        mux = MADDR_V_BALANSER6;

    setMuxAddressAndDischarge(mux);
}


void startConversion()
{
    setNextMuxAddress();

    g_adcInputName = order_analogInputs_on[current_input_].ai_name_;
    g_adcBurstCount = 0;
    g_adcSum = 0;
    setADC(order_analogInputs_on[current_input_].adc_);
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
    AnalogInputs::i_adc_[AnalogInputs::IsmpsValue]        = SMPS::getValue();
    AnalogInputs::i_adc_[AnalogInputs::IdischargeValue]   = Discharger::getValue();

    if(g_addSumToInput) {
        AnalogInputs::i_avrSum_[AnalogInputs::IsmpsValue]        += SMPS::getValue() * ANALOG_INPUTS_ADC_BURST_COUNT;
        AnalogInputs::i_avrSum_[AnalogInputs::IdischargeValue]   += Discharger::getValue() * ANALOG_INPUTS_ADC_BURST_COUNT;
        AnalogInputs::intterruptFinalizeMeasurement();
    }

}


} // namespace adc


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
                adc::conversionDone();
                break;
            }
        }

        ADC_CLR_INT_FLAG(ADC0, ADC_ADF_INT);
    }
} //extern "C"
