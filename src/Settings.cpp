#include "Settings.h"
#include "memory.h"
#include "SettingsMenu.h"
#include "Hardware.h"

Settings settings;

Settings savedSetting EEMEM;
const Settings defaultSettings PROGMEM = {
        70,
        ANALOG_CELCIUS(50), ANALOG_CELCIUS(40),
        ANALOG_CELCIUS(55), ANALOG_CELCIUS(60),
        ANALOG_VOLT(7),
        Screen::Simple
};


void Settings::load() {
    eeprom::read(settings, &savedSetting);
    settings.apply();
}

void Settings::save() {
    eeprom::write(&savedSetting, settings);
    settings.apply();
}

void Settings::restoreDefault() {
    pgm::read(settings, &defaultSettings);
    Settings::save();
}

void Settings::check() {
    AnalogInputs::ValueType v = fanTempOn_ - minTempDifference;
    if(fanTempOff_ > v) fanTempOff_ = v;

    v = dischargeTempOff_ - minTempDifference;
    if(dischargeTempOn_ > v) dischargeTempOn_ = v;
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
        *this = menu.p_;
        save();
    }
    apply();
}

