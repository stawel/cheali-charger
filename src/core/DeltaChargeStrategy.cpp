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

DeltaChargeStrategy deltaChargeStrategy;

void DeltaChargeStrategy::powerOn()
{
    state_ = PreCharge;
    SimpleChargeStrategy::powerOn();
}


Strategy::statusType DeltaChargeStrategy::doStrategy()
{
    calculateThevenin();
    AnalogInputs::ValueType Vout = analogInputs.getVout();

    if(state_ == PreCharge) {
        if(Vout > ProgramData::currentProgramData.getVoltage(ProgramData::VDischarge)) {
            state_ = RapidCharge;
            SMPS::setRealValue(ProgramData::currentProgramData.battery.Ic);
        }
    }

    if(isStable() && Vout > ProgramData::currentProgramData.getVoltage(ProgramData::VUpperLimit)) {
        Program::stopReason_ = PSTR("V limit");
        return COMPLETE;
    }

    if(analogInputs.deltaCount_ <= 1)
        return RUNNING;

    if(testDeltaV_) {
        int x = analogInputs.getRealValue(AnalogInputs::deltaVout);
        x=-x;
        if(x > ProgramData::currentProgramData.getDeltaVLimit()) {
            Program::stopReason_ = PSTR("-dV");
            return COMPLETE;
        }
    }
    if(testDeltaT_) {
        int x = analogInputs.getRealValue(AnalogInputs::deltaTextern);
        if(x > ProgramData::currentProgramData.getDeltaTLimit()) {
            Program::stopReason_ = PSTR("dT/dt");
            return COMPLETE;
        }
    }

    return RUNNING;
}


