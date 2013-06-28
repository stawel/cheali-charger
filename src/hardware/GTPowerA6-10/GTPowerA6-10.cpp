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
#include "Multiplexer.h"

#include "GTPowerA6-10-pins.h"
#include "GTPowerA6-10.h"


Multiplexer<MUX_ADR0_PIN, MUX_ADR1_PIN, MUX_ADR2_PIN,
          MUX0_Z_D_PIN, MUX1_Z_D_PIN,
          MUX0_Z_A_PIN, MUX1_Z_A_PIN> mux;

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
//    return vr;
    uint16_t vo = AnalogInputs::analogValue<OUTPUT_VOLATAGE_PIN>();
    if(vr > vo) return vr - vo;
    return 0;
}

const AnalogInputs::DefaultValues AnalogInputs::inputsP_[AnalogInputs::PHYSICAL_INPUTS] PROGMEM = {
    {{0,0},                         {50816, ANALOG_VOLT(23.118)}},  //Vout
    {{0,0},                         {27516, ANALOG_VOLT(12.552)}},  //VreversePolarity
    {{256,   ANALOG_AMP(0.051)},    {10240, ANALOG_AMP(2.000)}},    //Ismps
    {{384,   ANALOG_AMP(0.051)},    { 9024, ANALOG_AMP(1.000)}},    //Idischarge

    {{0, 0},                        {44736, ANALOG_VOLT(23.118)}},  //VoutMux
    {{21500, ANALOG_CELCIUS(52)},   {41023, ANALOG_CELCIUS(29)}},   //Tintern
    {{0, 0},                        {47872, ANALOG_VOLT(14.020)}},  //Vin
    {{6272,  ANALOG_CELCIUS(24)},   {14300, ANALOG_CELCIUS(60)}},   //Textern

    {{0, 0},                        {0, ANALOG_VOLT(0)}},           //Vb0_real -??
    {{0, 0},                        {50244, ANALOG_VOLT(3.834)}},   //Vb1_real
    {{0, 0},                        {50176, ANALOG_VOLT(3.835)}},   //Vb2_real
    {{0, 0},                        {50280, ANALOG_VOLT(3.837)}},   //Vb3_real

    {{0, 0},                        {50602, ANALOG_VOLT(3.862)}},   //Vb4_real
    {{0, 0},                        {50752, ANALOG_VOLT(3.865)}},   //Vb5_real
    {{0, 0},                        {51008, ANALOG_VOLT(3.885)}},   //Vb6_real

    {{22, ANALOG_AMP(0.051)},       {744, ANALOG_AMP(2.000)}},      //IsmpsValue
    {{32, ANALOG_AMP(0.051)},       {657, ANALOG_AMP(1.000)}},      //IdischargeValue
#ifdef ANALOG_INPUTS_V_UNKNOWN
    {{0,0},                         {1, 1}},                        //UNKNOWN0
    {{0,0},                         {1, 1}},                        //UNKNOWN1
#endif
};

uint8_t hardware::getKeyPressed()
{
    return   (mux.digitalRead(MADDR_BUTTON_STOP) ? 0 : BUTTON_STOP)
            | (mux.digitalRead(MADDR_BUTTON_DEC)  ? 0 : BUTTON_DEC)
            | (mux.digitalRead(MADDR_BUTTON_INC)  ? 0 : BUTTON_INC)
            | (mux.digitalRead(MADDR_BUTTON_START)? 0 : BUTTON_START);
}

void hardware::initialize()
{
    analogReference(EXTERNAL);
    pinMode(BACKLIGHT_PIN, OUTPUT);
    pinMode(OUTPUT_DISABLE_PIN, OUTPUT);
    pinMode(FAN_PIN, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);

    pinMode(SMPS_VALUE_PIN, OUTPUT);
    pinMode(SMPS_DISABLE_PIN, OUTPUT);
    pinMode(DISCHARGE_VALUE_PIN, OUTPUT);
    pinMode(DISCHARGE_DISABLE_PIN, OUTPUT);

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
    pinMode(MUX1_Z_D_PIN, INPUT);

    setBatteryOutput(false);
    setFan(false);
    hardware::setBuzzer(0);

    lcd.begin(LCD_COLUMNS, LCD_LINES);
    timer.init();

    Timer1.initialize(TIMER1_PERIOD_MICROSECONDS);         // initialize timer1, and set a 1/2 second period
}

void hardware::setLCDBacklight(uint8_t val)
{
    uint32_t v1,v2;
    v1  = LCD_BACKLIGHT_MAX;
    v1 *= val;
    v2  = LCD_BACKLIGHT_MIN;
    v2 *= 100 - val;
    v1+=v2;
    v1/=100;
    Timer1.pwm(BACKLIGHT_PIN, v1);
}

void hardware::setFan(bool enable)
{
    digitalWrite(FAN_PIN, enable);
}
void hardware::setBuzzer(uint16_t val)
{
    analogWrite(BUZZER_PIN, val);
}

void hardware::setBatteryOutput(bool enable)
{
    digitalWrite(OUTPUT_DISABLE_PIN, !enable);
}
void hardware::setChargerOutput(bool enable)
{
    digitalWrite(SMPS_DISABLE_PIN, !enable);
}
void hardware::setDischargerOutput(bool enable)
{
    digitalWrite(DISCHARGE_DISABLE_PIN, !enable);
}

void hardware::setChargerValue(uint16_t value)
{
    Timer1.pwm(SMPS_VALUE_PIN, value);
}
void hardware::setDischargerValue(uint16_t value)
{
    Timer1.pwm(DISCHARGE_VALUE_PIN, value);
}

void hardware::setBalancer(uint8_t v)
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

    case AnalogInputs::VoutMux:         v = mux.analogRead<MADDR_V_OUTMUX>(); break;
    case AnalogInputs::Tintern:         v = mux.analogRead<MADDR_T_INTERN>(); break;

    case AnalogInputs::Vin:             v = mux.analogRead<MADDR_V_IN>(); break;
    case AnalogInputs::Textern:         v = mux.analogRead<MADDR_T_EXTERN>(); break;

    //TODO: case AnalogInputs::Vb0_real:
    case AnalogInputs::Vb1_real:        v = mux.analogRead<MADDR_V_BALANSER1>(); break;
    case AnalogInputs::Vb2_real:        v = mux.analogRead<MADDR_V_BALANSER2>(); break;
    case AnalogInputs::Vb3_real:        v = mux.analogRead<MADDR_V_BALANSER3>(); break;
    case AnalogInputs::Vb4_real:        v = mux.analogRead<MADDR_V_BALANSER4>(); break;
    case AnalogInputs::Vb5_real:        v = mux.analogRead<MADDR_V_BALANSER5>(); break;
    case AnalogInputs::Vb6_real:        v = mux.analogRead<MADDR_V_BALANSER6>(); break;

    case AnalogInputs::IsmpsValue:      v = smpsValue(); break;
    case AnalogInputs::IdischargeValue: v = dischargerValue(); break;

#ifdef ANALOG_INPUTS_V_UNKNOWN
    case AnalogInputs::Vunknown0:       v = mux.analogRead<MADDR_V_UNKNOWN0>(); break;
    case AnalogInputs::Vunknown1:       v = mux.analogRead<MADDR_V_UNKNOWN1>(); break;
#endif

    default:  break;
    }
    analogInputs.measured_[name] = v;
}

// TODO: maybe it should be: hardware::doMeasurement
void AnalogInputs::doMeasurement(uint16_t count)
{
    while (count) {
        count--;
        measureValue(currentInput_);
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
        analogInputs.doMeasurement((AnalogInputs::PHYSICAL_INPUTS*t)/4);
    else {
        measureValue(AnalogInputs::VreversePolarity);
        measureValue(AnalogInputs::Tintern);
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

