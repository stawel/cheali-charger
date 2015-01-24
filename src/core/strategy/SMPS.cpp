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
#include "Hardware.h"
#include "SMPS.h"
#include "Program.h"
#include "EditMenu.h"
#include "LcdPrint.h"
#include "Screen.h"
#include "Settings.h"

#ifndef SMPS_MAX_CURRENT_CHANGE
#define SMPS_MAX_CURRENT_CHANGE     ANALOG_AMP(0.200)
#endif

#define SMPS_MAX_CURRENT_CHANGE_dM  ((AnalogInputs::ValueType)(SMPS_MAX_CURRENT_CHANGE*0.7))

namespace SMPS {
    STATE state_;
    uint16_t value_;
    AnalogInputs::ValueType IoutSet_;

    STATE getState()    { return state_; }
    bool isPowerOn()    { return getState() == CHARGING; }
    bool isWorking()    { return value_ != 0; }
    uint16_t getValue() { return value_; }
    AnalogInputs::ValueType getIout() { return IoutSet_; }

    void setValue(uint16_t value);

    AnalogInputs::ValueType getMaxIout()
    {
        uint32_t i = MAX_CHARGE_P;
        AnalogInputs::ValueType v = AnalogInputs::getVout();
        if (v == 0) {
            v = 1;
        }
        i *= ANALOG_VOLT(1);

#ifdef ENABLE_DYNAMIC_MAX_POWER
        AnalogInputs::ValueType v_in_10 = AnalogInputs::getRealValue(AnalogInputs::Vin);
        if(v_in_10 > ANALOG_VOLT(10)) {
            v_in_10 -= ANALOG_VOLT(10);
            if(v > v_in_10) {
                v -= v_in_10;
            } else {
                v = 1;
            }
        }
#endif

        i /= v;

        if(i > MAX_CHARGE_I)
            i = MAX_CHARGE_I;
        return i;
    }
}

void SMPS::initialize()
{
    value_ = 0;
    IoutSet_ = 0;
    setValue(0);
    powerOff(CHARGING_COMPLETE);
}


void SMPS::setValue(uint16_t value)
{
    if(value > SMPS_UPPERBOUND_VALUE)
        value = SMPS_UPPERBOUND_VALUE;
    value_ = value;

    hardware::setChargerValue(value_);
    AnalogInputs::resetMeasurement();
}

void SMPS::trySetIout(AnalogInputs::ValueType I)
{
    AnalogInputs::ValueType maxI = getMaxIout();
    if(maxI < I) I = maxI;

    if(I < IoutSet_) {
        if(SMPS_MAX_CURRENT_CHANGE_dM < IoutSet_ - I)
            I = IoutSet_ - SMPS_MAX_CURRENT_CHANGE_dM;
    } else {
        if(SMPS_MAX_CURRENT_CHANGE_dM < I - IoutSet_)
            I = IoutSet_ + SMPS_MAX_CURRENT_CHANGE_dM;
    }

    if(IoutSet_ == I) return;
    IoutSet_ = I;
    uint16_t value = AnalogInputs::reverseCalibrateValue(AnalogInputs::IsmpsSet, I);
    setValue(value);
}

void SMPS::powerOn()
{
    if(isPowerOn())
        return;
    //reset rising value
    value_ = 0;
    IoutSet_ = 0;
    setValue(0);
    hardware::setChargerOutput(true);
    state_ = CHARGING;
}


void SMPS::powerOff(STATE reason)
{
    if(!isPowerOn() || reason == CHARGING)
        return;

    setValue(0);
    //reset rising value
    value_ = 0;
    IoutSet_ = 0;
    hardware::setChargerOutput(false);
    state_ = reason;
}
