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


struct Settings {

    enum UARTType {Disabled, Normal,  Debug,  ExtDebug, ExtDebugAdc};
    static const uint8_t UARTSpeeds = 5;
    static const AnalogInputs::ValueType TempDifference = ANALOG_CELCIUS(5);
    static const AnalogInputs::ValueType MaxDischargeOffset_LiXX = ANALOG_VOLT(1.000);
    uint16_t backlight_;
    AnalogInputs::ValueType fanTempOn_;
    AnalogInputs::ValueType dischargeTempOff_;
    uint16_t externT_;
    AnalogInputs::ValueType externTCO_;
    AnalogInputs::ValueType deltaT_;
    uint16_t enable_deltaV_;
    AnalogInputs::ValueType deltaV_NiMH_, deltaV_NiCd_;
    uint16_t CDcycles_;
    uint16_t WasteTime_;
 #ifdef ENABLE_MUTEAUDIO   
    uint16_t AudioBeep_;
 #endif   
    uint16_t Lixx_Imin_;
    uint16_t capCutoff_;
    AnalogInputs::ValueType inputVoltageLow_;
    AnalogInputs::ValueType dischargeOffset_LiXX_;
    uint16_t dischargeAggressive_LiXX_;
    uint16_t forceBalancePort_;
    AnalogInputs::ValueType balancerError_;
    uint16_t UART_;
    uint16_t UARTspeed_;
    uint16_t calibratedState_;
    uint16_t SMPS_Upperbound_Value_;
    uint16_t DISCHARGER_Upperbound_Value_;

    void edit();
    void check();
    void apply();
    void setDefault();
    uint32_t getUARTspeed() const;

    static void load();
    static void save();
    static void restoreDefault();
};

extern Settings settings;

#endif /* SETTINGS_H_ */
