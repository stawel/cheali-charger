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
#include "SimpleDischarge.h"
#include "Hardware.h"
#include "ProgramData.h"


SimpleDischarge simpleDischarge;

void SimpleDischarge::powerOff()
{
    balancer.powerOff();
    discharger.powerOff();
}

void SimpleDischarge::powerOn()
{
    discharger.powerOn();
    balancer.powerOn();

    discharger.setRealValue(I_);
}


Strategy::statusType SimpleDischarge::doStrategy()
{
    if(isMinVout()) {
        discharger.powerOff(Discharger::DISCHARGING_COMPLETE);
        return COMPLETE;
    }
    return RUNNING;
}


bool SimpleDischarge::isMinVout() const
{
    AnalogInputs::ValueType Vc = V_;
    AnalogInputs::ValueType Vc_per_cell = balancer.calculatePerCell(Vc);

    return Vc >= discharger.getVout() || balancer.isMinVout(Vc_per_cell);
}


bool SimpleDischarge::isStable() const
{
    return analogInputs.isStable(discharger.VName) && analogInputs.isStable(discharger.IName) && balancer.isStable();
}
