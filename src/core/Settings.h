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
    enum UARTInput {Software, Hardware};
    static const uint8_t UARTSpeeds = 5;
    static const AnalogInputs::ValueType TempDifference = ANALOG_CELCIUS(5);
    uint16_t backlight;
    AnalogInputs::ValueType fanTempOn;
    AnalogInputs::ValueType dischargeTempOff;
    uint16_t externT;
    AnalogInputs::ValueType externTCO;
    AnalogInputs::ValueType deltaT;
    uint16_t enable_deltaV;
    int16_t deltaV_NiMH, deltaV_NiCd;
    AnalogInputs::ValueType Rwire;
    uint16_t DCcycles;
    uint16_t DCRestTime;
    uint16_t audioBeep;
    uint16_t minIoutDiv;
    uint16_t minIout;
    uint16_t capCutoff;
    AnalogInputs::ValueType inputVoltageLow;
    int16_t overCharge_LiXX;
    int16_t overDischarge_LiXX;
    uint16_t dischargeAggressive_LiXX;
    uint16_t forceBalancePort;
    AnalogInputs::ValueType balancerError;
    uint16_t adcNoise;
    uint16_t UART;
    uint16_t UARTspeed;
    uint16_t UARTinput;

    uint16_t calibratedState;
    uint16_t SMPS_Upperbound_Value;
    uint16_t DISCHARGER_Upperbound_Value;

    void edit();
    void check();
    void apply();
    void setDefault();
    uint32_t getUARTspeed() const;

    static void load();
    static void save();
    static void restoreDefault();
} CHEALI_EEPROM_PACKED;

extern Settings settings;

#endif /* SETTINGS_H_ */
