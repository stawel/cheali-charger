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
#include "Utils.h"
#include "Settings.h"


namespace Discharger {
    bool on_ = false;
    uint16_t value_;
    AnalogInputs::ValueType IoutSet_;

    bool isPowerOn()    { return on_; }
    bool isWorking()    { return value_ != 0; }
    uint16_t getValue() { return value_; }
    AnalogInputs::ValueType getIout() { return IoutSet_; }

    AnalogInputs::ValueType getMaxIout()
    {
#ifdef ENABLE_T_INTERNAL
        bool tempcutoff;
        testTintern(tempcutoff, settings.dischargeTempOff - Settings::TempDifference, settings.dischargeTempOff);

        if(tempcutoff)
            return 0;
#endif

        AnalogInputs::ValueType v = AnalogInputs::getVout();
        if (v == 0) {
            v = 1;
        }
        AnalogInputs::ValueType i = AnalogInputs::evalI(MAX_DISCHARGE_P, v);

        if(i > MAX_DISCHARGE_I)
            i = MAX_DISCHARGE_I;
        return i;
    }
}

void Discharger::initialize()
{
    on_ = true;
    powerOff();
}

void Discharger::setValue(uint16_t value)
{
    if(value > DISCHARGER_UPPERBOUND_VALUE)
        value = DISCHARGER_UPPERBOUND_VALUE;
    value_ = value;
    hardware::setDischargerValue(value_);
    AnalogInputs::resetMeasurement();

}

void Discharger::trySetIout(AnalogInputs::ValueType I)
{
    AnalogInputs::ValueType maxI = getMaxIout();
    if(maxI < I) I = maxI;

    if(IoutSet_ == I) return;
    IoutSet_ = I;
    uint16_t value = AnalogInputs::reverseCalibrateValue(AnalogInputs::IdischargeSet, I);
    setValue(value);
}

void Discharger::powerOn()
{
    if(isPowerOn())
        return;

    setValue(0);
    IoutSet_ = 0;
    hardware::setDischargerOutput(true);
    on_ = true;
}

void Discharger::powerOff()
{
    if(!isPowerOn())
        return;

    setValue(0);
    IoutSet_ = 0;
    hardware::setDischargerOutput(false);
    on_ = false;
}
