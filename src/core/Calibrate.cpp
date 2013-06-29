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
#include "Options.h"
#include "LcdPrint.h"
#include "StaticMenu.h"
#include "SMPS.h"
#include "Calibrate.h"
#include "Utils.h"
#include "Screen.h"
#include "Buzzer.h"
#include "StackInfo.h"


#ifdef ENABLE_B0_CALIBRATION
const char string_c0[] PROGMEM = "B0";
#endif
const char string_c1[] PROGMEM = "B1-3";
const char string_c2[] PROGMEM = "B4-6";
const char string_c3[] PROGMEM = "B1-6 to Vout";
const char string_c4[] PROGMEM = "Icharge";
const char string_c5[] PROGMEM = "Idischarge";
const char string_c6[] PROGMEM = "Vin";
const char string_c7[] PROGMEM = "Tintern";
const char string_c8[] PROGMEM = "Textern";
const char string_c9[] PROGMEM = "Info-debug";

const char string_ci0[] PROGMEM = "Icharge";
const char string_ci1[] PROGMEM = "Vout";
const char string_ci2[] PROGMEM = "B1-3";
const char string_ci3[] PROGMEM = "B4-6";
const char string_ci4[] PROGMEM = "Temp";
const char string_ci5[] PROGMEM = "Vin";
const char string_ci6[] PROGMEM = "Vreversed";
const char string_ci7[] PROGMEM = "Vunknown";
const char string_ci8[] PROGMEM = "Idischarge";
const char string_ci9[] PROGMEM = "time";
#ifdef ENABLE_STACK_INFO
const char string_cia[] PROGMEM = "stack info";
#endif

const char * const calibrateMenu[] PROGMEM =
{
#ifdef ENABLE_B0_CALIBRATION
  string_c0,
#endif
  string_c1,
  string_c2,
  string_c3,
  string_c4,
  string_c5,
  string_c6,
  string_c7,
  string_c8,
  string_c9,
};

const char * const calibrateInfoMenu[] PROGMEM =
{ string_ci0,
  string_ci1,
  string_ci2,
  string_ci3,
  string_ci4,
  string_ci5,
  string_ci6,
  string_ci7,
  string_ci8,
  string_ci9,
#ifdef ENABLE_STACK_INFO
  string_cia,
#endif
};


const char string_cI0[] PROGMEM = "50mA";
const char string_cI1[] PROGMEM = "1000mA";

const char string_cID1[] PROGMEM = "300mA";

const char * const calibrateICMenu[] PROGMEM =
{
    string_cI0,
    string_cI1,
};

const char * const calibrateIDMenu[] PROGMEM =
{
    string_cI0,
    string_cID1,
};

namespace {
void setCalibrationPoint(AnalogInputs::Name name, uint8_t i, const AnalogInputs::CalibrationPoint &x)
{
    buzzer.soundSave();
    analogInputs.setCalibrationPoint(name, i, x);
}
}


void Calibrate::calibrateI(screenType screen, AnalogInputs::Name name1, AnalogInputs::Name name2)
{
    AnalogInputs::ValueType value1 = ANALOG_AMP(0.3);
    const char * const *menu_str = calibrateIDMenu;
    if(name1==AnalogInputs::Ismps) {
        menu_str = calibrateICMenu;
        value1 = ANALOG_AMP(1);
    }
    StaticMenu menu(menu_str, sizeOfArray(calibrateICMenu));
    AnalogInputs::CalibrationPoint p;
    int8_t i = 0;
    do {
        i = menu.runSimple();
        if(i < 0) return;
        if(calibrate(screen)) {
            if(i == 0) {
                p.y = ANALOG_AMP(0.05);
            } else {
                i = 1;
                p.y = value1;
            }
            p.x = analogInputs.getValue(name1);
            setCalibrationPoint(name1, i, p);
            p.x = analogInputs.getValue(name2);
            setCalibrationPoint(name2, i, p);
        }
    } while(i >= 0);
}

void Calibrate::copyVbalVout()
{
    AnalogInputs::CalibrationPoint p;
    //TODO: remove the ability to change smps.value
    if(calibrate(SCREEN_VOUT_VBAL)) {
        p.x = analogInputs.getValue(AnalogInputs::Vout);
        p.y = analogInputs.getRealValue(AnalogInputs::Vbalancer);
        setCalibrationPoint(AnalogInputs::Vout, 1, p);

        p.x = analogInputs.getValue(AnalogInputs::VoutMux);
        setCalibrationPoint(AnalogInputs::VoutMux, 1, p);
    }
}

#ifdef ENABLE_SIMPLIFIED_VB0_VB2_CIRCUIT
void calibrateSimplifiedVb1_real(AnalogInputs::ValueType real_v)
{
    AnalogInputs::CalibrationPoint p1,p2;
    p1.x = analogInputs.getValue(AnalogInputs::Vb1_real);
    p1.y = real_v + analogInputs.getRealValue(AnalogInputs::Vb0_real);
    p2.x = analogInputs.getValue(AnalogInputs::Vb2_real);
    p2.y = p1.y + analogInputs.getRealValue(AnalogInputs::Vb2);
    setCalibrationPoint(AnalogInputs::Vb1_real, 1, p1);
    setCalibrationPoint(AnalogInputs::Vb2_real, 1, p2);
}
void calibrateSimplifiedVb2_real(AnalogInputs::ValueType real_v)
{
    AnalogInputs::CalibrationPoint p2;
    p2.x = analogInputs.getValue(AnalogInputs::Vb2_real);
    p2.y = real_v + analogInputs.getRealValue(AnalogInputs::Vb1_real);
    setCalibrationPoint(AnalogInputs::Vb2_real, 1, p2);
}

#endif


void Calibrate::setBalancer(AnalogInputs::Name firstName)
{
    AnalogInputs::Name name = AnalogInputs::Name(firstName + blink_);
    uint8_t x,y;
    switch(blink_) {
    case 0: x = 10; y = 0; break;
    case 1: x = 2;  y = 1; break;
    case 2: x = 10; y = 1; break;
    }
    AnalogInputs::CalibrationPoint p;

#ifdef ENABLE_SIMPLIFIED_VB0_VB2_CIRCUIT
    AnalogInputs::Name virtual_name = name;

    if(name == AnalogInputs::Vb1_real) virtual_name = AnalogInputs::Vb1;
    if(name == AnalogInputs::Vb2_real) virtual_name = AnalogInputs::Vb2;
    p.y = analogInputs.getRealValue(virtual_name);
#else
    p.y = analogInputs.getRealValue(name);
#endif
    if(setValue(x, y, p.y, AnalogInputs::Voltage, 6)) {
        p.x = analogInputs.getValue(name);

#ifdef ENABLE_SIMPLIFIED_VB0_VB2_CIRCUIT
        if(name == AnalogInputs::Vb1_real)
            calibrateSimplifiedVb1_real(p.y);
        else if(name == AnalogInputs::Vb2_real)
            calibrateSimplifiedVb2_real(p.y);
        else
            setCalibrationPoint(name, 1, p);
#else
        setCalibrationPoint(name, 1, p);
#endif
    }
}

void Calibrate::run()
{
    StaticMenu menu(calibrateMenu, sizeOfArray(calibrateMenu));
    int8_t i;
    do {
        i = menu.runSimple();
        if(i<0) break;
        START_CASE_COUNTER;
        switch(i) {
#ifdef ENABLE_B0_CALIBRATION
        case NEXT_CASE: calibrateBlink(SCREEN_B0_BLINK, 1); break;
#endif
        case NEXT_CASE: calibrateBlink(SCREEN_B1_3_BLINK, 3); break;
        case NEXT_CASE: calibrateBlink(SCREEN_B4_6_BLINK, 3); break;
        case NEXT_CASE: copyVbalVout(); break;
        case NEXT_CASE: calibrateI(SCREEN_ICHARGE, AnalogInputs::Ismps, AnalogInputs::IsmpsValue); break;
        case NEXT_CASE: calibrateI(SCREEN_IDISCHARGE, AnalogInputs::Idischarge, AnalogInputs::IdischargeValue); break;
        case NEXT_CASE+3: runInfo(); break;
        default:
                Screen::runNotImplemented(); break;
        }
    } while(i >= 0);
}

void Calibrate::runInfo()
{
    StaticMenu menu(calibrateInfoMenu, sizeOfArray(calibrateInfoMenu));
    int8_t i;
    do {
        i = menu.runSimple();
        if(i >= 0 && i<=7)
            info(screenType(i));

        switch(i) {
        case 8:     infoDis();          break;
        case 9:     infoTimeM();        break;
#ifdef ENABLE_STACK_INFO
        case 10:    infoStackInfo();    break;
#endif
        }
    } while(i>=0);
}



void Calibrate::print_v(uint8_t dig){
    lcdSetCursor0_0();
    switch(dispVal_) {
    case 0: lcdPrint_P(PSTR("v")); break;
    case 1: lcdPrint_P(PSTR("r")); break;
    case 2: lcdPrint_P(PSTR("R")); break;
    }
    dig--;
    lcdPrintUnsigned(value_, dig);
}

void Calibrate::print_d(AnalogInputs::Name name, int dig)
{
    if(dispVal_ == 1) {
        analogInputs.printRealValue(name, dig);
    } else {
        if(name == AnalogInputs::Vb1) name = AnalogInputs::Vb1_real;
        if(name == AnalogInputs::Vb2) name = AnalogInputs::Vb2_real;
        switch(dispVal_) {
        case 0: lcdPrintUnsigned(analogInputs.getValue(name), dig-2);
                 break;
        case 2: analogInputs.printMeasuredValue(name, dig);
                 break;
        }
    }
}

void Calibrate::print_m(const char *str, AnalogInputs::Name name, int dig)
{
    lcdPrint_P(str);
    print_d(name, dig);
}

void Calibrate::print_m_2(const char *str, AnalogInputs::Name name, int dig)
{
    print_m(str, name, dig);
    lcdPrintSpaces();
}
void Calibrate::print_m_1(const char *str, AnalogInputs::Name name, int dig)
{
    lcdSetCursor0_1();
    print_m(str, name, dig);
}
void Calibrate::print_m_3(const char *str, AnalogInputs::Name name, int dig)
{
    print_m_1(str, name , dig);
    lcdPrintSpaces();
}


void Calibrate::printCalibrateIcharge()
{
    print_v();
    print_m_2(PSTR(" Is:"), AnalogInputs::Ismps);
    print_m_3(PSTR("Iv:"),   AnalogInputs::IsmpsValue);
}

void Calibrate::printCalibrateIdischarge()
{
    print_v();
    print_m_2(PSTR(" Id:"), AnalogInputs::Idischarge);
    print_m_3(PSTR("Iv:"),   AnalogInputs::IdischargeValue);
}

void Calibrate::printCalibrateVout()
{
    print_v();
    print_m_2(PSTR(" Vo:"), AnalogInputs::Vout);
    print_m_3(PSTR("VoutMux:"), AnalogInputs::VoutMux);
}

void Calibrate::printCalibrateVreverse()
{
    print_v();
    print_m_2(PSTR(" Vo:"), AnalogInputs::Vout);
    print_m_3(PSTR("Vrev:"), AnalogInputs::VreversePolarity);
}

void Calibrate::printCalibrateVunknown()
{
#ifdef ANALOG_INPUTS_V_UNKNOWN
    print_v();
    print_m_2(PSTR(" V0:"), AnalogInputs::Vunknown0);
    print_m_3(PSTR("V1:"), AnalogInputs::Vunknown1);
#else
    screen.displayNotImplemented();
#endif
}

void Calibrate::printCalibrateVoutVbal()
{
    print_v();
    print_m_2(PSTR(" Vo:"), AnalogInputs::Vout);
    print_m_3(PSTR("Vbal:"), AnalogInputs::Vbalancer);
}

void Calibrate::printCalibrateB1_3()
{
    print_v(7);
    uint8_t dig = 7;
    if(dispVal_ != 0) dig = 6;
    print_m_2(PSTR(" 1:"), AnalogInputs::Vb1_real, dig);
    print_m_1(PSTR("2:"),  AnalogInputs::Vb2_real, dig);
    if(dispVal_ == 0) lcdPrintChar(' ');
    print_m_2(PSTR("3:"), AnalogInputs::Vb3_real, dig);
}
#ifdef ENABLE_B0_CALIBRATION
void Calibrate::printCalibrateB0_Blink()
{
    lcdSetCursor0_0();
    if(!analogInputs.isConnected(AnalogInputs::Vout)) {
        lcdPrint_P(PSTR("ERROR! "));
    } else {
        lcdPrintSpaces(7);
    }
    uint8_t dig = 7;
    if(dispVal_ != 0) dig = 6;

    lcdPrint_P(PSTR(" 0:"));
    if(blink_ != 0 || blinkOn_) print_d(AnalogInputs::Vb0_real, dig);
    else lcdPrintSpaces(dig);

    lcdSetCursor0_1();
    lcdPrint_P(PSTR("Con: 4V to B-,B0"));
}
#endif

void Calibrate::printCalibrateB1_3_Blink()
{
    lcdSetCursor0_0();
    if(!analogInputs.isConnected(AnalogInputs::Vout)) {
        lcdPrint_P(PSTR("ERROR! "));
    } else {
        lcdPrintSpaces(7);
    }
    uint8_t dig = 7;
    if(dispVal_ != 0) dig = 6;

    lcdPrint_P(PSTR(" 1:"));
    if(blink_ != 0 || blinkOn_) print_d(AnalogInputs::Vb1, dig);
    else lcdPrintSpaces(dig);

    lcdSetCursor0_1();
    lcdPrint_P(PSTR("2:"));
    if(blink_ != 1 || blinkOn_) print_d(AnalogInputs::Vb2, dig);
    else lcdPrintSpaces(dig);

    if(dispVal_ == 0) lcdPrintChar(' ');
    lcdPrint_P(PSTR("3:"));
    if(blink_ != 2 || blinkOn_) print_d(AnalogInputs::Vb3_real, dig);
    else lcdPrintSpaces(dig);
}



void Calibrate::printCalibrateB4_6_Blink()
{
    lcdSetCursor0_0();
    if(!analogInputs.isConnected(AnalogInputs::Vout)) {
        lcdPrint_P(PSTR("ERROR! "));
    } else {
        lcdPrintSpaces(7);
    }
    uint8_t dig = 7;
    if(dispVal_ != 0) dig = 6;

    lcdPrint_P(PSTR(" 4:"));
    if(blink_ != 0 || blinkOn_) print_d(AnalogInputs::Vb4_real, dig);
    else lcdPrintSpaces(dig);

    lcdSetCursor0_1();
    lcdPrint_P(PSTR("5:"));
    if(blink_ != 1 || blinkOn_) print_d(AnalogInputs::Vb5_real, dig);
    else lcdPrintSpaces(dig);

    if(dispVal_ == 0) lcdPrintChar(' ');
    lcdPrint_P(PSTR("6:"));
    if(blink_ != 2 || blinkOn_) print_d(AnalogInputs::Vb6_real, dig);
    else lcdPrintSpaces(dig);
}

void Calibrate::printCalibrateB4_6()
{
    print_v(7);
    uint8_t dig = 7;
    if(dispVal_ != 0) dig = 6;
    print_m_2(PSTR(" 4:"), AnalogInputs::Vb4_real, dig);
    print_m_1(PSTR("5:"), AnalogInputs::Vb5_real, dig);
    if(dispVal_ == 0) lcdPrintChar(' ');
    print_m_2(PSTR("6:"), AnalogInputs::Vb6_real, dig);
}

void Calibrate::printCalibrateT()
{
    print_v();
    print_m_2(PSTR(" Ti:"), AnalogInputs::Tintern);
    print_m_3(PSTR("Textern:"), AnalogInputs::Textern);
}
void Calibrate::printCalibrateVin()
{
    print_v();
    lcdPrintSpaces();
    print_m_3(PSTR("Vin:"), AnalogInputs::Vin);
}

void Calibrate::printCalibrate(screenType p) {
    switch(p) {
    case SCREEN_ICHARGE:        printCalibrateIcharge();    break;
    case SCREEN_VOUT:           printCalibrateVout();       break;
    case SCREEN_B1_3:           printCalibrateB1_3();       break;
    case SCREEN_B4_6:           printCalibrateB4_6();       break;
    case SCREEN_T:              printCalibrateT();          break;
    case SCREEN_VIN:            printCalibrateVin();        break;
    case SCREEN_VREVERSE:       printCalibrateVreverse();   break;
    case SCREEN_VUNKNOWN:       printCalibrateVunknown();   break;

    case SCREEN_VOUT_VBAL:      printCalibrateVoutVbal();   break;
#ifdef ENABLE_B0_CALIBRATION
    case SCREEN_B0_BLINK:       printCalibrateB0_Blink();   break;
#endif
    case SCREEN_B1_3_BLINK:     printCalibrateB1_3_Blink(); break;
    case SCREEN_B4_6_BLINK:     printCalibrateB4_6_Blink(); break;
    }
}

void Calibrate::info(screenType p)
{
    calibrate(p);
}

void Calibrate::setBlink(screenType screen)
{
    switch (screen) {
#ifdef ENABLE_B0_CALIBRATION
        case SCREEN_B0_BLINK:          setBalancer(AnalogInputs::Vb0_real); break;
#endif
        case SCREEN_B1_3_BLINK:        setBalancer(AnalogInputs::Vb1_real); break;
        case SCREEN_B4_6_BLINK:        setBalancer(AnalogInputs::Vb4_real); break;
    }
}

bool Calibrate::calibrateBlink(screenType p, int8_t maxBlink)
{
    bool retu = false;
    bool released = false;
    dispVal_ = 1;
    blink_ = 0;
    blinkOn_ = true;
    uint8_t blinkCount = 0;
    smps.powerOn();
    uint8_t key, last_key;
    do {
        printCalibrate(p);
        key = keyboard.getPressedWithSpeed();

        blinkOn_ = (blinkCount%6) != 0;
        blinkCount++;
        if(key == BUTTON_INC && blink_ < maxBlink - 1)      blink_++;
        if(key == BUTTON_DEC && blink_ > 0)                 blink_--;

        if(key == BUTTON_START && released) {
            buzzer.soundSelect();
            setBlink(p);
            released = false;
        }
        if(key == BUTTON_NONE) released = true;
        else blinkCount = 0;

    } while(key != BUTTON_STOP || !released);
    smps.powerOff();
    return retu;
}


bool Calibrate::setValue(uint8_t x, uint8_t y, AnalogInputs::ValueType &v, AnalogInputs::Type type, uint8_t dig)
{
    bool released = false;
    uint8_t key;
    int dir;
    do {
        dir = 0;
        lcdSetCursor(x,y);
        if(blinkOn_)    lcdPrintAnalog(v, type, dig);
        else             lcdPrintSpaces(dig);
        blinkOn_ = !blinkOn_;

        key = keyboard.getPressedWithSpeed();
        if(key == BUTTON_INC) {  dir  = 1; blinkOn_ = true; }
        if(key == BUTTON_DEC) {  dir = -1; blinkOn_ = true; }
        if(key == BUTTON_START && released) {
            return true;
        }
        if(key == BUTTON_NONE) released = true;

        dir *= keyboard.getSpeedFactor();
        v+=dir;
    } while(key != BUTTON_STOP);
    return false;
}


bool Calibrate::calibrate(screenType p)
{
    if(p == SCREEN_IDISCHARGE)
        return calibrateDischarge();

    bool retu = false;
    bool released = false;
    value_ = 0;
    dispVal_ = 1;
    //turn on output
    smps.powerOn();
    uint8_t key, last_key;
    do {
        printCalibrate(p);
        last_key = key;
        key = keyboard.getPressedWithSpeed();
        if(key == BUTTON_INC && value_ < MAX_CALIBRATION_SMPS_VALUE) {
            value_++;
            smps.setValue(value_);
        }
        if(key == BUTTON_DEC && value_ > 0) {
            value_--;
            smps.setValue(value_);
        }
        if(key == BUTTON_START && keyboard.getSpeed() == ACCEPT_DELAY && released) {
            retu = true;
            break;
        }
        if(key == BUTTON_NONE && last_key == BUTTON_START && released) {
            dispVal_ = (dispVal_+1)%3;
        }
        if(key == BUTTON_NONE) released = true;

    } while(key != BUTTON_STOP);
    smps.powerOff();
    return retu;
}

void Calibrate::infoDis()
{
    calibrateDischarge();
}

bool Calibrate::calibrateDischarge()
{
    bool retu = false;
    bool released = false;
    value_ = 0;
    dispVal_ = 1;
    discharger.powerOn();
    uint8_t key, last_key;
    do {
        printCalibrateIdischarge();
        last_key = key;
        key = keyboard.getPressedWithSpeed();
        if(key == BUTTON_INC && value_ < 760*2) {
            value_++;
            discharger.setValue(value_);
        }
        if(key == BUTTON_DEC && value_ > 0) {
            value_--;
            discharger.setValue(value_);
        }
        if(key == BUTTON_START && keyboard.getSpeed() == ACCEPT_DELAY && released) {
            retu = true;
            break;
        } if(key == BUTTON_NONE && last_key == BUTTON_START  && released) {
            dispVal_ = (dispVal_+1)%3;
        }
        if(key == BUTTON_NONE) released = true;
    } while(key != BUTTON_STOP);
    discharger.powerOff();
    return retu;
}

#ifdef ENABLE_STACK_INFO
void Calibrate::infoStackInfo()
{
    lcdClear();
    uint8_t key;
    do {
        key = keyboard.getPressedWithSpeed();
        lcdSetCursor0_0();
        lcdPrint_P(PSTR(" stack: "));
        lcdPrintUnsigned(StackInfo::getFreeStackSize());
        lcdPrintSpaces();
        lcdSetCursor0_1();
        lcdPrint_P(PSTR("unused: "));
        lcdPrintUnsigned(StackInfo::getNeverUsedStackSize());
        lcdPrintSpaces();
    } while(key != BUTTON_STOP);
}
#endif

void Calibrate::infoTimeM()
{
    lcdClear();
    analogInputs.powerOn();
    uint8_t key;
    do {
        key = keyboard.getPressedWithSpeed();
        uint32_t t1,t0;
        t0 = analogInputs.calculationCount_;
        hardware::delay(10000);
        t1 = analogInputs.calculationCount_;
        lcdSetCursor0_0();
        lcdPrint_P(PSTR("measu: "));
        lcdPrintUnsigned(t0);
        lcdPrintSpaces();
        lcdSetCursor0_1();
        lcdPrint_P(PSTR("  10s: "));
        t1-=t0;
        lcdPrintUnsigned(t1);
        lcdPrintSpaces();

    } while(key != BUTTON_STOP);
    analogInputs.powerOff();
}


