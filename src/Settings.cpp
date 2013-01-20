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
#include "Settings.h"
#include "memory.h"
#include "SettingsMenu.h"
#include "Hardware.h"
#include "Buzzer.h"

Settings settings;

uint16_t backlight_;
AnalogInputs::ValueType fanTempOn_;
AnalogInputs::ValueType dischargeTempOff_;
bool externT_;
AnalogInputs::ValueType externTCO_;
AnalogInputs::ValueType deltaT_;
AnalogInputs::ValueType deltaV_NiMH_, deltaV_NiCd_;
uint8_t CDcycles_;
uint16_t capCutoff_;
AnalogInputs::ValueType inputVoltageLow_;
Screen::ScreenViewType view_;


Settings savedSetting EEMEM;
const Settings defaultSettings PROGMEM = {
        70,                 //backlight_
        ANALOG_CELCIUS(50), //fanTempOn_
        ANALOG_CELCIUS(60), //dischargeTempOff_
        1,                  //externT_
        ANALOG_CELCIUS(60), //externTCO_
        ANALOG_CELCIUS(1),  //deltaT_
        ANALOG_VOLT(0.007), //deltaV_NiMH_
        ANALOG_VOLT(0.012), //deltaV_NiCd_
        5,                  //CDcycles_
        120,                //"%" capCutoff_
        ANALOG_VOLT(7),     //inputVoltageLow_
        Screen::Normal      //view_
};


void Settings::load() {
    eeprom::read(settings, &savedSetting);
    settings.apply();
}

void Settings::save() {
    eeprom::write(&savedSetting, settings);
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
    hardware::setLCDBacklight(backlight_);
}


//TODO: hmm... maybe it should be somewhere else
void Settings::edit()
{
    SettingsMenu menu(*this);
    if(menu.run()) {
        //save settings
        buzzer.soundSave();
        *this = menu.p_;
        save();
    }
    apply();
}

