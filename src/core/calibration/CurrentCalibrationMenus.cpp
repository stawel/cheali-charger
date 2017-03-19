/*
    cheali-charger - open source firmware for a variety of LiPo chargers
    Copyright (C) 2016  Paweł Stawicki. All right reserved.

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
#define __STDC_LIMIT_MACROS
#include <stdint.h>

#include "LcdPrint.h"
#include "SMPS.h"
#include "Calibration.h"
#include "Utils.h"
#include "Screen.h"
#include "Buzzer.h"
#include "StackInfo.h"
#include "EditMenu.h"
#include "SerialLog.h"
#include "Program.h"
#include "AnalogInputsPrivate.h"
#include "Hardware.h"
#include "eeprom.h"
#include "Balancer.h"
#include "memory.h"
#include "EditMenu.h"

namespace Calibration {

static AnalogInputs::Name       gNameSet_;
static AnalogInputs::Name       gName_;
static AnalogInputs::ValueType  gIexpected_;
static AnalogInputs::ValueType  gValue_;


#define EGLOBAL_U(name)     {CP_TYPE_UNSIGNED,      0, &name}
#define EGLOBAL_A(name)     {CP_TYPE_A,             0, &name}

static void printCurrent(int8_t) {
    lcdPrintCurrent(AnalogInputs::getIout(), 7);
    lcdPrintUnsigned(AnalogInputs::getAvrADCValue(gName_), 6);
}

const EditMenu::StaticEditData currentData[] PROGMEM = {
{string_i_menu_value,       EDIT_MENU_ALWAYS,   EGLOBAL_U(gValue_),             {CE_STEP_TYPE_KEY_SPEED, 0, 65535}},
{string_i_menu_output,      EDIT_MENU_ALWAYS,   CPRINTF_METHOD(printCurrent),   {0, 0, 0}},
{string_i_menu_expected,    EDIT_MENU_ALWAYS,   EGLOBAL_A(gIexpected_),         {CE_STEP_TYPE_KEY_SPEED, 0, 65535}},
{NULL,                      EDIT_MENU_LAST}
};

// current calibration menu
static void setCurrentValue(AnalogInputs::ValueType value);

static void editCallback(uint16_t * value) {
    //make sure we don't go too high
    if(gName_ == AnalogInputs::Ismps) {
        if(gValue_ > SMPS_UPPERBOUND_VALUE)         gValue_ = SMPS_UPPERBOUND_VALUE;
        if(gIexpected_ > MAX_CHARGE_I)              gIexpected_ = MAX_CHARGE_I;
    } else {
        if(gValue_ > DISCHARGER_UPPERBOUND_VALUE)   gValue_ = DISCHARGER_UPPERBOUND_VALUE;
        if(gIexpected_ > MAX_DISCHARGE_I)           gIexpected_ = MAX_DISCHARGE_I;
    }
    //set current
    if(value == &gValue_) {
        setCurrentValue(gValue_);
    }
    //TODO: hack, should be improved ... Gyuri: R138 burned.
    if(!AnalogInputs::isConnected(AnalogInputs::Vout)) {
        Screen::displayStrings(string_connect, string_battery);
        if(gName_ == AnalogInputs::Idischarge) {
            Discharger::powerOff();
        }
    }
}

static void setCurrentValue(AnalogInputs::ValueType value)
{
    if(gName_ == AnalogInputs::Ismps)      SMPS::setValue(value);
    else                                   Discharger::setValue(value);
}

static void powerOnCurrent()
{
    if(gName_ == AnalogInputs::Ismps) {
        SMPS::powerOn();
        hardware::setVoutCutoff(MAX_CHARGE_V);
    } else {
        Discharger::powerOn();
    }
}

static void powerOffCurrent()
{
    if(gName_ == AnalogInputs::Ismps)   SMPS::powerOff();
    else                                Discharger::powerOff();
}

static void currentCalibration(uint8_t point)
{
    AnalogInputs::CalibrationPoint pSet;
    AnalogInputs::CalibrationPoint p;
    bool save = false;

    Program::dischargeOutputCapacitor();
    AnalogInputs::powerOn();
    if(testVout(false)) {

        getCalibrationPoint(pSet, gNameSet_, point);
        getCalibrationPoint(p, gName_, point);

        EditMenu::initialize(currentData, editCallback);
        gIexpected_ = pSet.y;
        int8_t index;
        do {
            gValue_ = pSet.x;
            index = EditMenu::run(true);
            if(index < 0) break;
            else if(index == 0) {
                powerOnCurrent();
                setCurrentValue(gValue_);
                if(EditMenu::runEdit()) {
                    AnalogInputs::doFullMeasurement();
                    pSet.y = gIexpected_;
                    pSet.x = gValue_;
                    p.y = gIexpected_;
                    p.x = AnalogInputs::getAvrADCValue(gName_);
                    save = true;
                }
                setCurrentValue(0);
                powerOffCurrent();
            } else if(index == 2 && !EditMenu::runEdit()) {
                gIexpected_ = pSet.y;
            }
        } while(true);

        //Info: we save eeprom data only when no current is flowing
        if(save) {
            AnalogInputs::setCalibrationPoint(gNameSet_, point, pSet);
            AnalogInputs::setCalibrationPoint(gName_, point, p);
            eeprom::restoreCalibrationCRC();
        }
    }
    AnalogInputs::powerOff();
}


static void printCurrentPointItem(uint8_t index) {
    AnalogInputs::CalibrationPoint pSet;
    AnalogInputs::getCalibrationPoint(pSet, gName_, index);
    lcdPrintCurrent(pSet.y, 7);
}

static void currentCalibrationPointMenu()
{
    int8_t index = 0;
    do {
        Menu::initialize(2);
        Menu::printMethod_ = printCurrentPointItem;
        Menu::setIndex(index);
        index = Menu::run();
        if(index >= 0) {
            currentCalibration(index);
        }
    } while (index >= 0);
}

void chargeCurrentCalibration()
{
    gNameSet_ = AnalogInputs::IsmpsSet;
    gName_ = AnalogInputs::Ismps;
    currentCalibrationPointMenu();
}

void dischargeCurrentCalibration()
{
    gNameSet_ = AnalogInputs::IdischargeSet;
    gName_ = AnalogInputs::Idischarge;
    currentCalibrationPointMenu();
}


/* current calibration */

/*
void setCurrentValue(AnalogInputs::Name name, AnalogInputs::ValueType value)
{
    if(name == AnalogInputs::IsmpsSet)      SMPS::setValue(value);
    else                                    Discharger::setValue(value);
}

class CurrentMenu: public EditMenu {
public:
    AnalogInputs::Name cNameSet_;
    AnalogInputs::Name cName_;
    uint8_t point_;
    AnalogInputs::ValueType value_;
    AnalogInputs::ValueType maxValue_;
    AnalogInputs::ValueType Iexpected_;
    AnalogInputs::ValueType maxIexpected_;


    CurrentMenu(AnalogInputs::Name nameSet, AnalogInputs::Name name, uint8_t point, AnalogInputs::ValueType maxValue, AnalogInputs::ValueType maxI)
            : EditMenu(currentMenu), cNameSet_(nameSet), cName_(name), point_(point), maxValue_(maxValue), maxIexpected_(maxI) {}

    void resetValue(AnalogInputs::CalibrationPoint &p) {
        value_ = p.x;
    };
    void resetIexpected(AnalogInputs::CalibrationPoint &p) {
        Iexpected_ = p.y;
    };

    virtual void printItem(uint8_t index) {
        //TODO: hack, should be improved ... Gyuri: R138 burned.
        if(!AnalogInputs::isConnected(AnalogInputs::Vout)) {
            Screen::displayStrings(string_connect, string_battery);
            if(cNameSet_ == AnalogInputs::IdischargeSet) {
                Discharger::powerOff();
            }
        } else {
            StaticMenu::printItem(index);
            if(Blink::getBlinkIndex() != index) {
                switch (index) {
                    case 0:
                        lcdPrintUnsigned(value_, 9);
                        break;
                    case 1:
                        lcdPrintCurrent(AnalogInputs::getIout(), 7);
                        lcdPrintUnsigned(AnalogInputs::getAvrADCValue(cName_), 6);
                        break;
                    default:
                        lcdPrintCurrent(Iexpected_, 8);
                        break;
                }
            }
        }
    }
    virtual void editItem(uint8_t index, uint8_t key) {
        int dir = -1;
        uint8_t steps = Keyboard::getSpeedFactor();
        if(key == BUTTON_INC) dir = 1;
        if(index == 0) {
            changeMinToMaxStep(&value_, dir, 1, maxValue_, steps);
            setCurrentValue(cNameSet_, value_);
        } else {
            changeMinToMaxStep(&Iexpected_, dir, 1, maxIexpected_, steps);
        }
    }
};


*/


} // namespace Calibration
