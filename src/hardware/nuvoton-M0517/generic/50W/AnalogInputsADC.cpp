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
#include "irq_priority.h"

#include "adc.h"

#define ADC_CLOCK_FREQUENCY 4000000UL


namespace AnalogInputsADC {

//AnalogInputs::VirtualInputs == not used
const AnalogInputs::Name order_analogInputs_on[8]  = {
//MUX0_Z_D_PIN                    43 //(7+3*12)   P1.0 - T2, AIN0, nWRL
        AnalogInputs::Vb0_pin,
//DISCHARGE_DISABLE_PIN           44 //(8+3*12)   P1.1 - T3, AIN1, nWRH
        AnalogInputs::VirtualInputs,
//UART_TX_PIN,T_EXTERNAL_PIN      45 //(9+3*12)   P1.2 - RXD1[2], AIN2
        AnalogInputs::Textern,
//V_IN_PIN                        46 //(10+3*12)  P1.3 - TXD1[2], AIN3
        AnalogInputs::Vin,
//SMPS_CURRENT_PIN                47 //(11+3*12)  P1.4 - SPISS0, AIN4, ACMP0_N
        AnalogInputs::Ismps,
//OUTPUT_VOLTAGE_MINUS_PIN        1  //(1)        P1.5 - MOSI_0, AIN5, ACMP0_P
        AnalogInputs::Vout_minus_pin,
//OUTPUT_VOLTAGE_PLUS_PIN         2  //(2)        P1.6 - MISO_0, AIN6, ACMP2_N
        AnalogInputs::Vout_plus_pin,
//DISCHARGE_CURRENT_PIN           3  //(3)        P1.7 - SPICLK0, AIN7, ACMP2_P
        AnalogInputs::Idischarge,

/*
//    {MADDR_V_BALANSER_BATT_MINUS,   MUX0_Z_D_PIN,           AnalogInputs::Vb0_pin,         0},
    {-1,                            OUTPUT_VOLTAGE_MINUS_PIN,AnalogInputs::Vout_minus_pin, 0},
//    {MADDR_V_BALANSER1,             MUX0_Z_D_PIN,           AnalogInputs::Vb1_pin,         0},
//    {-1,                            SMPS_CURRENT_PIN,       AnalogInputs::Ismps,           SMPS_PID_UPDATE_TYPE_CURRENT},
//    {MADDR_V_BALANSER2,             MUX0_Z_D_PIN,           AnalogInputs::Vb2_pin,         0},
    {-1,                            OUTPUT_VOLTAGE_PLUS_PIN,AnalogInputs::Vout_plus_pin,   0},
//    {MADDR_V_BALANSER6,             MUX0_Z_D_PIN,           AnalogInputs::Vb6_pin,         0},
    {-1,                            SMPS_CURRENT_PIN,       AnalogInputs::Ismps,           SMPS_PID_UPDATE_TYPE_VOLTAGE | SMPS_PID_UPDATE_TYPE_CURRENT},
//    {MADDR_V_BALANSER5,             MUX0_Z_D_PIN,           AnalogInputs::Vb5_pin,         0},
//    {-1,                            DISCHARGE_CURRENT_PIN,  AnalogInputs::Idischarge,      0},
//    {MADDR_V_BALANSER4,             MUX0_Z_D_PIN,           AnalogInputs::Vb4_pin,         0},
//    {-1,                            SMPS_CURRENT_PIN,       AnalogInputs::Ismps,           SMPS_PID_UPDATE_TYPE_CURRENT},
//    {MADDR_V_BALANSER3,             MUX0_Z_D_PIN,           AnalogInputs::Vb3_pin,         0},
    {-1,                            V_IN_PIN,               AnalogInputs::Vin,             0},
//    {-1,                            T_EXTERNAL_PIN,         AnalogInputs::Textern,         0},
//    {-1,                            T_INTERNAL_PIN,         AnalogInputs::Tintern,         0},
//    {-1,                            SMPS_CURRENT_PIN,       AnalogInputs::Ismps,           SMPS_PID_UPDATE_TYPE_CURRENT},
*/

};

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

    IO::pinMode(MUX0_Z_D_PIN, DISCHARGE_CURRENT_PIN);
    IO::digitalWrite(MUX0_Z_D_PIN, 0);


    //initialize ADC
    //init clock
    CLK_EnableModuleClock(ADC_MODULE);
    CLK_SetModuleClock(ADC_MODULE, CLK_CLKSEL1_ADC_S_HCLK, CLK_CLKDIV_ADC(CLK_GetHCLKFreq()/ADC_CLOCK_FREQUENCY));
            //__HXT/ADC_CLOCK_FREQUENCY));

    ADC_Open(ADC, ADC_ADCR_DIFFEN_SINGLE_END, ADC_ADCR_ADMD_CONTINUOUS, 0xff);
    ADC_SET_DMOF(ADC, ADC_ADCR_DMOF_UNSIGNED_OUTPUT);

    /* Power on ADC module */
    ADC_POWER_ON(ADC);

    /* clear the A/D interrupt flag for safe */
    ADC_CLR_INT_FLAG(ADC, ADC_ADF_INT);

    /* Enable the ADC interrupt */
    ADC_EnableInt(ADC, ADC_ADF_INT);
    NVIC_EnableIRQ(ADC_IRQn);
    NVIC_SetPriority(ADC_IRQn, ADC_IRQ_PRIORITY);

    ADC_START_CONV(ADC);
}

#define ADC_GET_CONVERSION_DATA2(adc, u32ChNum) ((inpw(&(ADC->ADDR[(u32ChNum)])) & ADC_ADDR_RSLT_Msk)>>ADC_ADDR_RSLT_Pos)

void conversionDone()
{
    AnalogInputs::i_adc_[AnalogInputs::IsmpsSet]        = SMPS::getIoutPWM();
    AnalogInputs::i_adc_[AnalogInputs::IdischargeSet]   = Discharger::getValue();

    if(AnalogInputs::i_avrCount_ > 0) {
        AnalogInputs::i_avrSum_[AnalogInputs::IsmpsSet]        += SMPS::getIoutPWM();
        AnalogInputs::i_avrSum_[AnalogInputs::IdischargeSet]   += Discharger::getValue();
        AnalogInputs::intterruptFinalizeMeasurement();
    }

    SMPS_PID::update(SMPS_PID_UPDATE_TYPE_VOLTAGE | SMPS_PID_UPDATE_TYPE_CURRENT);
}


} // namespace AnalogInputsADC


extern "C" {

    void ADC_IRQHandler(void)
    {
        for(int i = 0; i < 8; i++) {
            uint16_t v = ADC_GET_CONVERSION_DATA2(ADC, i);
            AnalogInputs::i_adc_[AnalogInputsADC::order_analogInputs_on[i]] = v << 4;
        }

        if(AnalogInputs::i_avrCount_ > 0) {
            for(int i = 0; i < 8; i++) {
                AnalogInputs::Name name = AnalogInputsADC::order_analogInputs_on[i];
                AnalogInputs::i_avrSum_[name] += AnalogInputs::i_adc_[name];
            }
        }
        AnalogInputsADC::conversionDone();
        ADC_CLR_INT_FLAG(ADC0, ADC_ADF_INT);
    }
} //extern "C"
