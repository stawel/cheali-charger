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
#include <util/atomic.h>
#include <Arduino.h>
#include "Hardware.h"
#include "GTPowerA6-10-pins.h"
#include "adc.h"
#include "Utils.h"
#include "memory.h"



#define ADC_DELAY_MS 30
#define ADC_KEY_BORDER 128

struct adc_correlation {
    int8_t mux_;
    uint8_t adc_;
    AnalogInputs::Name ai_name_;
    uint8_t key_;
};

const adc_correlation order_analogInputs_on[] PROGMEM = {
    {-1,                    OUTPUT_VOLTAGE_PIN,     AnalogInputs::Vout,             0},
    {MADDR_V_OUTMUX,        MUX0_Z_A_PIN ,          AnalogInputs::VoutMux,          0},
    {MADDR_V_BALANSER1,     MUX1_Z_A_PIN,           AnalogInputs::Vb1_pin,         0},
    {-1,                    REVERSE_POLARITY_PIN,   AnalogInputs::VreversePolarity, 0},
    {MADDR_T_INTERN,        MUX0_Z_A_PIN,           AnalogInputs::Tintern,          0},
    {MADDR_V_BALANSER2,     MUX1_Z_A_PIN,           AnalogInputs::Vb2_pin,         0},
    {-1,                    SMPS_CURRENT_PIN,       AnalogInputs::Ismps,            0},
    {MADDR_V_IN,            MUX0_Z_A_PIN,           AnalogInputs::Vin,              0},
    {MADDR_V_BALANSER3,     MUX1_Z_A_PIN,           AnalogInputs::Vb3_pin,         0},
    {-1,                    OUTPUT_VOLTAGE_PIN,     AnalogInputs::Vout,             0},
    {MADDR_T_EXTERN,        MUX0_Z_A_PIN,           AnalogInputs::Textern,          0},
    {MADDR_V_BALANSER4,     MUX1_Z_A_PIN,           AnalogInputs::Vb4_pin,         0},
    {-1,                    DISCHARGE_CURRENT_PIN,  AnalogInputs::Idischarge,       0},
    {MADDR_BUTTON_DEC,      MUX0_Z_A_PIN,           AnalogInputs::VirtualInputs,    BUTTON_DEC},
    {MADDR_V_BALANSER5,     MUX1_Z_A_PIN,           AnalogInputs::Vb5_pin,         0},
    {-1,                    SMPS_CURRENT_PIN,       AnalogInputs::Ismps,            0},
    {MADDR_BUTTON_INC,      MUX0_Z_A_PIN,           AnalogInputs::VirtualInputs,    BUTTON_INC},
    {MADDR_V_BALANSER6,     MUX1_Z_A_PIN,           AnalogInputs::Vb6_pin,         0},
#ifdef ANALOG_INPUTS_V_UNKNOWN
    {MADDR_V_UNKNOWN0,      MUX1_Z_A_PIN,           AnalogInputs::Vunknown0,        0},
    {MADDR_V_UNKNOWN1,      MUX1_Z_A_PIN,           AnalogInputs::Vunknown1,        0},
#endif
    {MADDR_BUTTON_STOP,     MUX0_Z_A_PIN,           AnalogInputs::VirtualInputs,    BUTTON_STOP},
    {MADDR_BUTTON_START,    MUX0_Z_A_PIN,           AnalogInputs::VirtualInputs,    BUTTON_START},
};


#define DEFAULT 1
#define EXTERNAL 0

namespace {

static const uint8_t analog_reference = EXTERNAL;

static uint8_t current_input;
static uint8_t adc_keyboard;

void setADC(uint8_t pin) {
    // ADLAR - ADC Left Adjust Result
    ADMUX = (analog_reference << 6) | _BV(ADLAR) | (pin & 0x07);
}

void setMuxAddress(uint8_t address)
{
    static uint8_t last = -1;
    if(address !=last) {
        last = address;
//        digitalWrite(MUX_ADR0_PIN, address&1);
//        digitalWrite(MUX_ADR1_PIN, address&2);
//        digitalWrite(MUX_ADR2_PIN, address&4);
        uint8_t port_adr =  ((address&1) << 2) | (address&2) | ((address&4) >>2);
        PORTB = (PORTB & 0x1f) | (port_adr) << 5;
    }
}

inline uint8_t nextInput(uint8_t x)
{
    if(x == sizeOfArray(order_analogInputs_on) -1) return 0;
    return x+1;
}

/* if You want use interrupts then
 * most of the AnalogInputs methods should be rewritten
 * to be atomic. (see <util/atomic.h>)
 * We use a ADC_DELAY_MS delay between setting the multiplexer address
 * and the ADC measurement which is also a problem.
 */
ISR(ADC_vect)
{
//    adc::processMeasurement();
//    adc::startConversion();
}


AnalogInputs::Name getAIName(uint8_t input)
{
    return pgm::read(&order_analogInputs_on[input].ai_name_);
}

uint8_t getADC(uint8_t input)
{
    return pgm::read(&order_analogInputs_on[input].adc_);
}

uint8_t getKey(uint8_t input)
{
    return pgm::read(&order_analogInputs_on[input].key_);
}

int8_t getMUX(uint8_t input)
{
    int8_t mux;
    do {
        mux = pgm::read(&order_analogInputs_on[input].mux_);
        input = nextInput(input);
    } while(mux<0);
    return mux;
}

} // namespace ""

uint8_t hardware::getKeyPressed()
{
    return adc_keyboard;
}

void adc::processConversion(bool finalize)
{
    uint8_t low, high;
    uint8_t key;
    low  = ADCL;
    high = ADCH;

    AnalogInputs::Name name = getAIName(current_input);
    if(name != AnalogInputs::VirtualInputs) {
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            AnalogInputs::adc_[name] = (high << 8) | low;
        }
    } else {
        key = getKey(current_input);
        if(high < ADC_KEY_BORDER) {
            adc_keyboard |= key;
        } else {
            adc_keyboard &= ~key;
        }
    }
    current_input = nextInput(current_input);
    setMuxAddress(getMUX(current_input));
    setADC(getADC(current_input));

    if(finalize && current_input == 0)
        finalizeMeasurement();
    else {
        //info:improves the precision
        delayMicroseconds(ADC_DELAY_MS);
    }
}

void adc::reset()
{
    current_input = 0;
    setADC(getADC(current_input));
    setMuxAddress(getMUX(current_input));
}

void adc::startConversion() {
    // start the conversion
    ADCSRA |= _BV(ADSC);
}


void adc::finalizeMeasurement()
{
    AnalogInputs::adc_[AnalogInputs::IsmpsValue]        = SMPS::getValue();
    AnalogInputs::adc_[AnalogInputs::IdischargeValue]   = Discharger::getValue();
    AnalogInputs::finalizeMeasurement();
}

void adc::initialize()
{

    pinMode(MUX_ADR0_PIN, OUTPUT);
    pinMode(MUX_ADR1_PIN, OUTPUT);
    pinMode(MUX_ADR2_PIN, OUTPUT);

    //ADEN: ADC Enable
    //ADIE: ADC Interrupt Enable
    //ADPS2:0: ADC Prescaler Select Bits = 16MHz/ 128 = 125kHz
    ADCSRA = _BV(ADEN) | _BV(ADIE) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);

    adc::reset();
}

void adc::doMeasurement()
{
    startConversion();
    //TODO: go into Idle mode
    while (bit_is_set(ADCSRA, ADSC));
    processConversion(AnalogInputs::isPowerOn());
}


//this method depends on the ADC implementation
void AnalogInputs::resetADC()
{
    adc::reset();
}

//this method depends on the ADC implementation
void AnalogInputs::doFullMeasurement()
{
    powerOn();
    clearAvr();
    uint16_t c = calculationCount_;
    while(c == calculationCount_)
        adc::doMeasurement();
}

//this method depends on the ADC implementation
void hardware::delay(uint16_t t)
{
    uint32_t end = Timer::getMiliseconds() + t;
    do {
        adc::doMeasurement();
    } while(Timer::getMiliseconds() < end);
}

