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

#include <avr/interrupt.h>
#include <avr/io.h>
#include "atomic.h"
#include "Hardware.h"
#include "GTPowerA6-10-pins.h"
#include "adc.h"
#include "Utils.h"
#include "memory.h"
#include "IO.h"

#include "Timer0.h"
#include "AnalogInputsPrivate.h"

/* ADC - measurement:
 * uses Timer0 to trigger conversion
 * program flow:
 *     ADC routine                          |  multiplexer routine
 * -----------------------------------------------------------------------------
 * Timer0: start ADC (no MUX) conversion    |  1. switch to MUX desired output
 *                                          |
 *                                          |
 *                                          |
 *                                          |
 * ADC_vect:   switch ADC to MUX            |                             |
 *                                          |
 * Timer0: start ADC (MUX) conversion       |
 *                                          |
 *                                          |
 *                                          |
 *                                          |
 * ADC_vect:   switch ADC to no MUX         |
 *                                          |
 * Timer0: start ADC (MUX) conversion       |  repeat 1.
 * ...
 *
 * note: 1. is in setMuxAddress()
 */


#define ADC_KEY_BORDER 128

namespace adc {

static uint8_t input_;
static uint8_t adc_keyboard_;
static uint8_t g_addSumToInput = 0;

void reset() {
    input_ = 0;
}

void initialize()
{

    IO::pinMode(MUX0_Z_D_PIN, INPUT);
    IO::pinMode(MUX1_Z_D_PIN, INPUT);
    IO::digitalWrite(MUX0_Z_D_PIN, 0);
    IO::digitalWrite(MUX1_Z_D_PIN, 0);

    IO::pinMode(MUX_ADR0_PIN, OUTPUT);
    IO::pinMode(MUX_ADR1_PIN, OUTPUT);
    IO::pinMode(MUX_ADR2_PIN, OUTPUT);

    //ADC Auto Trigger Source - Timer/Counter0 Compare Match
    SFIOR |= _BV(ADTS1) | _BV(ADTS0);

    //ADEN: ADC Enable
    //ADATE: ADC Auto Trigger Enable
    //ADIE: ADC Interrupt Enable
    //ADPS2:0: ADC Prescaler Select Bits = 16MHz/ 128 = 125kHz
    ADCSRA = _BV(ADEN) | _BV(ADATE) | _BV(ADIE) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);

    adc::reset();
    Timer0::initialize();
}


struct adc_correlation {
    int8_t mux_;
    uint8_t adc_;
    AnalogInputs::Name ai_name_;
    uint8_t key_;
};

#define ADC_STANDARD_PER_ROUND 2
#if MAX_BANANCE_CELLS > 6
#define ADC_I_DISCHARGE_PER_ROUND 2
#else
#define ADC_I_DISCHARGE_PER_ROUND 1
#endif

const adc_correlation order_analogInputs_on[] PROGMEM = {
    {-1,                    OUTPUT_VOLTAGE_PLUS_PIN,AnalogInputs::Vout_plus_pin,    0},
    {MADDR_V_OUTMUX,        MUX0_Z_A_PIN ,          AnalogInputs::VoutMux,          0},
    {MADDR_V_BALANSER1,     MUX1_Z_A_PIN,           AnalogInputs::Vb1_pin,          0},
    {-1,                    OUTPUT_VOLTAGE_MINUS_PIN,AnalogInputs::Vout_minus_pin,  0},
    {MADDR_T_INTERN,        MUX0_Z_A_PIN,           AnalogInputs::Tintern,          0},
    {MADDR_V_BALANSER2,     MUX1_Z_A_PIN,           AnalogInputs::Vb2_pin,          0},
    {-1,                    SMPS_CURRENT_PIN,       AnalogInputs::Ismps,            0},
    {MADDR_V_IN,            MUX0_Z_A_PIN,           AnalogInputs::Vin,              0},
    {MADDR_V_BALANSER3,     MUX1_Z_A_PIN,           AnalogInputs::Vb3_pin,          0},
    {-1,                    DISCHARGE_CURRENT_PIN,  AnalogInputs::Idischarge,       0},
    {MADDR_T_EXTERN,        MUX0_Z_A_PIN,           AnalogInputs::Textern,          0},
    {MADDR_V_BALANSER4,     MUX1_Z_A_PIN,           AnalogInputs::Vb4_pin,          0},
    {-1,                    OUTPUT_VOLTAGE_PLUS_PIN,AnalogInputs::Vout_plus_pin,    0},
    {MADDR_BUTTON_DEC,      MUX0_Z_A_PIN,           AnalogInputs::VirtualInputs,    BUTTON_DEC},
    {MADDR_V_BALANSER5,     MUX1_Z_A_PIN,           AnalogInputs::Vb5_pin,          0},
    {-1,                    OUTPUT_VOLTAGE_MINUS_PIN,AnalogInputs::Vout_minus_pin,  0},
    {MADDR_BUTTON_INC,      MUX0_Z_A_PIN,           AnalogInputs::VirtualInputs,    BUTTON_INC},
    {MADDR_V_BALANSER6,     MUX1_Z_A_PIN,           AnalogInputs::Vb6_pin,          0},
#if MAX_BANANCE_CELLS > 6
    {-1,                    SMPS_CURRENT_PIN,       AnalogInputs::Ismps,            0},
    {MADDR_BUTTON_STOP,     MUX0_Z_A_PIN,           AnalogInputs::VirtualInputs,    BUTTON_STOP},
    {MADDR_V_BALANSER7,     MUX1_Z_A_PIN,           AnalogInputs::Vb7_pin,          0},
    {-1,                    DISCHARGE_CURRENT_PIN,  AnalogInputs::Idischarge,       0},
    {MADDR_BUTTON_START,    MUX0_Z_A_PIN,           AnalogInputs::VirtualInputs,    BUTTON_START},
    {MADDR_V_BALANSER8,     MUX1_Z_A_PIN,           AnalogInputs::Vb8_pin,          0},
#else
    {MADDR_BUTTON_STOP,     MUX0_Z_A_PIN,           AnalogInputs::VirtualInputs,    BUTTON_STOP},
    {-1,                    SMPS_CURRENT_PIN,       AnalogInputs::Ismps,            0},
    {MADDR_BUTTON_START,    MUX0_Z_A_PIN,           AnalogInputs::VirtualInputs,    BUTTON_START},
#endif
};

AnalogInputs::Name getAIName(uint8_t input){ return pgm::read(&order_analogInputs_on[input].ai_name_); }
uint8_t getADC(uint8_t input)               { return pgm::read(&order_analogInputs_on[input].adc_); }
uint8_t getKey(uint8_t input)               { return pgm::read(&order_analogInputs_on[input].key_); }
int8_t getMUX(uint8_t input)                { return pgm::read(&order_analogInputs_on[input].mux_);}
inline uint8_t nextInput(uint8_t i) {
    if(++i >= sizeOfArray(order_analogInputs_on)) i=0;
    return i;
}

void setADC(uint8_t pin) {
    // ADLAR - ADC Left Adjust Result
    ADMUX = (EXTERNAL << 6) | _BV(ADLAR) | pin;
}

void setMuxAddress(int8_t address)
{
    static uint8_t last = -1;
    if(address < 0)
        return;
    if(address != last) {
        last = address;
//        IO::digitalWrite(MUX_ADR0_PIN, address&1);
//        IO::digitalWrite(MUX_ADR1_PIN, address&2);
//        IO::digitalWrite(MUX_ADR2_PIN, address&4);
        uint8_t port_adr =  ((address&1) << 2) | (address&2) | ((address&4) >>2);
        PORTB = (PORTB & 0x1f) | (port_adr) << 5;
    }
}

void processConversion(bool finalize)
{
    uint8_t low, high;
    uint8_t key;
    low  = ADCL;
    high = ADCH;

    AnalogInputs::Name name = getAIName(input_);
    if(name != AnalogInputs::VirtualInputs) {
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            uint16_t v = (high << 8) | low;
            AnalogInputs::i_adc_[name] = v;
            if(g_addSumToInput) {
                AnalogInputs::i_avrSum_[name] += v;
            }
        }
    } else {
        key = getKey(input_);
        if(high < ADC_KEY_BORDER) {
            adc_keyboard_ |= key;
        } else {
            adc_keyboard_ &= ~key;
        }
    }
}

void finalizeMeasurement()
{
    AnalogInputs::i_adc_[AnalogInputs::IsmpsSet]        = SMPS::getValue();
    AnalogInputs::i_adc_[AnalogInputs::IdischargeSet]   = Discharger::getValue();
    if(g_addSumToInput) {
        AnalogInputs::i_avrSum_[AnalogInputs::IsmpsSet]        += SMPS::getValue();
        AnalogInputs::i_avrSum_[AnalogInputs::IdischargeSet]   += Discharger::getValue();
        if(AnalogInputs::i_avrCount_ == 1) {
            AnalogInputs::i_avrSum_[AnalogInputs::Ismps]            /= ADC_STANDARD_PER_ROUND;
            AnalogInputs::i_avrSum_[AnalogInputs::Vout_plus_pin]    /= ADC_STANDARD_PER_ROUND;
            AnalogInputs::i_avrSum_[AnalogInputs::Vout_minus_pin]   /= ADC_STANDARD_PER_ROUND;
            AnalogInputs::i_avrSum_[AnalogInputs::Idischarge]       /= ADC_I_DISCHARGE_PER_ROUND;
        }
        //TODO: maybe intterruptFinalizeMeasurement should be removed
        AnalogInputs::intterruptFinalizeMeasurement();
    }

}

void setNextMuxAddress()
{
    uint8_t input = nextInput(input_);
    int8_t mux = getMUX(input);
    setMuxAddress(mux);
}

void timerInterrupt()
{
    setNextMuxAddress();
}

void conversionDone()
{
    processConversion(input_);
    input_ = nextInput(input_);
    setADC(getADC(input_));

    if(input_ == 0) {
        finalizeMeasurement();
        g_addSumToInput = AnalogInputs::i_avrCount_ > 0;
    }

}

}// namespace adc

uint8_t hardware::getKeyPressed()
{
    return adc::adc_keyboard_;
}

ISR(TIMER0_COMP_vect)
{
    adc::timerInterrupt();
}

ISR(ADC_vect)
{
    adc::conversionDone();
}
