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

namespace SMPS {
    STATE state_;
    uint32_t charge_;
    bool clearCharge_;
    uint16_t value_;

    STATE getState()    { return state_; }
    bool isPowerOn()    { return getState() == CHARGING; }
    bool isWorking()    { return value_ != 0; }


    uint16_t getValue() { return value_; }
    void setClearCharge(bool v) {clearCharge_ = v;}

}

void SMPS::initialize()
{
    setValue(0);
    powerOff(CHARGING_COMPLETE);
}


void SMPS::setValue(uint16_t value)
{
    if(value > SMPS_UPPERBOUND_VALUE)
        value = SMPS_UPPERBOUND_VALUE;
    value_ = value;
    hardware::setChargerValue(value_);
    analogInputs.resetMeasurement();
}

void SMPS::setRealValue(uint16_t I)
{
    uint16_t value = analogInputs.reverseCalibrateValue(AnalogInputs::IsmpsValue, I);
    setValue(value);
}

void SMPS::powerOn()
{
    if(isPowerOn())
        return;

    hardware::setBatteryOutput(true);
    setValue(0);
    hardware::setChargerOutput(true);
    analogInputs.powerOn();
    analogInputs.doFullMeasurement();
    state_ = CHARGING;
    if(clearCharge_)
        charge_ = 0;
}


void SMPS::powerOff(STATE reason)
{
    if(!isPowerOn() || reason == CHARGING)
        return;

    analogInputs.powerOff();
    setValue(0);
    hardware::setChargerOutput(false);
    hardware::setBatteryOutput(false);
    state_ = reason;
}

void SMPS::doSlowInterrupt()
{
   if(isPowerOn()) {
           charge_ += analogInputs.getIout();
   }
}

uint16_t SMPS::getCharge()
{
    uint32_t retu = charge_;
#if TIMER_INTERRUPT_PERIOD_MICROSECONDS == 512
//    retu *= TIMER_INTERRUPT_PERIOD_MICROSECONDS;
    retu /= (1000000/32);//*(3600/16) == TIMER_SLOW_INTERRUPT_INTERVAL
#else
#warning "TIMER_INTERRUPT_PERIOD_MICROSECONDS != 512"
    retu /= 1000000/TIMER_INTERRUPT_PERIOD_MICROSECONDS;
    retu /= 3600/TIMER_SLOW_INTERRUPT_INTERVAL;
#endif
    return retu;
}


