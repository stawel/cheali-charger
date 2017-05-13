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
#include "Menu.h"

namespace Calibration {


//calibration main menu
const Menu::StaticMenu optionsStaticMenu[] PROGMEM = {
        {string_voltage,                voltageCalibration},
        {string_chargeCurrent,          chargeCurrentCalibration},
        {string_dischargeCurrent,       dischargeCurrentCalibration},
        {string_externalTemperature,    externalTemperatureCalibration},
#ifdef ENABLE_T_INTERNAL
        {string_internalTemperature,    internalTemperatureCalibration},
#endif
#ifdef ENABLE_EXPERT_VOLTAGE_CALIBRATION
        {string_expertVoltage,          expertVoltageCalibration},
#endif
        {NULL, NULL}
};


void run()
{
    //TODO: rewrite
    ProgramData::battery.enable_externT = 0;
    SerialLog::powerOn();

    Menu::runStatic(optionsStaticMenu);

    SerialLog::powerOff();
    Program::programState = Program::Done;
}


bool testVout(bool balancePort)
{
    Screen::displayStrings(string_connect, string_battery);
    bool displayed = false;
    do {
        if(AnalogInputs::isConnected(AnalogInputs::Vout)) {
            if(balancePort == (AnalogInputs::getConnectedBalancePortCellsCount() > 0) )
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
        if(Keyboard::getPressedWithDelay() == BUTTON_STOP)
            return false;
    } while(true);
}


/* calibration check */

#define CHECK_MAGIC_CONST 10

uint8_t check(AnalogInputs::Name name, AnalogInputs::ValueType min, AnalogInputs::ValueType max)
{
    AnalogInputs::ValueType adcMin, adcMax, adcAvr;
    adcMax = AnalogInputs::reverseCalibrateValue(name, max);
    adcMin = AnalogInputs::reverseCalibrateValue(name, min);
    adcAvr = AnalogInputs::reverseCalibrateValue(name, (min+max)/2);
    if(adcMin == 0)             return 1;
    if(adcMin == UINT16_MAX)    return 2;
    if(adcMax == 0)             return 3;
    if(adcMax == UINT16_MAX)    return 4;
    if(adcMax <= adcMin)        return 5;
    if(adcAvr-CHECK_MAGIC_CONST <= adcMin)  return 6;
    if(adcMax <= adcAvr+CHECK_MAGIC_CONST)  return 7;
    return 0;
}

uint8_t checkIcharge() {
    uint8_t error;
    error = check(AnalogInputs::IsmpsSet, settings.minIc, settings.maxIc);
    if(error) return error+10;
    error = check(AnalogInputs::Ismps, settings.minIc, settings.maxIc);
    return error;
}

uint8_t checkIdischarge() {
    uint8_t error;
    error = check(AnalogInputs::IdischargeSet, settings.minId, settings.maxId);
    if(error) return error+10;
    error = check(AnalogInputs::Idischarge, settings.minId, settings.maxId);
    return error;
}

bool checkAll() {
    uint8_t error;
    if(!eeprom::check())
        return false;
    error = check(AnalogInputs::Vout_plus_pin, ANALOG_VOLT(1.000), settings.maxVc);
    if(error) {
        Screen::runCalibrationError(string_voltage, error);
        return false;
    }
    error = checkIcharge();
    if(error) {
        Screen::runCalibrationError(string_chargeCurrent, error);
        return false;
    }

    error = checkIdischarge();
    if(error) {
        Screen::runCalibrationError(string_dischargeCurrent, error);
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
