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
#include "ProgramData.h"
#include "Screen.h"
#include "TheveninDischargeStrategy.h"
#include "Balancer.h"
#include "TheveninMethod.h"
#include "Settings.h"
#include "memory.h"

namespace TheveninDischargeStrategy {
    const Strategy::VTable vtable PROGMEM = {
        powerOn,
        powerOff,
        doStrategy
    };

    bool endOnTheveninMethodComplete_;
    bool isEndVout();

}



void TheveninDischargeStrategy::powerOff()
{
    Discharger::powerOff();
    Balancer::powerOff();
}


void TheveninDischargeStrategy::powerOn()
{
    Discharger::powerOn();
    Balancer::powerOn();
    //end on minimum Voltage reached or TheveninMethodComplete
    endOnTheveninMethodComplete_ = settings.dischargeAggressive_LiXX;
    TheveninMethod::initialize(false);
}

Strategy::statusType TheveninDischargeStrategy::doStrategy()
{
    bool isendVout = isEndVout();
    AnalogInputs::ValueType I = Discharger::getIout();

    //test for charge complete
    bool end = isendVout;
    if(endOnTheveninMethodComplete_) {
        end = TheveninMethod::balance_isComplete(isendVout, I);
    }
    if(end) {
        Discharger::powerOff(Discharger::DISCHARGING_COMPLETE);
        return Strategy::COMPLETE;
    }

    AnalogInputs::ValueType newI = TheveninMethod::calculateNewI(isendVout, I);
    Discharger::trySetIout(newI);

    return Strategy::RUNNING;
}

bool TheveninDischargeStrategy::isEndVout()
{
    AnalogInputs::ValueType Vc = Strategy::endV;
    AnalogInputs::ValueType Vc_per_cell = Balancer::calculatePerCell(Vc);

    bool end = Balancer::isMinVout(Vc_per_cell);
    if(!Balancer::isWorking()) {
        end = end || Vc >= AnalogInputs::getVbattery();
    }
    return  end;
}

