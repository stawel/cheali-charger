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
#include "Hardware.h"
#include "memory.h"
#include "SettingsMenu.h"
#include "Buzzer.h"
#include "Settings.h"
#include "Monitor.h"
#include "eeprom.h"

#ifndef SETTINGS_EXTERNAL_T_DEFAULT
#define SETTINGS_EXTERNAL_T_DEFAULT 0
#endif

#ifndef SETTINGS_DISCHARGE_AGGRESSIVE_LIXX_DEFAULT
#define SETTINGS_DISCHARGE_AGGRESSIVE_LIXX_DEFAULT 1
#endif

#ifndef SETTINGS_DELTA_V_ENABLE_DEFAULT
#define SETTINGS_DELTA_V_ENABLE_DEFAULT 1
#endif

#ifndef SETTINGS_FORCE_BALANCE_PORT_DEFAULT
#define SETTINGS_FORCE_BALANCE_PORT_DEFAULT 1
#endif

Settings settings;

const Settings defaultSettings PROGMEM = {
        70,                 //backlight_
        ANALOG_CELCIUS(50), //fanTempOn_
        ANALOG_CELCIUS(60), //dischargeTempOff_
        SETTINGS_EXTERNAL_T_DEFAULT, //externT_
        ANALOG_CELCIUS(60), //externTCO_
        ANALOG_CELCIUS(1),  //deltaT_
        SETTINGS_DELTA_V_ENABLE_DEFAULT, //enable_deltaV_
        ANALOG_VOLT(0.005), //deltaV_NiMH_
        ANALOG_VOLT(0.015), //deltaV_NiCd_
        5,                  //DCcycles_
        30,                 //DCRestTime_
        1,                  //AudioBeep_ yes/no
        10,                 //Lixx_Imin.
        120,                //"%" capCutoff_
        ANALOG_VOLT(7),     //inputVoltageLow_
        ANALOG_VOLT(0),     //dischargeOffset_LiXX_
        SETTINGS_DISCHARGE_AGGRESSIVE_LIXX_DEFAULT,     //dischargeAggressive_LiXX_
        SETTINGS_FORCE_BALANCE_PORT_DEFAULT,            //forceBalancePort_
        ANALOG_VOLT(0.008), //balancerError_
        Settings::Disabled, //UART_ - disabled
        3,                   //57600
        Settings::Software, //UARTinput_

        0,                   //calibratedState_
        0,                   //SMPS_Upperbound_Value_
        0                    //DISCHARGER_Upperbound_Value_
};


const uint32_t Settings_speeds[Settings::UARTSpeeds] PROGMEM = {
    9600,
    19200,
    38400,
    57600,
    115200,
};


uint32_t Settings::getUARTspeed() const {
    return pgm::read(&Settings_speeds[UARTspeed_]);
}

void Settings::load() {
    eeprom::read(settings, &eeprom::data.settings);
    settings.apply();
}

void Settings::save() {
    eeprom::write(&eeprom::data.settings, settings);
    eeprom::restoreSettingsCRC();

    settings.apply();
}

void Settings::setDefault()
{
    pgm::read(*this, &defaultSettings);
}
void Settings::restoreDefault() {
    settings.setDefault();
    Settings::save();
}

void Settings::check() {
    if(DCcycles_>5) DCcycles_ = 5;
}

void Settings::apply() {
#ifdef ENABLE_LCD_BACKLIGHT
    hardware::setLCDBacklight(backlight_);
#endif
    Monitor::update();
    hardware::setExternalTemperatueOutput(externT_);
}


void Settings::edit()
{
    SettingsMenu menu(*this);
    menu.run();
    *this = menu.p_;
    save();
    apply();
}

