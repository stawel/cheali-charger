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
#include "Hardware.h"
#include "imaxB6-pins.h"
#include "SMPS_PID.h"
#include "adc.h"
#include "Utils.h"
#include "memory.h"
#include <avr/io.h>


#define ADC_DELAY_MS 30

const AnalogInputs::DefaultValues AnalogInputs::inputsP_[AnalogInputs::PHYSICAL_INPUTS] PROGMEM = {
    {{0,0},                         {25920, ANALOG_VOLT(11.945)}},  //Vout
    {{0,0},                         {38000, ANALOG_VOLT(11.829)}},  //VreversePolarity
    {{390,ANALOG_AMP(0.050)},       {10966, ANALOG_AMP(1.000)}},    //Ismps
    {{0,0},                         {14212, ANALOG_AMP(0.100)}},    //Idischarge

    {{0, 0},                        {0, ANALOG_VOLT(0)}},           //VoutMux
    {{0, 0},                        {0, ANALOG_CELCIUS(0)}},        //Tintern
    {{0, 0},                        {48063, ANALOG_VOLT(14.044)}},  //Vin
    {{19842,ANALOG_CELCIUS(25.0)},  {18750, ANALOG_CELCIUS(60)}},   //Textern

    {{0, 0},                        {52072, ANALOG_VOLT(3.985)}},   //Vb0_real
    {{0, 0},                        {54079, ANALOG_VOLT(4.138)}},   //Vb1_real
    {{0, 0},                        {54682, ANALOG_VOLT(8.270)}},   //Vb2_real
    {{0, 0},                        {55246, ANALOG_VOLT(4.182)}},   //Vb3_real

    {{0, 0},                        {53726, ANALOG_VOLT(4.065)}},   //Vb4_real
    {{0, 0},                        {53680, ANALOG_VOLT(4.071)}},   //Vb5_real
    {{0, 0},                        {51656, ANALOG_VOLT(4.073)}},   //Vb6_real

    //1-1 correlation
    {{0,0},                         {10000, 10000}},                //IsmpsValue
    {{0, 0},                        {82,    ANALOG_AMP(0.100)}},    //IdischargeValue
#ifdef ANALOG_INPUTS_V_UNKNOWN
    {{0,0},                         {1, 1}},                        //UNKNOWN0
    {{0,0},                         {1, 1}},                        //UNKNOWN1
#endif
};



struct adc_correlation {
    int8_t mux_;
    uint8_t adc_;
    AnalogInputs::Name ai_name_;
    bool trigger_PID_;
};

const adc_correlation order_analogInputs_on[] PROGMEM = {
    {MADDR_T_EXTERN,                MUX0_Z_A_PIN,           AnalogInputs::Textern,          false},
    {-1,                            SMPS_CURRENT_PIN,       AnalogInputs::Ismps,            true},
    {MADDR_V_BALANSER_BATT_MINUS,   MUX0_Z_A_PIN,           AnalogInputs::Vb0_real,         false},
    {-1,                            REVERSE_POLARITY_PIN,   AnalogInputs::VreversePolarity, false},
    {MADDR_V_BALANSER6,             MUX0_Z_A_PIN,           AnalogInputs::Vb6_real,         false},
    {-1,                            SMPS_CURRENT_PIN,       AnalogInputs::Ismps,            true},
    {MADDR_V_BALANSER5,             MUX0_Z_A_PIN,           AnalogInputs::Vb5_real,         false},
    {-1,                            DISCHARGE_CURRENT_PIN,  AnalogInputs::Idischarge,       false},
    {MADDR_V_BALANSER4,             MUX0_Z_A_PIN,           AnalogInputs::Vb4_real,         false},
    {-1,                            SMPS_CURRENT_PIN,       AnalogInputs::Ismps,            true},
    {MADDR_V_BALANSER3,             MUX0_Z_A_PIN,           AnalogInputs::Vb3_real,         false},
    {-1,                            V_IN_PIN,               AnalogInputs::Vin,              false},
    {MADDR_V_BALANSER1,             MUX0_Z_A_PIN,           AnalogInputs::Vb1_real,         false},
    {-1,                            SMPS_CURRENT_PIN,       AnalogInputs::Ismps,            true},
    {MADDR_V_BALANSER2,             MUX0_Z_A_PIN,           AnalogInputs::Vb2_real,         false},
    {-1,                            OUTPUT_VOLATAGE_PIN,    AnalogInputs::Vout ,            false},
};


#define DEFAULT 1
#define EXTERNAL 0

namespace {

static const uint8_t analog_reference = EXTERNAL;

static uint8_t current_input;

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
        PORTB = (PORTB & 0x1f) | (address & 7) << 5;
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

bool getTriggerPID(uint8_t input)
{
    return pgm::read(&order_analogInputs_on[input].trigger_PID_);
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

void adc::processConversion(bool finalize)
{
    uint8_t low, high;
    bool trigger_PID;
    low  = ADCL;
    high = ADCH;

    analogInputs.measured_[getAIName(current_input)] = (high << 8) | low;
    trigger_PID = getTriggerPID(current_input);

    current_input = nextInput(current_input);
    setADC(getADC(current_input));
    setMuxAddress(getMUX(current_input));

    if(trigger_PID)
        SMPS_PID::update();

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
    analogInputs.measured_[AnalogInputs::IsmpsValue]        = smps.getValue();
    analogInputs.measured_[AnalogInputs::IdischargeValue]   = discharger.getValue();
    analogInputs.finalizeMeasurement();
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
    processConversion(analogInputs.on_);
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
    uint32_t end = timer.getMiliseconds() + t;
    do {
        adc::doMeasurement();
    } while(timer.getMiliseconds() < end);
}

