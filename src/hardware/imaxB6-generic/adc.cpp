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
#include "imaxB6-pins.h"
#include "SMPS_PID.h"
#include "adc.h"
#include "Utils.h"
#include "memory.h"
#include "Settings.h"


//TODO: the imaxb6 "adc" and GTPowerA6-10 "adc" should be refactored in the future,
//measurement should be done in an interrupt handler,
//the adc capacitor should be charged at a constant time.

//Then inputs: Vb0_pin,Vb1_pin,Vb2_pin should be measured less
//frequently,  because they have a big input resistance and the corresponding
//input capacitors need more time to charge.

/* if You want use interrupts then
 * most of the AnalogInputs methods should be rewritten
 * to be atomic. (see <util/atomic.h>)
 * We use a ADC_DELAY_US delay between setting the multiplexer address
 * and the ADC measurement which is also a problem.
 */
/*ISR(ADC_vect)
{
//    adc::processMeasurement();
//    adc::startConversion();
}*/


#define ADC_DELAY_US 60
//discharge ADC capacitor on Vb6 - there is an operational amplifier
#define ADC_CAPACITOR_DISCHARGE_ADDRESS MADDR_V_BALANSER6
#define ADC_CAPACITOR_DISCHARGE_DELAY_US 10
#define SLOW_INPUT_FACTOR 2


namespace adc {

struct adc_correlation {
    int8_t mux_;
    uint8_t adc_;
    AnalogInputs::Name ai_name_;
    bool trigger_PID_;
    bool slow_input_;
};

const adc_correlation order_analogInputs_on[] PROGMEM = {
    {MADDR_V_BALANSER_BATT_MINUS,   MUX0_Z_A_PIN,           AnalogInputs::Vb0_pin,         false, true},
    {-1,                            REVERSE_POLARITY_PIN,   AnalogInputs::VreversePolarity,false, false},
    {MADDR_V_BALANSER1,             MUX0_Z_A_PIN,           AnalogInputs::Vb1_pin,         false, true},
    {-1,                            SMPS_CURRENT_PIN,       AnalogInputs::Ismps,           true,  false},
    {MADDR_V_BALANSER2,             MUX0_Z_A_PIN,           AnalogInputs::Vb2_pin,         false, true},
    {-1,                            OUTPUT_VOLATAGE_PIN,    AnalogInputs::Vout ,           false, false},
    {MADDR_V_BALANSER6,             MUX0_Z_A_PIN,           AnalogInputs::Vb6_pin,         false, false},
    {-1,                            SMPS_CURRENT_PIN,       AnalogInputs::Ismps,           true,  false},
    {MADDR_V_BALANSER5,             MUX0_Z_A_PIN,           AnalogInputs::Vb5_pin,         false, false},
    {-1,                            DISCHARGE_CURRENT_PIN,  AnalogInputs::Idischarge,      false, false},
    {MADDR_V_BALANSER4,             MUX0_Z_A_PIN,           AnalogInputs::Vb4_pin,         false, false},
    {-1,                            SMPS_CURRENT_PIN,       AnalogInputs::Ismps,           true,  false},
    {MADDR_V_BALANSER3,             MUX0_Z_A_PIN,           AnalogInputs::Vb3_pin,         false, false},
    {-1,                            V_IN_PIN,               AnalogInputs::Vin,             false, false},
    {MADDR_T_EXTERN,                MUX0_Z_A_PIN,           AnalogInputs::Textern,         false, false},
    {-1,                            SMPS_CURRENT_PIN,       AnalogInputs::Ismps,           true,  false},
};


#define DEFAULT 1
#define EXTERNAL 0


static const uint8_t analog_reference = EXTERNAL;

static uint8_t current_input_;

void setADC(uint8_t pin) {
    // ADLAR - ADC Left Adjust Result
    ADMUX = (analog_reference << 6) | _BV(ADLAR) | (pin & 0x07);
}

inline uint8_t getPortBAddress(uint8_t address)
{
    return (PORTB & 0x1f) | (address & 7) << 5;
}

void setMuxAddress(uint8_t address)
{
    static uint8_t last = -1;
    if(address !=last) {
        last = address;
//        digitalWrite(MUX_ADR0_PIN, address&1);
//        digitalWrite(MUX_ADR1_PIN, address&2);
//        digitalWrite(MUX_ADR2_PIN, address&4);

        uint8_t new_portb = getPortBAddress(address);
        uint8_t disc_adr = getPortBAddress(ADC_CAPACITOR_DISCHARGE_ADDRESS);
        uint8_t bit = digitalPinToBitMask(MUX0_Z_D_PIN);
        uint8_t ddra_input = DDRA & (~bit);
        uint8_t ddra_output = DDRA | bit;

        //TODO: (yet another hack) do not discharge ADC capacitor when measuring:
        //Vb1_pin,Vb2_pin (slow inputs)
        if(address != MADDR_V_BALANSER1 && address != MADDR_V_BALANSER2) {
            //discharge ADC capacitor first
            PORTB = disc_adr;
            //pinMode(MUX0_Z_D_PIN, OUTPUT);
            DDRA = ddra_output;

            delayMicroseconds(ADC_CAPACITOR_DISCHARGE_DELAY_US);
        }

        //switch to the desired address
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            PORTB = new_portb;
            //pinMode(MUX0_Z_D_PIN, INPUT);
            DDRA = ddra_input;
        }
    }
}


AnalogInputs::Name getAIName(uint8_t input)
{
    return pgm::read(&order_analogInputs_on[input].ai_name_);
}

bool getSlowInput(uint8_t input)
{
    return pgm::read(&order_analogInputs_on[input].slow_input_);
}

uint8_t getADC(uint8_t input)
{
    return pgm::read(&order_analogInputs_on[input].adc_);
}

bool getTriggerPID(uint8_t input)
{
    return pgm::read(&order_analogInputs_on[input].trigger_PID_);
}

int8_t getMUX(uint8_t input)
{
    return pgm::read(&order_analogInputs_on[input].mux_);;
}


void processConversion(uint8_t input)
{
    uint8_t low, high;
    low  = ADCL;
    high = ADCH;

    AnalogInputs::adc_[getAIName(input)] = (high << 8) | low;
}

void reset()
{
    current_input_ = 0;
    setADC(getADC(current_input_));
    setMuxAddress(getMUX(current_input_));
}

void startConversion() {
    // start the conversion
    ADCSRA |= _BV(ADSC);
}


void finalizeMeasurement()
{
    AnalogInputs::adc_[AnalogInputs::IsmpsValue]        = SMPS::getValue();
    AnalogInputs::adc_[AnalogInputs::IdischargeValue]   = Discharger::getValue();
    AnalogInputs::finalizeMeasurement();
}

void initialize()
{
    digitalWrite(MUX0_Z_D_PIN, 0);
    pinMode(MUX0_Z_D_PIN, INPUT);

    pinMode(MUX_ADR0_PIN, OUTPUT);
    pinMode(MUX_ADR1_PIN, OUTPUT);
    pinMode(MUX_ADR2_PIN, OUTPUT);

    //ADEN: ADC Enable
    //ADIE: ADC Interrupt Enable
    //ADPS2:0: ADC Prescaler Select Bits = 16MHz/ 128 = 125kHz
//    ADCSRA = _BV(ADEN) | _BV(ADIE) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
    ADCSRA = _BV(ADEN) |  _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);

    adc::reset();
}

void doMeasurement()
{
    bool dontSave = getSlowInput(current_input_) && (AnalogInputs::avrCount_ % SLOW_INPUT_FACTOR);

    uint8_t mux = getMUX(current_input_);

    dontSave = dontSave || (settings.UART_ != Settings::Disabled && mux == MADDR_T_EXTERN);

    if(dontSave) {
        //TODO: disable temperature measurement when UART is on
        mux = MADDR_V_BALANSER5;
    }
    setMuxAddress(mux);
    setADC(getADC(current_input_ + 1));
    delayMicroseconds(ADC_DELAY_US);
    startConversion();
    while (bit_is_set(ADCSRA, ADSC));
    processConversion(current_input_ + 1);

    setADC(getADC(current_input_));
    delayMicroseconds(ADC_DELAY_US);
    startConversion();
    while (bit_is_set(ADCSRA, ADSC));
    if(!dontSave)
        processConversion(current_input_);


    bool trigger_PID = getTriggerPID(current_input_ + 1);

    if(trigger_PID)
        SMPS_PID::update();

    current_input_ += 2;
    if(current_input_ >= sizeOfArray(order_analogInputs_on))
        current_input_ = 0;

    if(AnalogInputs::isPowerOn() && current_input_ == 0)
        finalizeMeasurement();
    else {
        //info:improves the precision
    }
}

} //namespace adc


//this method depends on the ADC implementation
void AnalogInputs::resetADC()
{
    adc::reset();
}

//this method depends on the ADC implementation
void AnalogInputs::doFullMeasurement()
{
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

