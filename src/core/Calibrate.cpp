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
#include "EditMenu.h"
#include "SerialLog.h"
#include "Program.h"
#include "AnalogInputsPrivate.h"

namespace Calibrate {

const char c1[] PROGMEM = "voltage";
const char c2[] PROGMEM = "I charge";
const char c3[] PROGMEM = "I discharge";
const char c4[] PROGMEM = "temp extern";
#ifdef ENABLE_T_INTERNAL
const char c5[] PROGMEM = "temp intern";
#endif
#ifdef ENABLE_SERIAL_LOG
const char c6[] PROGMEM = "calib->UART";
#endif
#ifdef ENABLE_EXPERT_VOLTAGE_CALIBRATION
const char c7[] PROGMEM = "expert DANGER!";
#endif

const char * const calibrateMenu[] PROGMEM = {c1,c2,c3,c4
#ifdef ENABLE_T_INTERNAL
  ,c5
#endif
#ifdef ENABLE_SERIAL_LOG
  ,c6
#endif
#ifdef ENABLE_EXPERT_VOLTAGE_CALIBRATION
  ,c7
#endif
  ,NULL
};

const char cv1[] PROGMEM = "Vin: ";
const char cv2[] PROGMEM = "Vb1: ";
const char cv3[] PROGMEM = "Vb2: ";
const char cv4[] PROGMEM = "Vb3: ";
const char cv5[] PROGMEM = "Vb4: ";
const char cv6[] PROGMEM = "Vb5: ";
const char cv7[] PROGMEM = "Vb6: ";
const char cv8[] PROGMEM = "V1-6:";
const char cv9[] PROGMEM = "Vout:";

const char * const voltageMenu[] PROGMEM = {cv1, cv2,cv3,cv4,cv5,cv6,cv7,cv8, cv9, NULL};

const AnalogInputs::Name voltageName[] PROGMEM = {
       AnalogInputs::Vin,
       AnalogInputs::Vb1,
       AnalogInputs::Vb2,
       AnalogInputs::Vb3,
       AnalogInputs::Vb4,
       AnalogInputs::Vb5,
       AnalogInputs::Vb6,
       AnalogInputs::Vbalancer,
       AnalogInputs::Vout,
};
const AnalogInputs::Name voltageName2[] PROGMEM = {
       AnalogInputs::Vin,
       AnalogInputs::Vb1_pin,
       AnalogInputs::Vb2_pin,
       AnalogInputs::Vb3_pin,
       AnalogInputs::Vb4_pin,
       AnalogInputs::Vb5_pin,
       AnalogInputs::Vb6_pin,
};

const char cI0[] PROGMEM = "50mA";
const char cI1[] PROGMEM = "1000mA";
const char cID1[] PROGMEM = "300mA";

const char * const chargeIMenu[] PROGMEM = { cI0, cI1, NULL};
const char * const dischargeIMenu[] PROGMEM = { cI0, cID1, NULL};

const char cp1[] PROGMEM = "point 1.";
const char cp2[] PROGMEM = "point 2.";

const char * const pointMenu[] PROGMEM = {cp1,cp2, NULL};

const char ct1[] PROGMEM = "temp: ";
const char ct2[] PROGMEM = "value:";

const char * const tempMenu[] PROGMEM = {ct1,ct2, NULL};


class VoltageMenu: public EditMenu {
public:
    VoltageMenu(const char * const* vMenu,
            const AnalogInputs::Name * vNames, uint8_t dig) :
        EditMenu(vMenu),
        vNames_(vNames),
        dig_(dig){};
    virtual uint8_t printItem(uint8_t index) {
        StaticMenu::printItem(index);
        if(getBlinkIndex() != index) {
            AnalogInputs::Name name = pgm::read(&vNames_[index]);
            AnalogInputs::printRealValue(name, dig_);
        }
    }
    virtual void editItem(uint8_t index, uint8_t key) {
        int dir = -1;
        if(key == BUTTON_INC) dir = 1;
        dir *= Keyboard::getSpeedFactor();
        AnalogInputs::Name name = pgm::read(&vNames_[index]);
        AnalogInputs::real_[name] += dir;
    }
    const AnalogInputs::Name * vNames_;
    const uint8_t dig_;
};


void copyVbalVout()
{
    AnalogInputs::CalibrationPoint p;
    p.x = AnalogInputs::getAvrADCValue(AnalogInputs::Vout);
    p.y = AnalogInputs::getRealValue(AnalogInputs::Vbalancer);
    AnalogInputs::setCalibrationPoint(AnalogInputs::Vout, 1, p);
}

#ifdef ENABLE_SIMPLIFIED_VB0_VB2_CIRCUIT
void calibrateSimplifiedVb1_pin(AnalogInputs::ValueType real_v)
{
    AnalogInputs::CalibrationPoint p1,p2;
    p1.x = AnalogInputs::getAvrADCValue(AnalogInputs::Vb1_pin);
    p1.y = real_v + AnalogInputs::getRealValue(AnalogInputs::Vb0_pin);
    p2.x = AnalogInputs::getAvrADCValue(AnalogInputs::Vb2_pin);
    p2.y = p1.y + AnalogInputs::getRealValue(AnalogInputs::Vb2);
    AnalogInputs::setCalibrationPoint(AnalogInputs::Vb1_pin, 1, p1);
    AnalogInputs::setCalibrationPoint(AnalogInputs::Vb2_pin, 1, p2);
}
void calibrateSimplifiedVb2_pin(AnalogInputs::ValueType real_v)
{
    AnalogInputs::CalibrationPoint p2;
    p2.x = AnalogInputs::getAvrADCValue(AnalogInputs::Vb2_pin);
    p2.y = real_v + AnalogInputs::getRealValue(AnalogInputs::Vb1_pin);
    AnalogInputs::setCalibrationPoint(AnalogInputs::Vb2_pin, 1, p2);
}

#endif



bool testVout()
{
    return true;
    Screen::displayStrings(PSTR("Connect Battery!"),NULL);
    do {
        if(AnalogInputs::isConnected(AnalogInputs::Vout))
            return true;
        if(Keyboard::getPressedWithSpeed() == BUTTON_STOP)
            return false;
    }while(true);
}

void saveVoltage(AnalogInputs::Name name1, AnalogInputs::Name name2, AnalogInputs::ValueType adc, AnalogInputs::ValueType newValue)
{
    AnalogInputs::CalibrationPoint p;
    p.x = adc;
    p.y = newValue;

#ifdef ENABLE_SIMPLIFIED_VB0_VB2_CIRCUIT
        if(name1 == AnalogInputs::Vb1)
            calibrateSimplifiedVb1_pin(p.y);
        else if(name1 == AnalogInputs::Vb2)
            calibrateSimplifiedVb2_pin(p.y);
        else
            AnalogInputs::setCalibrationPoint(name2, 1, p);
#else
        AnalogInputs::setCalibrationPoint(name2, 1, p);
#endif
}

void saveVoltage(int8_t index, bool copyVbal2Vout, AnalogInputs::Name name1,  AnalogInputs::Name name2)
{
    Buzzer::soundSelect();
    AnalogInputs::ValueType newValue = AnalogInputs::getRealValue(name1);
    AnalogInputs::on_ = true;
    AnalogInputs::doFullMeasurement();
    saveVoltage(name1, name2, AnalogInputs::getAvrADCValue(name2), newValue);
    AnalogInputs::doFullMeasurement();
    if(copyVbal2Vout)
        copyVbalVout();
}


#ifdef ENABLE_EXPERT_VOLTAGE_CALIBRATION

const char cev0[] PROGMEM = "Vb0pin: ";
const char cev1[] PROGMEM = "Vb1pin: ";
const char cev2[] PROGMEM = "Vb2pin: ";

const char * const expertVoltageMenu[] PROGMEM = {cev0, cev1,cev2, NULL};

/* TODO: implement?
const char string_cv10[] PROGMEM = "Vreversed";
const char string_cv11[] PROGMEM = "Vunknown";
*/

const AnalogInputs::Name expertVoltageName[] PROGMEM = {
        AnalogInputs::Vb0_pin,
        AnalogInputs::Vb1_pin,
        AnalogInputs::Vb2_pin,
};
const AnalogInputs::Name expertVoltageName2[] PROGMEM = {
        AnalogInputs::Vb0_pin,
        AnalogInputs::Vb1_pin,
        AnalogInputs::Vb2_pin,
};


void expertCalibrateVoltage()
{
    Discharger::powerOn();

    //TODO: optimization: this method should be merged with calibrateVoltage
    VoltageMenu v(expertVoltageMenu, expertVoltageName, 6);
    int8_t index;
    do {
        index = v.runSimple(true);
        if(index < 0) break;
        AnalogInputs::Name Vinput = pgm::read(&expertVoltageName[index]);
        if(!AnalogInputs::isConnected(Vinput))
            continue;
        AnalogInputs::doFullMeasurement();
        AnalogInputs::on_ = false;
        if(v.runEdit(index))
            saveVoltage(index, false, Vinput, pgm::read(&expertVoltageName2[index]));
        AnalogInputs::on_ = true;
    } while(true);
    Discharger::powerOff();
}
#endif


void calibrateVoltage()
{
    Discharger::powerOn();

    if(testVout()) {
        VoltageMenu v(voltageMenu, voltageName, 9);
        int8_t index;
        do {
            index = v.runSimple(true);
            if(index < 0) break;
            AnalogInputs::Name Vinput = pgm::read(&voltageName[index]);
            if(index < 7 && AnalogInputs::isConnected(Vinput)) {
                AnalogInputs::doFullMeasurement();
                AnalogInputs::on_ = false;
                if(v.runEdit(index))
                    saveVoltage(index, true, Vinput,pgm::read(&voltageName2[index]));
                AnalogInputs::on_ = true;
            }
        } while(true);
    }
    Discharger::powerOff();
}

enum calibrateType {CCharger, CDischarger};
AnalogInputs::ValueType value_;

void printCalibrate()
{
    lcdSetCursor0_0();
    lcdPrint_P(PSTR("value: "));
    lcdPrintUnsigned(value_, 5);
    lcdPrintSpaces();
    lcdSetCursor0_1();
    lcdPrint_P(PSTR("Iout:  "));
    AnalogInputs::printRealValue(AnalogInputs::Iout, 7);
    lcdPrintSpaces();
}

bool calibrateI(calibrateType p)
{
    bool retu = false;
    bool released = false;
    value_ = 0;
    AnalogInputs::ValueType maxValue;

    if(p == CCharger){
        maxValue = MAX_CALIBRATION_SMPS_VALUE;
        SMPS::powerOn();
    } else {
        maxValue = MAX_CALIBRATION_DISCHARGER_VALUE;
        Discharger::powerOn();
    }

    if(testVout()) {
        uint8_t key, val_changed;
        do {
            printCalibrate();
            key = Keyboard::getPressedWithSpeed();
            val_changed = 0;
            uint16_t speed = CALIBRATION_SPEED;
#if CALIBRATION_SPEED > 1
            speed *= Keyboard::getSpeedFactor();
#endif
            if(key == BUTTON_INC && value_ < maxValue) {
                value_ += speed;
                val_changed++;
            }
            if(key == BUTTON_DEC && value_ >= speed) {
                value_ -= speed;
                val_changed++;
            }
            if(val_changed) {
                if(p == CCharger)   SMPS::setValue(value_);
                else                Discharger::setValue(value_);
            }

            if(key == BUTTON_START && released) {
                retu = true;
                break;
            }

            if(key == BUTTON_NONE) released = true;
        } while(key != BUTTON_STOP);
    }
    if(p == CCharger)   SMPS::powerOff();
    else                Discharger::powerOff();
    return retu;
}


void calibrateI(calibrateType t, uint8_t point, AnalogInputs::ValueType current, AnalogInputs::Name name1, AnalogInputs::Name name2)
{
    AnalogInputs::CalibrationPoint p;
    if(calibrateI(t)) {
        Buzzer::soundSelect();
        p.y = current;
        p.x = AnalogInputs::getAvrADCValue(name1);
        AnalogInputs::setCalibrationPoint(name1, point, p);
        p.x = AnalogInputs::getAvrADCValue(name2);
        AnalogInputs::setCalibrationPoint(name2, point, p);
    }
}


void calibrateIcharge()
{
    StaticMenu menu(chargeIMenu);
    int8_t i;
    AnalogInputs::ValueType current;
    do {
        i = menu.runSimple();
        if(i<0) break;
        if(i==0) current = ANALOG_AMP(0.050);
        else     current = ANALOG_AMP(1.000);
        calibrateI(CCharger, i, current, AnalogInputs::Ismps, AnalogInputs::IsmpsValue);
    } while(true);
}

void calibrateIdischarge()
{
    StaticMenu menu(dischargeIMenu);
    int8_t i;
    AnalogInputs::ValueType current;
    do {
        i = menu.runSimple();
        if(i<0) break;
        if(i==0) current = ANALOG_AMP(0.050);
        else     current = ANALOG_AMP(0.300);
        calibrateI(CDischarger, i, current, AnalogInputs::Idischarge, AnalogInputs::IdischargeValue);
    } while(true);
}


class TempMenu: public EditMenu {
public:
    AnalogInputs::Name tName_;
    TempMenu(AnalogInputs::Name name) : EditMenu(tempMenu), tName_(name){};
    virtual uint8_t printItem(uint8_t index) {
        StaticMenu::printItem(index);
        if(getBlinkIndex() != index) {
            if(index == 0) {
                AnalogInputs::printRealValue(tName_, 7);
            } else {
                lcdPrintUnsigned(AnalogInputs::getAvrADCValue(tName_), 7);
            }
        }
    }
    virtual void editItem(uint8_t index, uint8_t key) {
        int dir = -1;
        if(key == BUTTON_INC) dir = 1;
        dir *= Keyboard::getSpeedFactor();
        AnalogInputs::real_[tName_] += dir;
    }
};



void saveTemp(AnalogInputs::Name name, uint8_t point)
{
    AnalogInputs::CalibrationPoint p;
    p.x = AnalogInputs::getAvrADCValue(name);
    p.y = AnalogInputs::getRealValue(name);
    AnalogInputs::setCalibrationPoint(name, point, p);
}


void calibrateTemp(AnalogInputs::Name name, uint8_t point)
{
    TempMenu v(name);
    int8_t index;
    do {
        index = v.runSimple(true);
        if(index < 0) break;
        if(index == 0) {
            AnalogInputs::on_ = false;
            if(v.runEdit(index))
                saveTemp(name, point);
            AnalogInputs::on_ = true;
        }
    } while(true);
}

void calibrateTemp(AnalogInputs::Name name)
{
    StaticMenu menu(pointMenu);
    int8_t i;
    do {
        i = menu.runSimple();
        if(i<0) break;
        calibrateTemp(name, i);
    } while(true);
}


void run()
{
    Program::programState_ = Program::Calibration;
    StaticMenu menu(calibrateMenu);
    int8_t i;
    do {
        i = menu.runSimple();
        if(i<0) break;
        SerialLog::powerOn();
        AnalogInputs::powerOn();
        START_CASE_COUNTER;
        switch(i) {
        case NEXT_CASE: calibrateVoltage(); break;
        case NEXT_CASE: calibrateIcharge(); break;
        case NEXT_CASE: calibrateIdischarge(); break;
        case NEXT_CASE: calibrateTemp(AnalogInputs::Textern); break;
#ifdef ENABLE_T_INTERNAL
        case NEXT_CASE: calibrateTemp(AnalogInputs::Tintern); break;
#endif
#ifdef ENABLE_SERIAL_LOG
        case NEXT_CASE: SerialLog::sendCalibration(); break;
#endif
#ifdef ENABLE_EXPERT_VOLTAGE_CALIBRATION
        case NEXT_CASE: expertCalibrateVoltage(); break;
#endif
        }
        AnalogInputs::powerOff();
        SerialLog::powerOff();

    } while(true);
    Program::programState_ = Program::Done;
}

} // namespace Calibrate

