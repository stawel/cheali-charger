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
#include "SimpleDischargeStrategy.h"
#include "Hardware.h"
#include "ProgramData.h"


SimpleDischargeStrategy simpleDischargeStrategy;

void SimpleDischargeStrategy::powerOff()
{
    balancer.powerOff();
    Discharger::powerOff();
}

void SimpleDischargeStrategy::powerOn()
{
    Discharger::powerOn();
    balancer.powerOn();

    Discharger::setRealValue(I_);
}


Strategy::statusType SimpleDischargeStrategy::doStrategy()
{
    if(isMinVout()) {
        Discharger::powerOff(Discharger::DISCHARGING_COMPLETE);
        return COMPLETE;
    }
    return RUNNING;
}


bool SimpleDischargeStrategy::isMinVout() const
{
    AnalogInputs::ValueType Vc = V_;
    AnalogInputs::ValueType Vc_per_cell = balancer.calculatePerCell(Vc);

    return Vc >= AnalogInputs::getVout() || balancer.isMinVout(Vc_per_cell);
}


