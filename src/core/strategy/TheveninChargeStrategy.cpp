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
#include "TheveninChargeStrategy.h"
#include "SMPS.h"
#include "Hardware.h"
#include "ProgramData.h"
#include "Screen.h"
#include "TheveninMethod.h"
#include "Balancer.h"
#include "memory.h"

namespace TheveninChargeStrategy {
    const Strategy::VTable vtable PROGMEM = {
        powerOn,
        powerOff,
        doStrategy
    };

    bool isEndVout();
}

void TheveninChargeStrategy::powerOn()
{
    Balancer::powerOn();
    SMPS::powerOn();
    TheveninMethod::initialize(true);
}

void TheveninChargeStrategy::powerOff()
{
    SMPS::powerOff();
    Balancer::powerOff();
}

Strategy::statusType TheveninChargeStrategy::doStrategy()
{
    bool isendVout = isEndVout();
    AnalogInputs::ValueType I = SMPS::getIout();

    //balance, test if charge complete
    if(TheveninMethod::balance_isComplete(isendVout, I)) {
        SMPS::powerOff(SMPS::CHARGING_COMPLETE);
        return Strategy::COMPLETE;
    }
    AnalogInputs::ValueType newI = TheveninMethod::calculateNewI(isendVout, I);
    SMPS::trySetIout(newI);

    return Strategy::RUNNING;
}


bool TheveninChargeStrategy::isEndVout()
{
    AnalogInputs::ValueType Vc = Strategy::endV;
    AnalogInputs::ValueType Vc_per_cell = Balancer::calculatePerCell(Vc);

    bool end = Balancer::isMaxVout(Vc_per_cell);
    if(!Balancer::isWorking()) {
        end = end || Vc <= AnalogInputs::getVbattery();
    }
    return  end;
}


