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
#include "StorageStrategy.h"
#include "TheveninChargeStrategy.h"
#include "TheveninDischargeStrategy.h"
#include "memory.h"

namespace StorageStrategy {

    enum State  {Charge, Discharge, Balance};

    AnalogInputs::ValueType V_;
    AnalogInputs::ValueType Ic_;
    AnalogInputs::ValueType Id_;
    State state;
    bool doBalance_;
    void setDoBalance(bool v) { doBalance_ = v; }

    const Strategy::VTable vtable PROGMEM = {
        powerOn,
        powerOff,
        doStrategy
    };


}// namespace StorageStrategy

void StorageStrategy::powerOff()
{
    TheveninChargeStrategy::powerOff();
    TheveninDischargeStrategy::powerOff();
    Balancer::powerOff();
}

void StorageStrategy::powerOn()
{
    Balancer::powerOn();
    if(Balancer::isMinVout(Balancer::calculatePerCell(V_))) {
        TheveninChargeStrategy::setVIB(V_, Ic_, false);
        TheveninChargeStrategy::powerOn();
        state = Charge;
    } else {
        TheveninDischargeStrategy::setVI(V_, Id_);
        TheveninDischargeStrategy::powerOn();
        TheveninDischargeStrategy::endOnTheveninMethodComplete_ = true;
        state = Discharge;
    }
}


Strategy::statusType StorageStrategy::doStrategy()
{
    Strategy::statusType status;
    switch(state) {
        case Charge:
            status = TheveninChargeStrategy::doStrategy();
            break;
        case Discharge:
            status = TheveninDischargeStrategy::doStrategy();
            break;
        default: // Balance:
            status = Balancer::doStrategy();
            if(status != Strategy::RUNNING) {
                powerOff();
                return status;
            }
            break;
    }

    if(status == Strategy::COMPLETE && doBalance_) {
        status = Strategy::RUNNING;
        state = Balance;
        //turn one measurement
        Balancer::powerOn();
    }

    return status;
}

void StorageStrategy::setVII(AnalogInputs::ValueType V, AnalogInputs::ValueType Ic, AnalogInputs::ValueType Id)
{
    V_ = V;
    Ic_ = Ic;
    Id_ = Id;
}
