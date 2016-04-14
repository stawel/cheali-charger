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
#include "DeltaChargeStrategy.h"
#include "Hardware.h"
#include "ProgramData.h"
#include "Screen.h"
#include "TheveninMethod.h"
#include "Program.h"
#include "memory.h"
#include "Settings.h"

#define DELTA_COUNTS_PER_MINUTE (60/(ANALOG_INPUTS_DELTA_TIME_MILISECONDS/1000))

namespace DeltaChargeStrategy {

    void powerOn();
    Strategy::statusType doStrategy();

    const Strategy::VTable vtable PROGMEM = {
        powerOn,
        SimpleChargeStrategy::powerOff,
        doStrategy
    };
}

void DeltaChargeStrategy::powerOn()
{
    SimpleChargeStrategy::powerOn();
}

Strategy::statusType DeltaChargeStrategy::doStrategy()
{
    SimpleChargeStrategy::calculateThevenin();
    AnalogInputs::ValueType Vout = AnalogInputs::getVbattery();

    if(ProgramData::getVoltage(ProgramData::VDischarged) < Vout) {
        SMPS::trySetIout(Strategy::maxI);
    }

    if(Vout > Strategy::endV) {
        Program::stopReason = string_batteryVoltageReachedUpperLimit;
        return Strategy::COMPLETE;
    }

    //we don't have enough data to compute delta values (we need at least 2)
    if(AnalogInputs::getDeltaCount() < 2)
        return Strategy::RUNNING;

    if(ProgramData::battery.enable_externT) {
        int16_t x = AnalogInputs::getRealValue(AnalogInputs::deltaTextern);
        if(x > ProgramData::getDeltaTLimit()) {
            Program::stopReason = string_externalTemperatureReachedDeltaTLimit;
            return Strategy::COMPLETE;
        }
    }

    //ignore few first -dV values until output voltage is stable
    bool dontIgnore = AnalogInputs::getDeltaCount() >= ProgramData::battery.deltaVIgnoreTime * DELTA_COUNTS_PER_MINUTE;
    AnalogInputs::enableDeltaVoutMax(dontIgnore);
    if(dontIgnore) {
        if(ProgramData::battery.enable_deltaV) {
            int16_t x = AnalogInputs::getRealValue(AnalogInputs::deltaVout);
            if(x < ProgramData::getDeltaVLimit()) {
                Program::stopReason = string_batteryVoltageReachedDeltaVLimit;
                return Strategy::COMPLETE;
            }
        }
    }

    return Strategy::RUNNING;
}


