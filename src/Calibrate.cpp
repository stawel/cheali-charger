#include "Options.h"
#include "LcdPrint.h"
#include "StaticMenu.h"
#include "SMPS.h"
#include "Calibrate.h"
#include "Utils.h"
#include "Screen.h"
#include "Buzzer.h"

const char string_c0[] PROGMEM = "B0-2";
const char string_c1[] PROGMEM = "B3-5";
const char string_c2[] PROGMEM = "B0-5 to Vout";
const char string_c3[] PROGMEM = "Icharge";
const char string_c4[] PROGMEM = "Idischarge";
const char string_c5[] PROGMEM = "Vin";
const char string_c6[] PROGMEM = "Tintern";
const char string_c7[] PROGMEM = "Textern";
const char string_c8[] PROGMEM = "Info";


const char string_ci0[] PROGMEM = "Icharge";
const char string_ci1[] PROGMEM = "Vout";
const char string_ci2[] PROGMEM = "B0-2";
const char string_ci3[] PROGMEM = "B3-5";
const char string_ci4[] PROGMEM = "Temp";
const char string_ci5[] PROGMEM = "Vin";
const char string_ci6[] PROGMEM = "Vreversed";
const char string_ci7[] PROGMEM = "Vunknown";
const char string_ci8[] PROGMEM = "Idischarge";
const char string_ci9[] PROGMEM = "time";


const char * const calibrateMenu[] PROGMEM =
{ string_c0,
  string_c1,
  string_c2,
  string_c3,
  string_c4,
  string_c5,
  string_c6,
  string_c7,
  string_c8,
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
};


const char string_cI0[] PROGMEM = "50mA";
const char string_cI1[] PROGMEM = "1000mA";


const char * const calibrateIMenu[] PROGMEM =
{
    string_cI0,
    string_cI1,
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
    StaticMenu menu(calibrateIMenu, sizeOfArray(calibrateIMenu));
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
                p.y = ANALOG_AMP(1);
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
    if(calibrate(SCREEN_VOUT_VBAL)) {
        p.x = analogInputs.getValue(AnalogInputs::Vout);
        p.y = analogInputs.getRealValue(AnalogInputs::Vbalacer);
        setCalibrationPoint(AnalogInputs::Vout, 1, p);

        p.x = analogInputs.getValue(AnalogInputs::VoutMux);
        setCalibrationPoint(AnalogInputs::VoutMux, 1, p);
    }
}

void Calibrate::setBalancer(AnalogInputs::Name firstName)
{
    AnalogInputs::Name name = AnalogInputs::Name(firstName + blink_);
    uint8_t x,y;
    switch(blink_) {
    case 0:    x = 10; y = 0; break;
    case 1: x = 2;  y = 1; break;
    case 2: x = 10; y = 1; break;
    }
    AnalogInputs::CalibrationPoint p;
    p.y = analogInputs.getRealValue(name);
    if(setValue(x, y, p.y, AnalogInputs::Voltage, 6)) {
        p.x = analogInputs.getValue(name);
        setCalibrationPoint(name, 1, p);
    }
}

void Calibrate::run()
{
    StaticMenu menu(calibrateMenu, sizeOfArray(calibrateMenu));
    int8_t i;
    do {
        i = menu.runSimple();
        if(i<0) break;
        switch(i) {
        case 0: calibrateBlink(SCREEN_B0_2_BLINK, 3); break;
        case 1: calibrateBlink(SCREEN_B3_5_BLINK, 3); break;
        case 2: copyVbalVout(); break;
        case 3: calibrateI(SCREEN_ICHARGE, AnalogInputs::Ismps, AnalogInputs::IsmpsValue); break;
        case 4: calibrateI(SCREEN_IDISCHARGE, AnalogInputs::Idischarge, AnalogInputs::IdischargeValue); break;
        case 8: runInfo(); break;
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
        case 8: infoDis(); break;
        case 9: infoTimeM(); break;
        }
    } while(i>=0);
}



void Calibrate::print_v(uint8_t dig){
    lcdSetCursor0_0();
    switch(dispVal_) {
    case 0:    lcdPrint_P(PSTR("v:")); break;
    case 1: lcdPrint_P(PSTR("r:")); break;
    case 2: lcdPrint_P(PSTR("R:")); break;
    }
    lcdPrintUnsigned(value_, dig);
}

void Calibrate::print_d(AnalogInputs::Name name, int dig)
{
    switch(dispVal_) {
    case 0:    lcdPrintUnsigned(analogInputs.getValue(name), dig-2);
            break;
    case 1: analogInputs.printRealValue(name, dig);
            break;
    case 2: analogInputs.printMeasuredValue(name, dig);
            break;
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
    print_m_3(PSTR("Vbal:"), AnalogInputs::Vbalacer);
}

void Calibrate::printCalibrateB0_2()
{
    print_v(5);
    uint8_t dig = 7;
    if(dispVal_ != 0) dig = 6;
    print_m_2(PSTR(" 0:"), AnalogInputs::Vb0, dig);
    print_m_1(PSTR("1:"),  AnalogInputs::Vb1, dig);
    if(dispVal_ == 0) lcdPrintChar(' ');
    print_m_2(PSTR("2:"), AnalogInputs::Vb2, dig);
}
void Calibrate::printCalibrateB0_2_Blink()
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
    if(blink_ != 0 || blinkOn_) print_d(AnalogInputs::Vb0, dig);
    else lcdPrintSpaces(dig);

    lcdSetCursor0_1();
    lcdPrint_P(PSTR("1:"));
    if(blink_ != 1 || blinkOn_) print_d(AnalogInputs::Vb1, dig);
    else lcdPrintSpaces(dig);

    if(dispVal_ == 0) lcdPrintChar(' ');
    lcdPrint_P(PSTR("2:"));
    if(blink_ != 2 || blinkOn_) print_d(AnalogInputs::Vb2, dig);
    else lcdPrintSpaces(dig);
}

void Calibrate::printCalibrateB3_5_Blink()
{
    lcdSetCursor0_0();
    if(!analogInputs.isConnected(AnalogInputs::Vout)) {
        lcdPrint_P(PSTR("ERROR! "));
    } else {
        lcdPrintSpaces(7);
    }
    uint8_t dig = 7;
    if(dispVal_ != 0) dig = 6;

    lcdPrint_P(PSTR(" 3:"));
    if(blink_ != 0 || blinkOn_) print_d(AnalogInputs::Vb3, dig);
    else lcdPrintSpaces(dig);

    lcdSetCursor0_1();
    lcdPrint_P(PSTR("4:"));
    if(blink_ != 1 || blinkOn_) print_d(AnalogInputs::Vb4, dig);
    else lcdPrintSpaces(dig);

    if(dispVal_ == 0) lcdPrintChar(' ');
    lcdPrint_P(PSTR("5:"));
    if(blink_ != 2 || blinkOn_) print_d(AnalogInputs::Vb5, dig);
    else lcdPrintSpaces(dig);
}

void Calibrate::printCalibrateB3_5()
{
    print_v(5);
    uint8_t dig = 7;
    if(dispVal_ != 0) dig = 6;
    print_m_2(PSTR(" 3:"), AnalogInputs::Vb3, dig);
    print_m_1(PSTR("4:"), AnalogInputs::Vb4, dig);
    if(dispVal_ == 0) lcdPrintChar(' ');
    print_m_2(PSTR("5:"), AnalogInputs::Vb5, dig);
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
    case SCREEN_ICHARGE:         printCalibrateIcharge();     break;
    case SCREEN_VOUT:        printCalibrateVout();     break;
    case SCREEN_B0_2:        printCalibrateB0_2();     break;
    case SCREEN_B3_5:         printCalibrateB3_5();     break;
    case SCREEN_T:             printCalibrateT();         break;
    case SCREEN_VIN:         printCalibrateVin();     break;
    case SCREEN_VREVERSE:     printCalibrateVreverse();     break;
    case SCREEN_VUNKNOWN:     printCalibrateVunknown();     break;

    case SCREEN_VOUT_VBAL:     printCalibrateVoutVbal();     break;
    case SCREEN_B0_2_BLINK:    printCalibrateB0_2_Blink(); break;
    case SCREEN_B3_5_BLINK:    printCalibrateB3_5_Blink(); break;
    }
}

void Calibrate::info(screenType p)
{
    calibrate(p);
}

void Calibrate::setBlink(screenType screen)
{
    switch (screen) {
        case SCREEN_B0_2_BLINK:        setBalancer(AnalogInputs::Vb0); break;
        case SCREEN_B3_5_BLINK:        setBalancer(AnalogInputs::Vb3); break;
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
        if(key == BUTTON_INC && blink_ < maxBlink - 1)    blink_++;
        if(key == BUTTON_DEC && blink_ > 0)                blink_--;

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
        if(key == BUTTON_INC && value_ < 760) {
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


void Calibrate::infoTimeM()
{
    lcdClear();
    uint8_t key;
    do {
        key = keyboard.getPressedWithSpeed();
        uint32_t t1,t0;
        t0 = timer.getMiliseconds();
        hardware::delay(100);
        t1 = timer.getMiliseconds();
        lcdSetCursor0_0();
        lcdPrint_P(PSTR(" time: "));
        lcdPrintUnsigned(t0);
        lcdPrintSpaces();
        lcdSetCursor0_1();
        lcdPrint_P(PSTR("100ms: "));
        t1-=t0;
        lcdPrintUnsigned(t1);
        lcdPrintSpaces();

    } while(key != BUTTON_STOP);
}


