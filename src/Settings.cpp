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

