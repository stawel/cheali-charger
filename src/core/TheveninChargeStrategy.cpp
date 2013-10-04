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


namespace TheveninChargeStrategy {
    const Strategy::VTable vtable PROGMEM = {
        powerOn,
        powerOff,
        doStrategy
    };
}

void TheveninChargeStrategy::powerOff()
{
    SMPS::powerOff();
}


void TheveninChargeStrategy::powerOn()
{
    SMPS::powerOn();
    Balancer::powerOn();
    TheveninMethod::initialize(AnalogInputs::IsmpsValue);
}

void TheveninChargeStrategy::setVI(AnalogInputs::ValueType v, AnalogInputs::ValueType i)
{
       TheveninMethod::setVI(v, AnalogInputs::reverseCalibrateValue(AnalogInputs::IsmpsValue, i));
}
void TheveninChargeStrategy::setMinI(AnalogInputs::ValueType i)
{
       TheveninMethod::setMinI(AnalogInputs::reverseCalibrateValue(AnalogInputs::IsmpsValue, i));
}

Strategy::statusType TheveninChargeStrategy::doStrategy()
{
    bool stable;
    bool isendVout = isEndVout();
    uint16_t oldValue = SMPS::getValue();

    stable = AnalogInputs::isOutStable() || isendVout;
    //test for charge complete
    if(TheveninMethod::isComlete(isendVout, oldValue)) {
        SMPS::powerOff(SMPS::CHARGING_COMPLETE);
        return Strategy::COMPLETE;
    }

    if(stable) {
        uint16_t value = TheveninMethod::calculateNewValue(isendVout, oldValue);
        if(value != oldValue)
            SMPS::setValue(value);
    }
    return Strategy::RUNNING;
}


bool TheveninChargeStrategy::isEndVout()
{
    AnalogInputs::ValueType Vc = TheveninMethod::Vend_;
    AnalogInputs::ValueType Vc_per_cell = Balancer::calculatePerCell(Vc);

    return Vc <= AnalogInputs::getVout() || Balancer::isMaxVout(Vc_per_cell);
}


