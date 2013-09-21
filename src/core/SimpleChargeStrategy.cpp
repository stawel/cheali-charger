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
#include "SimpleChargeStrategy.h"
#include "ProgramData.h"
#include "Screen.h"
#include "Hardware.h"
#include "TheveninMethod.h"


SimpleChargeStrategy simpleChargeStrategy;


void SimpleChargeStrategy::powerOn()
{
    SMPS::powerOn();
    AnalogInputs::ValueType I = ProgramData::currentProgramData.battery.Ic;
    AnalogInputs::ValueType Vc = ProgramData::currentProgramData.getVoltage(ProgramData::VCharge);
    I/=5; //start charging with 0.2CmAh
    uint16_t value = analogInputs.reverseCalibrateValue(AnalogInputs::IsmpsValue, I);
    theveninMethod.setVI(Vc, value);
    theveninMethod.init();
    SMPS::setValue(value);
    Program::iName_ = AnalogInputs::IsmpsValue;
}

void SimpleChargeStrategy::powerOff()
{
    SMPS::powerOff();
}

void SimpleChargeStrategy::calculateThevenin() const
{
    if(isStable()) theveninMethod.calculateRthVth(SMPS::getValue());
}

/*
Strategy::statusType SimpleCharge::doStrategy()
{
    calculateThevenin();
    smps.setRealValue(ProgramData::currentProgramData.battery.Ic);
    if(smps.getIcharge() >= ProgramData::currentProgramData.battery.Ic) {
        smps.powerOff(SMPS::ERROR);
        return ERROR;
    }

    if(testVout_ && smps.getVout() >= ProgramData::currentProgramData.getVoltage(ProgramData::VCharge)) {
        smps.powerOff(SMPS::CHARGING_COMPLETE);
        return COMPLETE;
    }

    if(smps.getCharge() > ProgramData::currentProgramData.battery.C) {
        smps.powerOff(SMPS::CHARGING_COMPLETE);
        return COMPLETE;
    }
    return RUNNING;
}
*/

bool SimpleChargeStrategy::isStable() const
{
    return analogInputs.isStable(AnalogInputs::VoutBalancer) && analogInputs.isStable(AnalogInputs::Iout) && balancer.isStable();
}


