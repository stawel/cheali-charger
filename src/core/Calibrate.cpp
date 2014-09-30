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
#include "eeprom.h"

namespace Calibrate {

const char * const calibrateMenu[] PROGMEM = {
        string_voltage,
        string_chargeCurrent,
        string_dischargeCurrent,
        string_externalTemperature,
#ifdef ENABLE_T_INTERNAL
        string_internalTemperature,
#endif
#ifdef ENABLE_EXPERT_VOLTAGE_CALIBRATION
        string_extertsVoltage,
#endif
    NULL
};

const char * const voltageMenu[] PROGMEM = {string_v_menu_input, string_v_menu_cell1, string_v_menu_cell2,
        string_v_menu_cell3, string_v_menu_cell4, string_v_menu_cell5, string_v_menu_cell6,
        BALANCER_PORTS_GT_6( string_v_menu_cell7, string_v_menu_cell8,)
        string_v_menu_cellSum, string_v_menu_output, NULL};

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

const char * const chargeIMenu[]    PROGMEM = { string_ic_menu_current0, string_ic_menu_current1, NULL};
const char * const dischargeIMenu[] PROGMEM = { string_id_menu_current0, string_id_menu_current1, NULL};
const AnalogInputs::ValueType chargeIValues[]     PROGMEM = {CALIBRATION_CHARGE_POINT0_mA,    CALIBRATION_CHARGE_POINT1_mA};
const AnalogInputs::ValueType dischargeIValues[]  PROGMEM = {CALIBRATION_DISCHARGE_POINT0_mA, CALIBRATION_DISCHARGE_POINT1_mA};

const char * const currentMenu[] PROGMEM = {string_i_menu_value,        string_i_menu_output,   NULL};
const char * const pointMenu[]   PROGMEM = {string_tp_menu_point0,      string_tp_menu_point1,  NULL};
const char * const tempMenu[]    PROGMEM = {string_t_menu_temperature,  string_t_menu_value,    NULL};

#ifdef ENABLE_EXPERT_VOLTAGE_CALIBRATION

const char * const expertVoltageMenu[] PROGMEM = {
#ifdef ENABLE_SIMPLIFIED_VB0_VB2_CIRCUIT
        string_ev_menu_cell0pin, string_ev_menu_cell1pin, string_ev_menu_cell2pin,
#endif //ENABLE_SIMPLIFIED_VB0_VB2_CIRCUIT
        string_ev_menu_plusVoltagePin,
        string_ev_menu_minusVoltagePin,
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

/* voltage calibration */


class VoltageMenu: public EditMenu {
public:
    VoltageMenu(const char * const* vMenu,
            const AnalogInputs::Name * vNames, uint8_t dig) :
        EditMenu(vMenu),
        vNames_(vNames),
        dig_(dig){};
    virtual void printItem(uint8_t index) {
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
    //TODO: ??? Vb0_pin
    AnalogInputs::setCalibrationPoint(AnalogInputs::Vb0_pin, 1, p1);

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
    Screen::displayStrings(string_connectBattery);
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
    virtual void printItem(uint8_t index) {
        //TODO: hack, should be improved ... Gyuri: R138 burned.
        if(!AnalogInputs::isConnected(AnalogInputs::Vout)) {
            Screen::displayStrings(string_connectBattery);
            if(cName_ == AnalogInputs::IdischargeValue) {
                Discharger::powerOff();
            }
        } else {
            StaticMenu::printItem(index);
            if(getBlinkIndex() != index) {
                if(index == 0) {
                    lcdPrintUnsigned(value_, 5);
                } else {
                    lcdPrintCurrent(AnalogInputs::getIout(), 7);
                }
            }
        }
    }
    virtual void editItem(uint8_t index, uint8_t key) {
        int dir = -1;
        if(key == BUTTON_INC) dir = 1;
        dir *= Keyboard::getSpeedFactor();
        change0ToMaxSmart(value_, dir, maxValue_, Keyboard::getSpeedFactor(),1);
        setCurrentValue(cName_, value_);
    }
};

void calibrateI(bool charging, uint8_t point, AnalogInputs::ValueType current)
{
    AnalogInputs::ValueType maxValue;
    AnalogInputs::Name name1;
    AnalogInputs::Name name2;

    if(testVout()) {

        if(charging) {
            maxValue = SMPS_UPPERBOUND_VALUE;
            name1 = AnalogInputs::IsmpsValue;
            name2 = AnalogInputs::Ismps;
            SMPS::powerOn();
        } else {
            name1 = AnalogInputs::IdischargeValue;
            name2 = AnalogInputs::Idischarge;
            maxValue = DISCHARGER_UPPERBOUND_VALUE;
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
    virtual void printItem(uint8_t index) {
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
        case NEXT_CASE: calibrateI(chargeIMenu, chargeIValues); break;
        case NEXT_CASE: calibrateI(dischargeIMenu, dischargeIValues); break;
        case NEXT_CASE: calibrateTemp(AnalogInputs::Textern); break;
#ifdef ENABLE_T_INTERNAL
        case NEXT_CASE: calibrateTemp(AnalogInputs::Tintern); break;
#endif
#ifdef ENABLE_EXPERT_VOLTAGE_CALIBRATION
        case NEXT_CASE: expertCalibrateVoltage(); break;
#endif
        }
        eeprom::restoreCalibrationCRC();

        AnalogInputs::powerOff();
        SerialLog::powerOff();

    } while(true);
    Program::programState_ = Program::Done;
}

} // namespace Calibrate

