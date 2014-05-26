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
#include "Hardware.h"
#include "Settings.h"
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

const char cvin[]   PROGMEM = "Vin: ";
const char cvb1[]   PROGMEM = "Vb1: ";
const char cvb2[]   PROGMEM = "Vb2: ";
const char cvb3[]   PROGMEM = "Vb3: ";
const char cvb4[]   PROGMEM = "Vb4: ";
const char cvb5[]   PROGMEM = "Vb5: ";
const char cvb6[]   PROGMEM = "Vb6: ";
#if MAX_BANANCE_CELLS > 6
const char cvb7[]   PROGMEM = "Vb7: ";
const char cvb8[]   PROGMEM = "Vb8: ";
const char cvbsum[] PROGMEM = "V1-8:";
#else
const char cvbsum[] PROGMEM = "V1-6:";
#endif
const char cvo[]    PROGMEM = "Vout:";

const char * const voltageMenu[] PROGMEM = {cvin, cvb1, cvb2, cvb3, cvb4, cvb5, cvb6,
        BALANCER_PORTS_GT_6(cvb7, cvb8,)
        cvbsum, cvo, NULL};

const AnalogInputs::Name voltageName[] PROGMEM = {
       AnalogInputs::Vin,
       AnalogInputs::Vb1,
       AnalogInputs::Vb2,
       AnalogInputs::Vb3,
       AnalogInputs::Vb4,
       AnalogInputs::Vb5,
       AnalogInputs::Vb6,
       BALANCER_PORTS_GT_6(AnalogInputs::Vb7, AnalogInputs::Vb8,)
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
       BALANCER_PORTS_GT_6(AnalogInputs::Vb7_pin, AnalogInputs::Vb8_pin)
};



const char cIC0[] PROGMEM = CHEALI_CHARGER_STRING(CALIBRATION_CHARGE_POINT0_mA)    "mA";
const char cIC1[] PROGMEM = CHEALI_CHARGER_STRING(CALIBRATION_CHARGE_POINT1_mA)    "mA";
const char cID0[] PROGMEM = CHEALI_CHARGER_STRING(CALIBRATION_DISCHARGE_POINT0_mA) "mA";
const char cID1[] PROGMEM = CHEALI_CHARGER_STRING(CALIBRATION_DISCHARGE_POINT1_mA) "mA";

const char * const chargeIMenu[]    PROGMEM = { cIC0, cIC1, NULL};
const char * const dischargeIMenu[] PROGMEM = { cID0, cID1, NULL};
const AnalogInputs::ValueType chargeIValues[]     PROGMEM = {CALIBRATION_CHARGE_POINT0_mA,    CALIBRATION_CHARGE_POINT1_mA};
const AnalogInputs::ValueType dischargeIValues[]  PROGMEM = {CALIBRATION_DISCHARGE_POINT0_mA, CALIBRATION_DISCHARGE_POINT1_mA};

const char cc1[] PROGMEM = "value: ";
const char cc2[] PROGMEM = "DATA:  ";

const char * const currentMenu[] PROGMEM = {cc1,cc2, NULL};



const char cp1[] PROGMEM = "point 1.";
const char cp2[] PROGMEM = "point 2.";

const char * const pointMenu[] PROGMEM = {cp1,cp2, NULL};

const char ct1[] PROGMEM = "temp: ";
const char ct2[] PROGMEM = "value:";

const char * const tempMenu[] PROGMEM = {ct1,ct2, NULL};



#ifdef ENABLE_EXPERT_VOLTAGE_CALIBRATION

#ifdef ENABLE_SIMPLIFIED_VB0_VB2_CIRCUIT
const char cev0[] PROGMEM = "Vb0pin:";
const char cev1[] PROGMEM = "Vb1pin:";
const char cev2[] PROGMEM = "Vb2pin:";
#endif //ENABLE_SIMPLIFIED_VB0_VB2_CIRCUIT

const char cevp[] PROGMEM = "Vplus: ";
const char cevm[] PROGMEM = "Vminus:";

const char * const expertVoltageMenu[] PROGMEM = {
#ifdef ENABLE_SIMPLIFIED_VB0_VB2_CIRCUIT
        cev0,cev1,cev2,
#endif //ENABLE_SIMPLIFIED_VB0_VB2_CIRCUIT
        cevp,
        cevm,
        NULL};

const AnalogInputs::Name expertVoltageName[] PROGMEM = {
#ifdef ENABLE_SIMPLIFIED_VB0_VB2_CIRCUIT
        AnalogInputs::Vb0_pin,
        AnalogInputs::Vb1_pin,
        AnalogInputs::Vb2_pin,
#endif //ENABLE_SIMPLIFIED_VB0_VB2_CIRCUIT
        AnalogInputs::Vout_plus_pin,
        AnalogInputs::Vout_minus_pin
};

#endif //ENABLE_EXPERT_VOLTAGE_CALIBRATION


uint16_t x1=0;  //lastGood calibrationcurrent.




/* voltage calibration */


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
    p.x = AnalogInputs::getAvrADCValue(AnalogInputs::Vout_plus_pin);
    p.y = AnalogInputs::getRealValue(AnalogInputs::Vbalancer);
    AnalogInputs::setCalibrationPoint(AnalogInputs::Vout_plus_pin, 1, p);
    //info: we assume that Vout_plus_pin and Vout_minus_pin have the same voltage dividers
    AnalogInputs::setCalibrationPoint(AnalogInputs::Vout_minus_pin, 1, p);
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

void saveVoltage(bool doCopyVbalVout, AnalogInputs::Name name1,  AnalogInputs::Name name2)
{
    Buzzer::soundSelect();
    AnalogInputs::ValueType newValue = AnalogInputs::getRealValue(name1);
    saveVoltage(name1, name2, AnalogInputs::getAvrADCValue(name2), newValue);
    AnalogInputs::on_ = true;
    AnalogInputs::doFullMeasurement();
    if(doCopyVbalVout)
        copyVbalVout();
}

void calibrateVoltage()
{
    if(testVout()) {
        VoltageMenu v(voltageMenu, voltageName, 9);
        int8_t index;
        do {
            index = v.runSimple(true);
            if(index < 0) break;
            AnalogInputs::Name Vinput = pgm::read(&voltageName[index]);
            if(index <= MAX_BANANCE_CELLS && AnalogInputs::isConnected(Vinput)) {
                AnalogInputs::doFullMeasurement();
                AnalogInputs::on_ = false;
                if(v.runEdit(index))
                    saveVoltage(true, Vinput,pgm::read(&voltageName2[index]));
                    //save calib status
                    settings.calibratedState_ |= 1;  Settings::save();
                AnalogInputs::on_ = true;
            }
        } while(true);
    }
}


/* expert voltage calibration */


#ifdef ENABLE_EXPERT_VOLTAGE_CALIBRATION

void expertCalibrateVoltage()
{
    PolarityCheck::checkReversedPolarity_ = false;
    //TODO: optimization: this method should be merged with calibrateVoltage
    VoltageMenu v(expertVoltageMenu, expertVoltageName, 7);
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
            saveVoltage(false, Vinput, Vinput);
        AnalogInputs::on_ = true;
    } while(true);

    PolarityCheck::checkReversedPolarity_ = true;
}
#endif


/* current calibration */


void setCurrentValue(AnalogInputs::Name name, AnalogInputs::ValueType value)
{
    if(name == AnalogInputs::IsmpsValue)    SMPS::setValue(value);
    else                                    Discharger::setValue(value);
}

class CurrentMenu: public EditMenu {
public:
    AnalogInputs::Name cName_;
    uint8_t point_;
    AnalogInputs::ValueType value_;
    AnalogInputs::ValueType maxValue_;

    CurrentMenu(AnalogInputs::Name name, uint8_t point, AnalogInputs::ValueType maxValue)
            : EditMenu(currentMenu), cName_(name), point_(point), maxValue_(maxValue) {}
    void refreshValue() {
        AnalogInputs::CalibrationPoint p;
        AnalogInputs::getCalibrationPoint(p, cName_, point_);
        value_ = p.x;
    };
    virtual uint8_t printItem(uint8_t index) {
        //TODO: hack, should be improved ... Gyuri: R138 burned.
        if(!AnalogInputs::isConnected(AnalogInputs::Vout)) {
            Screen::displayStrings(PSTR(" Connect Battery"),NULL);
            if(cName_ == AnalogInputs::IdischargeValue) {
                Discharger::powerOff();
            }
        } else {
            StaticMenu::printItem(index);
            if(getBlinkIndex() != index) {
                if(index == 0) {
                    lcdPrintUnsigned(value_, 5);
                } else {
                    lcdPrintUnsigned(AnalogInputs::getIout(), 5);
                }
            }
        }
    }
    virtual void editItem(uint8_t index, uint8_t key) {
        int dir = -1;
        if(key == BUTTON_INC) dir = 1;
        dir *= Keyboard::getSpeedFactor();
        change0ToMaxSmart(value_, dir, maxValue_, Keyboard::getSpeedFactor());
        setCurrentValue(cName_, value_);
    }
};

void calibrateI(bool charging, uint8_t point, AnalogInputs::ValueType current)
{
    AnalogInputs::ValueType maxValue = 65535 ;
    AnalogInputs::Name name1;
    AnalogInputs::Name name2;

    if(testVout()) {
        if(charging) {
            name1 = AnalogInputs::IsmpsValue;
            name2 = AnalogInputs::Ismps;
            settings.calibratedState_   &= ~(1 << 1);  Settings::save();  //relase calibrationlimit
            SMPS::powerOn();
        } else {
            name1 = AnalogInputs::IdischargeValue;
            name2 = AnalogInputs::Idischarge;
            settings.calibratedState_   &= ~(1 << 2);  Settings::save();  //relase calibrationlimit
            Discharger::powerOn();
        }

        CurrentMenu menu(name1, point, maxValue);
        int8_t index;
        do {
            menu.refreshValue();
            index = menu.runSimple(true);
            if(index < 0) break;
            if(index == 0) {
                setCurrentValue(name1, menu.value_);
                if(menu.runEdit(index)) {
                    AnalogInputs::doFullMeasurement();
                    AnalogInputs::CalibrationPoint p;
                    p.y = current;
                    p.x = menu.value_;
                    AnalogInputs::setCalibrationPoint(name1, point, p);
                    p.x = AnalogInputs::getAvrADCValue(name2);
                    AnalogInputs::setCalibrationPoint(name2, point, p);
                }
                setCurrentValue(name1, 0);
            }
        } while(true);

        if(charging)   SMPS::powerOff();
        else           Discharger::powerOff();
    }
}

void calibrateI(const char * const textMenu[], const AnalogInputs::ValueType  values[])
{
    StaticMenu menu(textMenu);
    int8_t i;
    AnalogInputs::ValueType current;
    do {
        i = menu.runSimple();
        if(i<0) break;
        current = pgm::read(&values[i]);
        calibrateI(textMenu == chargeIMenu, i, current);
    } while(true);
}


/* temperature calibration */


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


/* calibration menu*/


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
        case NEXT_CASE: calibrateI(chargeIMenu, chargeIValues);  checkCalibrateIcharge(); break;
        case NEXT_CASE: calibrateI(dischargeIMenu, dischargeIValues); checkCalibrateIdischarge(); break;
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




/* Check calibrate and set .SMPS_Upperbound_Value_ */




void checkCalibrateIcharge()
{
    //CHECKHARDWAREPIDVALIDCALIBRATE
    //check 'overflow" ismpsvalue
    
    //check 'overflow" ismps (protect hardware PID ctrl chargers)
        //if r53-54 failure, then calibration value too high. Protect the SMPS circiuit the overflow value.

    if (!checkCalibrate(MAX_CHARGE_I,AnalogInputs::IsmpsValue))
     {
       Screen::displayCalibrationErrorScreen(1);
     }

    settings.SMPS_Upperbound_Value_ = x1;
    //save calib status
    settings.calibratedState_ = settings.calibratedState_ | 2; 
    Settings::save();   


    //info only
    if (!checkCalibrate(MAX_CHARGE_I,AnalogInputs::Ismps))
     {
       Screen::displayCalibrationErrorScreen(2);
     }
}



void checkCalibrateIdischarge()
{
    if (!checkCalibrate(MAX_DISCHARGE_I,AnalogInputs::IdischargeValue))
     {
       Screen::displayCalibrationErrorScreen(3);
     }
   
   settings.DISCHARGER_Upperbound_Value_ = x1;
   //save calib status
   settings.calibratedState_ = settings.calibratedState_  | 4;
   Settings::save(); 
   
   //info only
    if (!checkCalibrate(MAX_DISCHARGE_I,AnalogInputs::Idischarge))
     {
       Screen::displayCalibrationErrorScreen(4);
     }
}



bool checkCalibrate(AnalogInputs::ValueType testCurrent, AnalogInputs::Name name1)
{   
    uint16_t x2=0;
    bool r=true;
    for(uint16_t i=0; i < testCurrent; i=i++)
    {
        x1 = AnalogInputs::reverseCalibrateValue(name1, i);
        if (x1 < x2)  {r=false; break;}
        x2=x1;
    }
     if(x1==0) r=false;

    x1=x2;    //x1 return maxcharge/discharge value before overflow
    return r;
}




















} // namespace Calibrate

