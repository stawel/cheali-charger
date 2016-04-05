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

#ifndef SETTINGS_ADC_NOISE_DEFAULT
#define SETTINGS_ADC_NOISE_DEFAULT 0
#endif

DATAMEM struct Settings::Settings settings;

#if defined(ENABLE_EEPROM_RESTORE_DEFAULT) || defined(ENABLE_SETTINGS_MENU_RESET)
const PROGMEM struct Settings::Settings defaultSettings = {
        70,                 //backlight
        Settings::FanTemperature, //fanOn
        ANALOG_CELCIUS(50), //fanTempOn
        ANALOG_CELCIUS(60), //dischargeTempOff

        1,                  //AudioBeep: yes/no
        ANALOG_AMP(0.050),  //minIc
        MAX_CHARGE_I,       //maxIc
        ANALOG_AMP(0.050),  //minId
        MAX_DISCHARGE_I,    //maxId
        ANALOG_VOLT(10.000),//inputVoltageLow

        SETTINGS_ADC_NOISE_DEFAULT, //adcNoise
        Settings::Disabled, //UART - disabled
        3,                   //57600
        Settings::Software, //UARToutput
        Settings::MenuSimple, //menuType
};

void Settings::restoreDefault() {
    pgm_read(settings, &defaultSettings);
    save();
}

#endif


const PROGMEM uint32_t Settings::UARTSpeedValue[SETTINGS_UART_SPEEDS] = {
    9600,
    19200,
    38400,
    57600,
    115200,
};


uint32_t Settings::getUARTspeed() {
    uint32_t x;
    pgm_read(x, &UARTSpeedValue[settings.UARTspeed]);
    return x;
}

void Settings::load() {
    eeprom_read(settings, &eeprom::data.settings);
    apply();
}

void Settings::save() {
    eeprom_write(&eeprom::data.settings, settings);
    eeprom::restoreSettingsCRC();

    apply();
}

void Settings::check() {
    if(settings.maxIc < settings.minIc) {
        settings.maxIc = settings.minIc;
    }
    if(settings.maxIc > MAX_CHARGE_I) {
        settings.maxIc = MAX_CHARGE_I;
    }

    if(settings.maxId < settings.minId) {
        settings.maxId = settings.minId;
    }
    if(settings.maxId > MAX_DISCHARGE_I) {
        settings.maxId = MAX_DISCHARGE_I;
    }
}


void Settings::apply() {
#ifdef ENABLE_LCD_BACKLIGHT
    hardware::setLCDBacklight(settings.backlight);
#endif
}

