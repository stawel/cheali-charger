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
#include "Discharger.h"
#include "Program.h"    
#include "Utils.h"
#include "Settings.h"


namespace Discharger {
#ifdef ENABLE_T_INTERNAL
    uint16_t correctValueTintern(uint16_t v);
    bool tempcutoff_;
#endif
    void finalizeValueTintern(bool force);

    STATE state_;
    uint16_t value_,valueSet_;

    STATE getState()    { return state_; }
    bool isPowerOn()    { return getState() == DISCHARGING; }
    bool isWorking()    { return value_ != 0; }
    uint16_t getValue() { return value_; }

}

void Discharger::initialize()
{
    setValue(0);
    powerOff(DISCHARGING_COMPLETE);
}

void Discharger::setValue(uint16_t value)
{
    //if (settings.calibratedState_ >= 7)
    if (Program::programState_ != Program::Calibration)
    {
        if(value > settings.DISCHARGER_Upperbound_Value_) value = settings.DISCHARGER_Upperbound_Value_;
    }
    valueSet_ = value;
    finalizeValueTintern(true);
}

#ifdef ENABLE_T_INTERNAL
uint16_t Discharger::correctValueTintern(uint16_t v)
{
    testTintern(tempcutoff_, settings.dischargeTempOff_ - Settings::TempDifference, settings.dischargeTempOff_);

    if(tempcutoff_)
        v = 0;
    return v;
}
#endif

void Discharger::finalizeValueTintern(bool force)
{
#ifdef ENABLE_T_INTERNAL
    uint16_t  v = correctValueTintern(valueSet_);
#else
    uint16_t  v = valueSet_;
#endif

    if(v != value_ || force) {
        value_ = v;
        hardware::setDischargerValue(value_);
        AnalogInputs::resetMeasurement();
    }
}

void Discharger::setRealValue(uint16_t I)
{
    uint16_t value = AnalogInputs::reverseCalibrateValue(AnalogInputs::IdischargeValue, I);
    setValue(value);
}

void Discharger::powerOn()
{
    if(isPowerOn())
        return;

    setValue(0);
    hardware::setDischargerOutput(true);
    state_ = DISCHARGING;
}

void Discharger::powerOff(STATE reason)
{
    if(!isPowerOn() || reason == DISCHARGING)
        return;

    setValue(0);
    hardware::setDischargerOutput(false);
    state_ = reason;
}

void Discharger::doIdle()
{
#ifdef ENABLE_T_INTERNAL
    if(isPowerOn()) {
        finalizeValueTintern(false);
    }
#endif
}
