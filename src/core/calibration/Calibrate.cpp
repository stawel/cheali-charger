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
#include "Balancer.h"
#include "StaticEditMenu.h"

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

uint16_t calibrationPoint;

const char * const chargeIMenu[]    PROGMEM = { string_ic_menu_current0, string_ic_menu_current1, NULL};
const char * const dischargeIMenu[] PROGMEM = { string_id_menu_current0, string_id_menu_current1, NULL};
const AnalogInputs::ValueType chargeIValues[]     PROGMEM = {CALIBRATION_CHARGE_POINT0_mA,    CALIBRATION_CHARGE_POINT1_mA};
const AnalogInputs::ValueType dischargeIValues[]  PROGMEM = {CALIBRATION_DISCHARGE_POINT0_mA, CALIBRATION_DISCHARGE_POINT1_mA};

const char * const currentMenu[] PROGMEM = {string_i_menu_value,        string_i_menu_output,   NULL};

/* voltage calibration */


void copyVbalVout()
{
    //info: we assume that Vout_plus_pin and Vout_minus_pin have identically voltage dividers
    AnalogInputs::CalibrationPoint p;
    p.x = AnalogInputs::getAvrADCValue(AnalogInputs::Vout_plus_pin);
    p.x -= AnalogInputs::getAvrADCValue(AnalogInputs::Vout_minus_pin);
    p.y = AnalogInputs::getRealValue(AnalogInputs::Vbalancer);
    AnalogInputs::setCalibrationPoint(AnalogInputs::Vout_plus_pin, calibrationPoint, p);
    AnalogInputs::setCalibrationPoint(AnalogInputs::Vout_minus_pin, calibrationPoint, p);
}

#ifdef ENABLE_SIMPLIFIED_VB0_VB2_CIRCUIT
void calibrateSimplifiedVb1_pin(AnalogInputs::ValueType real_v)
{
    AnalogInputs::CalibrationPoint p1,p2;
    p1.x = AnalogInputs::getAvrADCValue(AnalogInputs::Vb1_pin);
    p1.y = real_v + AnalogInputs::getRealValue(AnalogInputs::Vb0_pin);
    p2.x = AnalogInputs::getAvrADCValue(AnalogInputs::Vb2_pin);
    p2.y = p1.y + AnalogInputs::getRealValue(AnalogInputs::Vb2);

    //info: we assume that Vb0_pin and Vb1_pin have identically voltage dividers
    AnalogInputs::setCalibrationPoint(AnalogInputs::Vb0_pin, calibrationPoint, p1);
    AnalogInputs::setCalibrationPoint(AnalogInputs::Vb1_pin, calibrationPoint, p1);

    if(AnalogInputs::isConnected(AnalogInputs::Vb2)) {
        AnalogInputs::setCalibrationPoint(AnalogInputs::Vb2_pin, calibrationPoint, p2);
    }
}
void calibrateSimplifiedVb2_pin(AnalogInputs::ValueType real_v)
{
    AnalogInputs::CalibrationPoint p2;
    p2.x = AnalogInputs::getAvrADCValue(AnalogInputs::Vb2_pin);
    p2.y = real_v + AnalogInputs::getRealValue(AnalogInputs::Vb1_pin);
    AnalogInputs::setCalibrationPoint(AnalogInputs::Vb2_pin, calibrationPoint, p2);
}

#endif



bool testVout(bool balancePort)
{
    Screen::displayStrings(string_connect, string_battery);
    bool displayed = false;
    do {
        if(AnalogInputs::isConnected(AnalogInputs::Vout)) {
            if(AnalogInputs::isConnected(AnalogInputs::Vb1) == balancePort)
                return true;
            if(!displayed) {
                if(balancePort) {
                    Screen::displayStrings(string_connect, string_balancePort);
                } else {
                    Screen::displayStrings(string_disconnect, string_balancePort);
                }
            }
            displayed = true;
        }
        if(Keyboard::getPressedWithSpeed() == BUTTON_STOP)
            return false;
    }while(true);
}

void saveCalibration(AnalogInputs::Name name1, AnalogInputs::Name name2, AnalogInputs::ValueType adc, AnalogInputs::ValueType newValue)
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
            AnalogInputs::setCalibrationPoint(name2, calibrationPoint, p);
#else
        AnalogInputs::setCalibrationPoint(name2, calibrationPoint, p);
#endif
}

void saveCalibration(bool doCopyVbalVout, AnalogInputs::Name name1,  AnalogInputs::Name name2)
{
    Buzzer::soundSelect();
    AnalogInputs::ValueType newValue = AnalogInputs::getRealValue(name1);
    SerialLog::flush();
    saveCalibration(name1, name2, AnalogInputs::getAvrADCValue(name2), newValue);
    if(doCopyVbalVout) {
	    AnalogInputs::on_ = true;
	    AnalogInputs::doFullMeasurement();
        SerialLog::flush();
        copyVbalVout();
    }
    eeprom::restoreCalibrationCRC();
}

#define COND_ALWAYS StaticEditMenu::StaticEditMenu::Always
#define COND_COPY_VOUT      128
#define COND_POINT          4
#define COND_E_ANALOG       2
#define COND_E_C_ANALOG     COND_E_ANALOG + COND_COPY_VOUT
#define EANALOG_V(name) {CP_TYPE_V, 0, &AnalogInputs::real_[AnalogInputs::name]}

void runCalibrationMenu(const StaticEditMenu::StaticEditData * menuData,
        const AnalogInputs::Name * name1,
        const AnalogInputs::Name * name2) {
    StaticEditMenu menu(menuData);
    menu.setSelector(COND_ALWAYS);
    int8_t item;
    do {
        item = menu.runSimple(true);
        if(item < 0) break;
        uint8_t c = menu.getEnableCondition(item);
        if(!(c & 1)) {
            if(c & COND_E_ANALOG) {
                AnalogInputs::Name Vinput = pgm::read(&name1[item]);
                if(AnalogInputs::isConnected(Vinput)) {
                    AnalogInputs::doFullMeasurement();
                    AnalogInputs::on_ = false;
                    if(menu.runEdit()) {
                        bool doCopyVout = c & COND_COPY_VOUT;
                        saveCalibration(doCopyVout, Vinput, pgm::read(&name2[item]));
                    }
                    AnalogInputs::on_ = true;
                }
            } else {
                menu.runEdit();
            }
        }
    } while(true);
}

const AnalogInputs::Name voltageName[] PROGMEM = {
       AnalogInputs::Vin,
       AnalogInputs::Vb1,
       AnalogInputs::Vb2,
       AnalogInputs::Vb3,
       AnalogInputs::Vb4,
       AnalogInputs::Vb5,
       AnalogInputs::Vb6,
       BALANCER_PORTS_GT_6(AnalogInputs::Vb7, AnalogInputs::Vb8,)
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

const StaticEditMenu::StaticEditData editVoltageData[] PROGMEM = {
{string_v_menu_input,       COND_E_C_ANALOG,    EANALOG_V(Vin),         {1, 0, ANALOG_VOLT(30)}},
{string_v_menu_cell1,       COND_E_C_ANALOG,    EANALOG_V(Vb1),         {1, 0, ANALOG_VOLT(5)}},
{string_v_menu_cell2,       COND_E_C_ANALOG,    EANALOG_V(Vb2),         {1, 0, ANALOG_VOLT(5)}},
{string_v_menu_cell3,       COND_E_C_ANALOG,    EANALOG_V(Vb3),         {1, 0, ANALOG_VOLT(5)}},
{string_v_menu_cell4,       COND_E_C_ANALOG,    EANALOG_V(Vb4),         {1, 0, ANALOG_VOLT(5)}},
{string_v_menu_cell5,       COND_E_C_ANALOG,    EANALOG_V(Vb5),         {1, 0, ANALOG_VOLT(5)}},
{string_v_menu_cell6,       COND_E_C_ANALOG,    EANALOG_V(Vb6),         {1, 0, ANALOG_VOLT(5)}},
BALANCER_PORTS_GT_6(
{string_v_menu_cell7,       COND_E_C_ANALOG,    EANALOG_V(Vb7),         {1, 0, ANALOG_VOLT(5)}},
{string_v_menu_cell8,       COND_E_C_ANALOG,    EANALOG_V(Vb8),         {1, 0, ANALOG_VOLT(5)}},
)
{string_v_menu_cellSum,     COND_ALWAYS,        EANALOG_V(Vbalancer),   {0, 0, 0}},
{string_v_menu_output,      COND_ALWAYS,        EANALOG_V(Vout),        {0, 0, 0}},
{string_menu_point,         COND_POINT,         {CP_TYPE_UNSIGNED, 0, &calibrationPoint},        {1, 0, 1}},
{NULL,                      StaticEditMenu::Last}
};

void calibrateVoltage()
{
    calibrationPoint = 1;
    AnalogInputs::powerOn();
    if(testVout(true)) {
        runCalibrationMenu(editVoltageData, voltageName, voltageName2);
    }
    AnalogInputs::powerOff();
}


/* expert voltage calibration */


#ifdef ENABLE_EXPERT_VOLTAGE_CALIBRATION

const StaticEditMenu::StaticEditData editExpertVoltageData[] PROGMEM = {
#ifdef ENABLE_SIMPLIFIED_VB0_VB2_CIRCUIT
{string_ev_menu_cell0pin,           COND_E_ANALOG,   EANALOG_V(Vb0_pin),         {1, 0, ANALOG_VOLT(10)}},
{string_ev_menu_cell1pin,           COND_E_ANALOG,   EANALOG_V(Vb1_pin),         {1, 0, ANALOG_VOLT(10)}},
{string_ev_menu_cell2pin,           COND_E_ANALOG,   EANALOG_V(Vb2_pin),         {1, 0, ANALOG_VOLT(10)}},
#endif //ENABLE_SIMPLIFIED_VB0_VB2_CIRCUIT
{string_ev_menu_plusVoltagePin,     COND_E_ANALOG,   EANALOG_V(Vout_plus_pin),   {1, 0, MAX_CHARGE_V}},
{string_ev_menu_minusVoltagePin,    COND_E_ANALOG,   EANALOG_V(Vout_minus_pin),  {1, 0, MAX_CHARGE_V}},
{string_menu_point,                 COND_POINT,     {CP_TYPE_UNSIGNED, 0, &calibrationPoint},        {1, 0, 1}},
{NULL,                      StaticEditMenu::Last}
};


const AnalogInputs::Name expertVoltageName[] PROGMEM = {
#ifdef ENABLE_SIMPLIFIED_VB0_VB2_CIRCUIT
        AnalogInputs::Vb0_pin,
        AnalogInputs::Vb1_pin,
        AnalogInputs::Vb2_pin,
#endif //ENABLE_SIMPLIFIED_VB0_VB2_CIRCUIT
        AnalogInputs::Vout_plus_pin,
        AnalogInputs::Vout_minus_pin
};



void expertCalibrateVoltage()
{
    calibrationPoint = 1;
    AnalogInputs::powerOn(false);
    PolarityCheck::checkReversedPolarity_ = false;
    runCalibrationMenu(editExpertVoltageData, expertVoltageName, expertVoltageName);
    PolarityCheck::checkReversedPolarity_ = true;
    AnalogInputs::powerOff();

}
#endif //ENABLE_EXPERT_VOLTAGE_CALIBRATION


/* current calibration */


void setCurrentValue(AnalogInputs::Name name, AnalogInputs::ValueType value)
{
    if(name == AnalogInputs::IsmpsSet)      SMPS::setValue(value);
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
    void refreshValue(AnalogInputs::CalibrationPoint &p) {
        value_ = p.x;
    };
    virtual void printItem(uint8_t index) {
        //TODO: hack, should be improved ... Gyuri: R138 burned.
        if(!AnalogInputs::isConnected(AnalogInputs::Vout)) {
            Screen::displayStrings(string_connect, string_battery);
            if(cName_ == AnalogInputs::IdischargeSet) {
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
        changeMinToMaxStep(&value_, dir, 1, maxValue_, 1);
        setCurrentValue(cName_, value_);
    }
};

void calibrateI(bool charging, uint8_t point, AnalogInputs::ValueType current)
{
    AnalogInputs::ValueType maxValue;
    AnalogInputs::Name name1;
    AnalogInputs::Name name2;
    AnalogInputs::CalibrationPoint pName1;
    AnalogInputs::CalibrationPoint pName2;

    AnalogInputs::powerOn();
    if(testVout(false)) {

        if(charging) {
            maxValue = SMPS_UPPERBOUND_VALUE;
            name1 = AnalogInputs::IsmpsSet;
            name2 = AnalogInputs::Ismps;
            SMPS::powerOn();
            hardware::setVoutCutoff(MAX_CHARGE_V);

        } else {
            name1 = AnalogInputs::IdischargeSet;
            name2 = AnalogInputs::Idischarge;
            maxValue = DISCHARGER_UPPERBOUND_VALUE;
            Discharger::powerOn();
        }

        getCalibrationPoint(pName1, name1, point);
        getCalibrationPoint(pName2, name2, point);

        CurrentMenu menu(name1, point, maxValue);
        int8_t index;
        do {
            menu.refreshValue(pName1);
            index = menu.runSimple(true);
            if(index < 0) break;
            if(index == 0) {
                setCurrentValue(name1, menu.value_);
                if(menu.runEdit()) {
                    AnalogInputs::doFullMeasurement();
                    pName1.y = current;
                    pName1.x = menu.value_;
                    pName2.y = current;
                    pName2.x = AnalogInputs::getAvrADCValue(name2);
                }
                setCurrentValue(name1, 0);
            }
        } while(true);

        if(charging)   SMPS::powerOff();
        else           Discharger::powerOff();

        //Info: we save eeprom data only when no current is flowing
        AnalogInputs::setCalibrationPoint(name1, point, pName1);
        AnalogInputs::setCalibrationPoint(name2, point, pName2);
        eeprom::restoreCalibrationCRC();
    }
    AnalogInputs::powerOff();
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

#define EANALOG_T(name) {CP_TYPE_TEMPERATURE, 0, &AnalogInputs::real_[AnalogInputs::name]}
#define EANALOG_ADC(name) {CP_TYPE_UNSIGNED, 0, &AnalogInputs::avrAdc_[AnalogInputs::name]}

const StaticEditMenu::StaticEditData editExternTData[] PROGMEM = {
{string_t_menu_temperature,     COND_E_ANALOG,  EANALOG_T(Textern),             {1, 0, ANALOG_CELCIUS(100)}},
{string_t_menu_adc,             COND_ALWAYS,    EANALOG_ADC(Textern),           {0,0,0}},
{string_menu_point,             COND_POINT,     {CP_TYPE_UNSIGNED, 0, &calibrationPoint},        {1, 0, 1}},
{NULL,                      StaticEditMenu::Last}
};

const StaticEditMenu::StaticEditData editInternTData[] PROGMEM = {
{string_t_menu_temperature,     COND_E_ANALOG,  EANALOG_T(Tintern),             {1, 0, ANALOG_CELCIUS(100)}},
{string_t_menu_adc,             COND_ALWAYS,    EANALOG_ADC(Tintern),           {0,0,0}},
{string_menu_point,             COND_POINT,     {CP_TYPE_UNSIGNED, 0, &calibrationPoint},        {1, 0, 1}},
{NULL,                      StaticEditMenu::Last}
};

const AnalogInputs::Name externTName[] PROGMEM = { AnalogInputs::Textern };
const AnalogInputs::Name internTName[] PROGMEM = { AnalogInputs::Tintern };


void calibrateExternT()
{
    calibrationPoint = 0;
    AnalogInputs::powerOn(false);
    runCalibrationMenu(editExternTData, externTName, externTName);
    AnalogInputs::powerOff();
}

void calibrateInternT()
{
    calibrationPoint = 0;
    AnalogInputs::powerOn(false);
    runCalibrationMenu(editInternTData, internTName, internTName);
    AnalogInputs::powerOff();
}

/* calibration menu */


void run()
{
    StaticMenu menu(calibrateMenu);
    int8_t i;
    do {
        i = menu.runSimple();
        if(i<0) break;
        SerialLog::powerOn();
        START_CASE_COUNTER;
        switch(i) {
        case NEXT_CASE: calibrateVoltage(); break;
        case NEXT_CASE: calibrateI(chargeIMenu, chargeIValues); break;
        case NEXT_CASE: calibrateI(dischargeIMenu, dischargeIValues); break;
        case NEXT_CASE: calibrateExternT(); break;
#ifdef ENABLE_T_INTERNAL
        case NEXT_CASE: calibrateInternT(); break;
#endif
#ifdef ENABLE_EXPERT_VOLTAGE_CALIBRATION
        case NEXT_CASE: expertCalibrateVoltage(); break;
#endif
        }
        SerialLog::powerOff();

    } while(true);
    Program::programState = Program::Done;
}

/* calibration check */

bool checkMax(AnalogInputs::ValueType maxVal, uint16_t maxAdc, AnalogInputs::Name name)
{
    AnalogInputs::ValueType t = AnalogInputs::calibrateValue(name, maxAdc);
    if(t < maxVal+maxVal/2
        && maxVal-maxVal/2 < t)
        return true;
    return false;
}

bool checkVout()
{
    // rev point: MAX_CHARGE_V
    AnalogInputs::ValueType adcMax;
    adcMax = AnalogInputs::reverseCalibrateValue(AnalogInputs::Vout_plus_pin, ANALOG_VOLT(1.000));
    adcMax *= MAX_CHARGE_V/ANALOG_VOLT(1.000);
    return checkMax(MAX_CHARGE_V, adcMax, AnalogInputs::Vout_plus_pin);
}

bool checkIcharge(AnalogInputs::Name name)
{
    // rev point: MAX_CHARGE_I
    AnalogInputs::ValueType adcMax;
    adcMax = AnalogInputs::reverseCalibrateValue(name, CALIBRATION_CHARGE_POINT1_mA);
    adcMax *= MAX_CHARGE_I/CALIBRATION_CHARGE_POINT1_mA;
    return checkMax(MAX_CHARGE_I, adcMax, name);
}

bool checkIdischarge(AnalogInputs::Name name)
{
    // rev point: CALIBRATION_DISCHARGE_POINT1_mA
    // we don't use MAX_DISCHARGE_I because on some chargers (imaxB6-clone) it's very off.
    AnalogInputs::ValueType adcMax;
    adcMax = AnalogInputs::reverseCalibrateValue(name, CALIBRATION_DISCHARGE_POINT0_mA);
    adcMax *= CALIBRATION_DISCHARGE_POINT1_mA/CALIBRATION_DISCHARGE_POINT0_mA;
    return checkMax(CALIBRATION_DISCHARGE_POINT1_mA, adcMax, name);
}

bool checkAll() {
    if(!eeprom::check())
        return false;
    if(!checkVout()) {
        Screen::runCalibrationError(string_voltage);
        return false;
    }
    if(!checkIcharge(AnalogInputs::IsmpsSet) || !checkIcharge(AnalogInputs::Ismps)) {
        Screen::runCalibrationError(string_chargeCurrent);
        return false;
    }
    if(!checkIdischarge(AnalogInputs::IdischargeSet) || !checkIdischarge(AnalogInputs::Idischarge)) {
        Screen::runCalibrationError(string_dischargeCurrent);
        return false;
    }
    return true;
}

#ifdef ENABLE_CALIBRATION_CHECK
bool check() {
    return checkAll();
}
#else
bool check() {
    return true;
}
#endif


} // namespace Calibrate
#undef COND_ALWAYS
