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
#include "imaxB6.h"
#include "TimerOne.h"
#include "imaxB6-pins.h"
#include "SMPS_PID.h"

Multiplexer<MUX_ADR0_PIN, MUX_ADR1_PIN, MUX_ADR2_PIN,
          MUX0_Z_D_PIN, MUX0_Z_D_PIN,
          MUX0_Z_A_PIN, MUX0_Z_A_PIN>
 mux;

uint16_t smpsValue()
{
    return smps.getValue();
}

uint16_t dischargerValue()
{
    return discharger.getValue();
}


uint16_t reversePolarityValue()
{
    uint16_t vr = AnalogInputs::analogValue<REVERSE_POLARITY_PIN>();
    uint16_t vo = AnalogInputs::analogValue<OUTPUT_VOLATAGE_PIN>();
    if(vr > vo) return vr - vo;
    return 0;
}

uint16_t empty()
{
    return 0;
}

const AnalogInputs::DefaultValues AnalogInputs::inputsP_[AnalogInputs::PHYSICAL_INPUTS] PROGMEM = {
    {{0,0},                         {25920, ANALOG_VOLT(11.945)}},  //Vout
    {{0,0},                         {38000, ANALOG_VOLT(11.829)}},  //VreversePolarity
    {{390,ANALOG_AMP(0.050)},       {10966, ANALOG_AMP(1.000)}},    //Ismps
    {{0,0},                         {14212, ANALOG_AMP(0.100)}},    //Idischarge

    {{0, 0},                        {0, ANALOG_VOLT(0)}},           //VoutMux
    {{0, 0},                        {0, ANALOG_CELCIUS(0)}},        //Tintern
    {{0, 0},                        {48063, ANALOG_VOLT(14.044)}},  //Vin
    {{5765,ANALOG_CELCIUS(23.2)},   {14300, ANALOG_CELCIUS(60)}},   //Textern

    {{0, 0},                        {52072, ANALOG_VOLT(3.985)}},   //Vb0
    {{0, 0},                        {52686, ANALOG_VOLT(3.984)}},   //Vb1
    {{0, 0},                        {52094, ANALOG_VOLT(3.935)}},   //Vb2

    {{0, 0},                        {51180, ANALOG_VOLT(3.867)}},   //Vb3
    {{0, 0},                        {51130, ANALOG_VOLT(3.866)}},   //Vb4
    {{0, 0},                        {49348, ANALOG_VOLT(3.876)}},   //Vb5

    //1-1 correlation
    {{0,0},                         {10000, 10000}},                //IsmpsValue
    {{0, 0},                        {82,    ANALOG_AMP(0.100)}},    //IdischargeValue
#ifdef ANALOG_INPUTS_V_UNKNOWN
    {{0,0},                         {1, 1}},                        //UNKNOWN0
    {{0,0},                         {1, 1}},                        //UNKNOWN1
#endif
};

uint8_t hardware::getKeyPressed()
{
    return   (digitalRead(BUTTON_STOP_PIN) ? 0 : BUTTON_STOP)
            | (digitalRead(BUTTON_DEC_PIN)  ? 0 : BUTTON_DEC)
            | (digitalRead(BUTTON_INC_PIN)  ? 0 : BUTTON_INC)
            | (digitalRead(BUTTON_START_PIN)? 0 : BUTTON_START);
}

void hardware::init()
{
    analogReference(EXTERNAL);
    pinMode(OUTPUT_DISABLE_PIN, OUTPUT);

    pinMode(DISCHARGE_VALUE_PIN, OUTPUT);
    pinMode(DISCHARGE_DISABLE_PIN, OUTPUT);

    pinMode(SMPS_VALUE0_PIN, OUTPUT);
    pinMode(SMPS_VALUE1_PIN, OUTPUT);
    pinMode(SMPS_DISABLE_PIN, OUTPUT);

    pinMode(BUZZER_PIN, OUTPUT);

    pinMode(BALANCER1_LOAD_PIN, OUTPUT);
    pinMode(BALANCER2_LOAD_PIN, OUTPUT);
    pinMode(BALANCER3_LOAD_PIN, OUTPUT);
    pinMode(BALANCER4_LOAD_PIN, OUTPUT);
    pinMode(BALANCER5_LOAD_PIN, OUTPUT);
    pinMode(BALANCER6_LOAD_PIN, OUTPUT);

    pinMode(MUX_ADR0_PIN, OUTPUT);
    pinMode(MUX_ADR1_PIN, OUTPUT);
    pinMode(MUX_ADR2_PIN, OUTPUT);
    pinMode(MUX0_Z_D_PIN, INPUT);

    setBatteryOutput(false);
    setFan(false);
    setBuzzer(0);

    lcd.begin(LCD_COLUMNS, LCD_LINES);
    timer.init();

    TimerOne::initialize();
}

void hardware::setLCDBacklight(uint8_t val)
{
}

void hardware::setFan(bool enable)
{
}
void hardware::setBuzzer(uint16_t val)
{
    digitalWrite(BUZZER_PIN, val>0);
}

void hardware::setBatteryOutput(bool enable)
{
    digitalWrite(OUTPUT_DISABLE_PIN, !enable);
}

void hardware::setBalancer(uint16_t v)
{
    digitalWrite(BALANCER1_LOAD_PIN, v&1);
    digitalWrite(BALANCER2_LOAD_PIN, v&2);
    digitalWrite(BALANCER3_LOAD_PIN, v&4);
    digitalWrite(BALANCER4_LOAD_PIN, v&8);
    digitalWrite(BALANCER5_LOAD_PIN, v&16);
    digitalWrite(BALANCER6_LOAD_PIN, v&32);
}


void measureValue(AnalogInputs::Name name)
{
    AnalogInputs::ValueType v = 0;
    switch(name) {
    case AnalogInputs::Vout:            v = AnalogInputs::analogValue<OUTPUT_VOLATAGE_PIN>(); break;
    case AnalogInputs::VreversePolarity:v = reversePolarityValue(); break;
    case AnalogInputs::Ismps:           v = AnalogInputs::analogValue<SMPS_CURRENT_PIN>(); break;
    case AnalogInputs::Idischarge:      v = AnalogInputs::analogValue<DISCHARGE_CURRENT_PIN>(); break;

    case AnalogInputs::Vin:             v = AnalogInputs::analogValue<V_IN_PIN>(); break;
    case AnalogInputs::Textern:         v = mux.analogRead<MADDR_T_EXTERN>(); break;

    case AnalogInputs::Vb1:             v = mux.analogRead<MADDR_V_BALANSER1>(); break;
    case AnalogInputs::Vb2:             v = mux.analogRead<MADDR_V_BALANSER2>(); break;
    case AnalogInputs::Vb3:             v = mux.analogRead<MADDR_V_BALANSER3>(); break;
    case AnalogInputs::Vb4:             v = mux.analogRead<MADDR_V_BALANSER4>(); break;
    case AnalogInputs::Vb5:             v = mux.analogRead<MADDR_V_BALANSER5>(); break;
    case AnalogInputs::Vb6:             v = mux.analogRead<MADDR_V_BALANSER6>(); break;

    case AnalogInputs::IsmpsValue:      v = smpsValue(); break;
    case AnalogInputs::IdischargeValue: v = dischargerValue(); break;

    default: break;
    }
    analogInputs.measured_[name] = v;
}

// TODO: maybe it should be: hardware::doMeasurement
void AnalogInputs::doMeasurement(uint16_t count)
{
    while (count) {
        count--;
        measureValue(currentInput_);
        measureValue(AnalogInputs::AnalogInputs::Ismps);
        SMPS_PID::newIsmpsMeasurement();

        avrSum_[currentInput_] += measured_[currentInput_];
        currentInput_ = Name(currentInput_ + 1);
        if(currentInput_ == VirtualInputs) {
            finalizeMeasurement();
        }
    }
}

void hardware::delay(uint16_t t)
{
    if(analogInputs.on_)
        analogInputs.doMeasurement((AnalogInputs::PHYSICAL_INPUTS*t)/8);
    else {
        measureValue(AnalogInputs::VreversePolarity);
        timer.delay(t);
    }
}


Keyboard keyboard;
SMPS smps;
Discharger discharger;
Balancer balancer;
AnalogInputs analogInputs;

LiquidCrystal lcd(LCD_ENABLE_RS, LCD_ENABLE_PIN,
        LCD_D0_PIN, LCD_D1_PIN, LCD_D2_PIN, LCD_D3_PIN);

