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
#include "Storage.h"
#include "TheveninCharge.h"
#include "TheveninDischarge.h"

StorageStrategy storageStrategy;

void StorageStrategy::powerOff()
{
    theveninChargeStrategy.powerOff();
    theveninDischargeStrategy.powerOff();
    balancer.powerOff();
}

void StorageStrategy::powerOn()
{
    balancer.powerOn();
    if(balancer.isMinVout(balancer.calculatePerCell(V_))) {
        theveninChargeStrategy.powerOn();
        state = Charge;
    } else {
        theveninDischargeStrategy.powerOn();
        state = Discharge;
    }
}


Strategy::statusType StorageStrategy::doStrategy()
{
    Strategy::statusType status;
    switch(state) {
        case Charge:
            status = theveninChargeStrategy.doStrategy();
            break;
        case Discharge:
            status = theveninDischargeStrategy.doStrategy();
            break;
        case Balance:
            status = balancer.doStrategy();
            if(status != RUNNING) {
                powerOff();
                return status;
            }
            break;
    }

    if(status == COMPLETE && doBalance_) {
        status = RUNNING;
        state = Balance;
        //turn one measurement
        balancer.powerOn();
    }

    return status;
}

void StorageStrategy::setVII(AnalogInputs::ValueType V, AnalogInputs::ValueType Ic, AnalogInputs::ValueType Id)
{
    V_ = V;
    theveninChargeStrategy.setVI(V, Ic);
    theveninDischargeStrategy.setVI(V, Id);
}
