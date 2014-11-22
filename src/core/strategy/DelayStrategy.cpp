/*
    cheali-charger - open source firmware for a variety of LiPo chargers
    Copyright (C) 2013  Pawel Stawicki. All right reserved.

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


#include "DelayStrategy.h"
#include "Time.h"
#include "memory.h"
#include "Strategy.h"
#include "Buzzer.h"

namespace DelayStrategy {
     const Strategy::VTable vtable PROGMEM = {
        powerOn,
        powerOff,
        doStrategy
     };

    bool state_ = false;
    uint16_t start_time_U16_;
    uint16_t delay_;

}// namespace DelayStrategy

void DelayStrategy::powerOn()
{
    start_time_U16_ = Time::getMinutesU16();
}

void DelayStrategy::powerOff()
{
    state_ = false;
}

Strategy::statusType DelayStrategy::doStrategy()
{
    if(Time::diffU16(start_time_U16_, Time::getMinutesU16()) <= delay_) {
        state_ = true;
        return Strategy::RUNNING;
    } else {
        state_ = false;
        return Strategy::COMPLETE;
    }
}

bool DelayStrategy::isDelay()
{
   return state_;
}

void DelayStrategy::setDelay(uint16_t minutes)
{
   delay_ = minutes;
   DelayStrategy::powerOn();
}

