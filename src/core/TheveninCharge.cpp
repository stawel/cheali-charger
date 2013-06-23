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
#include "TheveninCharge.h"
#include "SMPS.h"
#include "Hardware.h"
#include "ProgramData.h"
#include "Screen.h"
#include "TheveninMethod.h"

TheveninChargeStrategy theveninChargeStrategy;

void TheveninChargeStrategy::powerOff()
{
    smps.powerOff();
}


void TheveninChargeStrategy::powerOn()
{
    smps.powerOn();
    balancer.powerOn();
    theveninMethod.init();
    Program::iName_ = AnalogInputs::IsmpsValue;
}

void TheveninChargeStrategy::setVI(AnalogInputs::ValueType v, AnalogInputs::ValueType i)
{
       theveninMethod.setVI(v, analogInputs.reverseCalibrateValue(AnalogInputs::IsmpsValue, i));
}
void TheveninChargeStrategy::setMinI(AnalogInputs::ValueType i)
{
       theveninMethod.setMinI(analogInputs.reverseCalibrateValue(AnalogInputs::IsmpsValue, i));
}

Strategy::statusType TheveninChargeStrategy::doStrategy()
{
    bool stable;
    bool isendVout = isEndVout();
    uint16_t oldValue = smps.getValue();

    stable = isStable() || isendVout;
    //test for charge complete
    if(theveninMethod.isComlete(isendVout, oldValue)) {
        smps.powerOff(SMPS::CHARGING_COMPLETE);
        return COMPLETE;
    }

    if(stable) {
        uint16_t value = theveninMethod.calculateNewValue(isendVout, oldValue);
        if(value != oldValue)
            smps.setValue(value);
    }
    return RUNNING;
}


bool TheveninChargeStrategy::isEndVout() const
{
    AnalogInputs::ValueType Vc = theveninMethod.Vend_;
    AnalogInputs::ValueType Vc_per_cell = balancer.calculatePerCell(Vc);

    return Vc <= smps.getVout() || balancer.isMaxVout(Vc_per_cell);
}


