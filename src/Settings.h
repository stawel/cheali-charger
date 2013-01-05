#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <inttypes.h>
#include "AnalogInputs.h"
#include "Screen.h"

struct Settings {

    static const AnalogInputs::ValueType minTempDifference = ANALOG_CELCIUS(1);

    uint16_t backlight_;
    AnalogInputs::ValueType fanTempOn_,fanTempOff_;
    AnalogInputs::ValueType dischargeTempOn_,dischargeTempOff_;
    AnalogInputs::ValueType inputVoltageLow_;
    AnalogInputs::ValueType deltaV_NiCd_, deltaV_NiMH_;
    AnalogInputs::ValueType deltaT_;

    Screen::ScreenViewType view_;

    void edit();
    void check();
    void apply();
    bool isDebug() { return view_ == Screen::Debug; }

    static void load();
    static void save();
    static void restoreDefault();
};

extern Settings settings;

#endif /* SETTINGS_H_ */
