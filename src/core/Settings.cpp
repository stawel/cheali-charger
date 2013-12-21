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

#ifndef DEFAULT_SETTINGS_EXTERNAL_T
#define DEFAULT_SETTINGS_EXTERNAL_T 1
#endif

Settings settings;

const Settings defaultSettings PROGMEM = {
        70,                 //backlight_
        ANALOG_CELCIUS(50), //fanTempOn_
        ANALOG_CELCIUS(60), //dischargeTempOff_

        DEFAULT_SETTINGS_EXTERNAL_T, //externT_
        ANALOG_CELCIUS(60), //externTCO_
        ANALOG_CELCIUS(1),  //deltaT_
        ANALOG_VOLT(0.007), //deltaV_NiMH_
        ANALOG_VOLT(0.012), //deltaV_NiCd_
        5,                  //CDcycles_
        30,                 //WasteTime_
        0,                  //AudioBeep_ yes/no
        120,                //"%" capCutoff_
        ANALOG_VOLT(7),     //inputVoltageLow_
        ANALOG_VOLT(0),     //dischargeOffset_LiXX_
        ANALOG_VOLT(0.008), //balancerError_
        Settings::Disabled, //UART_ - disabled
        0,                   //9600
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
    if(CDcycles_>5) CDcycles_ = 5;
}

void Settings::apply() {
#ifdef ENABLE_LCD_BACKLIGHT
    hardware::setLCDBacklight(backlight_);
#endif
    Monitor::update();
}


void Settings::edit()
{
    SettingsMenu menu(*this);
    menu.run();
    *this = menu.p_;
    save();
    apply();
}

