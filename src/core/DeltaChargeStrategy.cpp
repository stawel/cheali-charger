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

namespace DeltaChargeStrategy {
    enum StateType {PreCharge, RapidCharge};

    StateType state_;

    const Strategy::VTable vtable PROGMEM = {
        powerOn,
        SimpleChargeStrategy::powerOff,
        doStrategy
    };


}


void DeltaChargeStrategy::powerOn()
{
    state_ = PreCharge;
    SimpleChargeStrategy::powerOn();
}


Strategy::statusType DeltaChargeStrategy::doStrategy()
{
    SimpleChargeStrategy::calculateThevenin();
    AnalogInputs::ValueType Vout = AnalogInputs::getVout();

    if(state_ == PreCharge) {
        if(Vout > ProgramData::currentProgramData.getVoltage(ProgramData::VDischarge)) {
            state_ = RapidCharge;
            SMPS::setRealValue(ProgramData::currentProgramData.battery.Ic);
        }
    }

    if(AnalogInputs::isOutStable() && Vout > ProgramData::currentProgramData.getVoltage(ProgramData::VUpperLimit)) {
        Program::stopReason_ = PSTR("V limit");
        return Strategy::COMPLETE;
    }

    if(AnalogInputs::getDeltaCount() <= 1)
        return Strategy::RUNNING;

    if(settings.enable_deltaV_) {
        int x = AnalogInputs::getRealValue(AnalogInputs::deltaVout);
        x=-x;
        if(x > ProgramData::currentProgramData.getDeltaVLimit()) {
            Program::stopReason_ = PSTR("-dV");
            return Strategy::COMPLETE;
        }
    }
    if(settings.externT_) {
        int x = AnalogInputs::getRealValue(AnalogInputs::deltaTextern);
        if(x > ProgramData::currentProgramData.getDeltaTLimit()) {
            Program::stopReason_ = PSTR("dT/dt");
            return Strategy::COMPLETE;
        }
    }

    return Strategy::RUNNING;
}


