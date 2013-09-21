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
#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <inttypes.h>
#include "AnalogInputs.h"
#include "Screen.h"

#ifndef DEFAULT_SETTINGS_EXTERNAL_T
#define DEFAULT_SETTINGS_EXTERNAL_T 1
#endif

struct Settings {

    static const AnalogInputs::ValueType TempDifference = ANALOG_CELCIUS(5);
#ifdef ENABLE_LCD_BACKLIGHT
    uint16_t backlight_;
#endif
#ifdef ENABLE_FAN
    AnalogInputs::ValueType fanTempOn_;
#endif
#ifdef ENABLE_T_INTERNAL
    AnalogInputs::ValueType dischargeTempOff_;
#endif
    uint16_t externT_;
    AnalogInputs::ValueType externTCO_;
    AnalogInputs::ValueType deltaT_;
    AnalogInputs::ValueType deltaV_NiMH_, deltaV_NiCd_;
    uint16_t CDcycles_;
    uint16_t capCutoff_;
    AnalogInputs::ValueType inputVoltageLow_;
    AnalogInputs::ValueType balancerError_;
    uint16_t view_;

    void edit();
    void check();
    void apply();
    void setDefault();
    bool isDebug() { return view_ == 1; /*TODO: */  }

    static void load();
    static void save();
    static void restoreDefault();
};

extern Settings settings;

#endif /* SETTINGS_H_ */
