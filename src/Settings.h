#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <inttypes.h>
#include "AnalogInputs.h"
#include "Screen.h"

struct Settings {

    static const AnalogInputs::ValueType TempDifference = ANALOG_CELCIUS(5);

    uint16_t backlight_;
    AnalogInputs::ValueType fanTempOn_;
    AnalogInputs::ValueType dischargeTempOff_;
    uint16_t externT_;
    AnalogInputs::ValueType externTCO_;
    AnalogInputs::ValueType deltaT_;
    AnalogInputs::ValueType deltaV_NiMH_, deltaV_NiCd_;
    uint16_t CDcycles_;
    uint16_t capCutoff_;
    AnalogInputs::ValueType inputVoltageLow_;
    Screen::ScreenViewType view_;

    void edit();
    void check();
    void apply();
    void setDefault();
    bool isDebug() { return view_ == Screen::Debug; }

    static void load();
    static void save();
    static void restoreDefault();
};

extern Settings settings;

#endif /* SETTINGS_H_ */
