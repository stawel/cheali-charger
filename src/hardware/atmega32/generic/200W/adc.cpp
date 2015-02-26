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

#include <util/delay_basic.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include "atomic.h"
#include "Hardware.h"
#include "GTPowerA6-10-pins.h"
#include "adc.h"
#include "Utils.h"
#include "memory.h"
#include "IO.h"
#include "Settings.h"
#include "AnalogInputsPrivate.h"

//#define ENABLE_DEBUG
#include "debug.h"

/* ADC - measurement:
 * program flow: see conversionDone()
 */

#define ADC_KEY_BORDER 128

namespace adc {

void setupNextInput();

void initialize()
{

    IO::pinMode(MUX0_Z_D_PIN, INPUT);
    IO::pinMode(MUX1_Z_D_PIN, INPUT);
    IO::digitalWrite(MUX0_Z_D_PIN, 0);
    IO::digitalWrite(MUX1_Z_D_PIN, 0);

    IO::pinMode(MUX_ADR0_PIN, OUTPUT);
    IO::pinMode(MUX_ADR1_PIN, OUTPUT);
    IO::pinMode(MUX_ADR2_PIN, OUTPUT);

    //ADC Auto Trigger Source - Free Running mode

    //ADEN: ADC Enable
    //ADATE: ADC Auto Trigger Enable
    //ADIE: ADC Interrupt Enable
    //ADPS2:0: ADC Prescaler Select Bits = 16MHz/ 64 = 250kHz (above the recommended value)
    /* atmega32 datasheet:
        By default, the successive approximation circuitry requires an input clock frequency between
        50kHz and 200kHz to get maximum resolution. If a lower resolution than 10 bits is needed, the
        input clock frequency to the ADC can be higher than 200kHz to get a higher sample rate. */
    ADCSRA = _BV(ADEN) | _BV(ADATE) | _BV(ADIE) | _BV(ADPS2) | _BV(ADPS1);

    //start conversion
    ADCSRA |= _BV(ADSC);
}


struct adc_correlation {
    int8_t mux;
    uint8_t adc;
    AnalogInputs::Name ai_name;
    uint8_t key;
    uint8_t noise;
};

#define ADC_STANDARD_PER_ROUND 2
#if MAX_BANANCE_CELLS > 6
#define ADC_I_DISCHARGE_PER_ROUND 2
#else
#define ADC_I_DISCHARGE_PER_ROUND 1
#endif

#define NO_NOISE 0

const adc_correlation order_analogInputs_on[] PROGMEM = {
    {-1,                    OUTPUT_VOLTAGE_PLUS_PIN,AnalogInputs::Vout_plus_pin,    0,              10},
    {MADDR_V_OUTMUX,        MUX0_Z_A_PIN ,          AnalogInputs::VoutMux,          0,              NO_NOISE},
    {MADDR_V_BALANSER1,     MUX1_Z_A_PIN,           AnalogInputs::Vb1_pin,          0,              NO_NOISE},
    {-1,                    OUTPUT_VOLTAGE_MINUS_PIN,AnalogInputs::Vout_minus_pin,  0,              10},
    {MADDR_T_INTERN,        MUX0_Z_A_PIN,           AnalogInputs::Tintern,          0,              NO_NOISE},
    {MADDR_V_BALANSER2,     MUX1_Z_A_PIN,           AnalogInputs::Vb2_pin,          0,              NO_NOISE},
    {-1,                    SMPS_CURRENT_PIN,       AnalogInputs::Ismps,            0,              NO_NOISE},
    {MADDR_V_IN,            MUX0_Z_A_PIN,           AnalogInputs::Vin,              0,              NO_NOISE},
    {MADDR_V_BALANSER3,     MUX1_Z_A_PIN,           AnalogInputs::Vb3_pin,          0,              NO_NOISE},
    {-1,                    DISCHARGE_CURRENT_PIN,  AnalogInputs::Idischarge,       0,              NO_NOISE},
    {MADDR_T_EXTERN,        MUX0_Z_A_PIN,           AnalogInputs::Textern,          0,              NO_NOISE},
    {MADDR_V_BALANSER4,     MUX1_Z_A_PIN,           AnalogInputs::Vb4_pin,          0,              NO_NOISE},
    {-1,                    OUTPUT_VOLTAGE_PLUS_PIN,AnalogInputs::Vout_plus_pin,    0,              10},
    {MADDR_BUTTON_DEC,      MUX0_Z_A_PIN,           AnalogInputs::VirtualInputs,    BUTTON_DEC,     NO_NOISE},
    {MADDR_V_BALANSER5,     MUX1_Z_A_PIN,           AnalogInputs::Vb5_pin,          0,              NO_NOISE},
    {-1,                    OUTPUT_VOLTAGE_MINUS_PIN,AnalogInputs::Vout_minus_pin,  0,              10},
    {MADDR_BUTTON_INC,      MUX0_Z_A_PIN,           AnalogInputs::VirtualInputs,    BUTTON_INC,     NO_NOISE},
    {MADDR_V_BALANSER6,     MUX1_Z_A_PIN,           AnalogInputs::Vb6_pin,          0,              NO_NOISE},
#if MAX_BANANCE_CELLS > 6
    {-1,                    SMPS_CURRENT_PIN,       AnalogInputs::Ismps,            0,              NO_NOISE},
    {MADDR_BUTTON_STOP,     MUX0_Z_A_PIN,           AnalogInputs::VirtualInputs,    BUTTON_STOP,    NO_NOISE},
    {MADDR_V_BALANSER7,     MUX1_Z_A_PIN,           AnalogInputs::Vb7_pin,          0,              NO_NOISE},
    {-1,                    DISCHARGE_CURRENT_PIN,  AnalogInputs::Idischarge,       0,              NO_NOISE},
    {MADDR_BUTTON_START,    MUX0_Z_A_PIN,           AnalogInputs::VirtualInputs,    BUTTON_START,   NO_NOISE},
    {MADDR_V_BALANSER8,     MUX1_Z_A_PIN,           AnalogInputs::Vb8_pin,          0,              NO_NOISE},
#else
    {MADDR_BUTTON_STOP,     MUX0_Z_A_PIN,           AnalogInputs::VirtualInputs,    BUTTON_STOP,    NO_NOISE},
    {-1,                    SMPS_CURRENT_PIN,       AnalogInputs::Ismps,            0,              NO_NOISE},
    {MADDR_BUTTON_START,    MUX0_Z_A_PIN,           AnalogInputs::VirtualInputs,    BUTTON_START,   NO_NOISE},
#endif
};

inline uint8_t nextInput(uint8_t i) {
    if(++i >= sizeOfArray(order_analogInputs_on)) i=0;
    return i;
}
adc_correlation adc_input;
adc_correlation adc_input_next;
static volatile uint8_t g_addSumToInput = 0;
static volatile uint8_t g_input_ = 0;
static volatile uint8_t g_adcBurstCount_ = 0;

static uint8_t adc_keyboard_;


inline void setADC(uint8_t pin) {
    // ADLAR - ADC Left Adjust Result
    ADMUX = (EXTERNAL << 6)
            | _BV(ADLAR)
            | pin;
}

inline uint8_t getPortBAddress(int8_t address)
{
    uint8_t port_adr =  ((address&1) << 2) | (address&2) | ((address&4) >>2);
    return (PORTB & 0x1f) | (port_adr) << 5;
}


uint16_t processConversion()
{
    uint8_t low, high, key;
    uint16_t v;
    low  = ADCL;
    high = ADCH;

    AnalogInputs::Name name = adc_input.ai_name;
    if(name != AnalogInputs::VirtualInputs) {
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            v = (high << 8) | low;
            AnalogInputs::i_adc_[name] = v;
            if(g_addSumToInput)
                AnalogInputs::i_avrSum_[name] += v;
        }
    } else {
        key = adc_input.key;
        if(high < ADC_KEY_BORDER) {
            adc_keyboard_ |= key;
        } else {
            adc_keyboard_ &= ~key;
        }
    }
    return v;
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

#ifdef ENABLE_DEBUG
#define MAX_DEBUG_DATA 160
uint16_t adcDebugData[MAX_DEBUG_DATA];
uint8_t adcDebugCount = 0;
bool adcDebugStart = false;
#endif

void debug() {
#ifdef ENABLE_DEBUG
    if(adcDebugCount == MAX_DEBUG_DATA){
        for (int i=0;i<MAX_DEBUG_DATA;i++) {
            LogDebug(i, ':', adcDebugData[i]);
        }
        LogDebug('-');
        adcDebugStart = false;
        adcDebugCount = 0;
    }
#endif
}

void addAdcNoise()
{
    if(!settings.adcNoise)
        return;
    if(adc_input_next.noise == NO_NOISE)
        return;

    uint8_t adcbit = 1 << adc_input_next.adc;
    uint8_t time = AnalogInputs::i_avrCount_ & 15;
    time += adc_input_next.noise;

    //we only add positive noise

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        // PORTA up (charge, pin: Pull-up resistor)
        uint8_t old_porta = PORTA;
        PORTA |= adcbit;
        _delay_loop_1(time);
        PORTA = old_porta;
    }
}

#if ANALOG_INPUTS_ADC_BURST_COUNT < 4
#error "ANALOG_INPUTS_ADC_BURST_COUNT < 4"
#endif

void conversionDone()
{
    uint16_t v = processConversion();

#ifdef ENABLE_DEBUG
    if(g_adcBurstCount_ == 0 && adc_input.ai_name == AnalogInputs::Vout_plus_pin) {
        if(adcDebugCount < MAX_DEBUG_DATA) {
            adcDebugStart = true;
        }
    }
    if(adcDebugStart && adcDebugCount < MAX_DEBUG_DATA
            && adc_input.ai_name == AnalogInputs::VirtualInputs
           ) {
        if(g_adcBurstCount_ == 0)
            adcDebugData[adcDebugCount++] = adc_input.ai_name; //AnalogInputs::i_avrCount_;
        adcDebugData[adcDebugCount++] = v;
    }
#endif

    switch(g_adcBurstCount_++) {
    case 0:
        /* set new mux address */
        if(adc_input_next.mux >= 0) {
            ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
                PORTB = getPortBAddress(adc_input_next.mux);
            }
        }

        break;

#ifdef ENABLE_ANALOG_INPUTS_ADC_NOISE
    case ANALOG_INPUTS_ADC_BURST_COUNT-3:
        addAdcNoise();
        break;
#endif

    case ANALOG_INPUTS_ADC_BURST_COUNT-2:
        /* set next adc input */
        setADC(adc_input_next.adc);
        break;

    case ANALOG_INPUTS_ADC_BURST_COUNT-1:
        /* switch to new input */
        g_adcBurstCount_ = 0;
        setupNextInput();
    }
}

void setupNextInput() {
    g_input_ = nextInput(g_input_);
    adc_input = adc_input_next;
    pgm::read(adc_input_next, &order_analogInputs_on[nextInput(g_input_)]);

    if(g_input_ == 0) {
        finalizeMeasurement();
        g_addSumToInput = AnalogInputs::i_avrCount_ > 0;
    }
}

}// namespace adc

uint8_t hardware::getKeyPressed()
{
    return adc::adc_keyboard_;
}

ISR(ADC_vect)
{
    adc::conversionDone();
}
