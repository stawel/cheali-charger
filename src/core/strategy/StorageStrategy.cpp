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
#include "Balancer.h"

namespace StorageStrategy {

    enum State  {Charge, Discharge, Balance};
    enum State state;

    const PROGMEM struct Strategy::VTable vtable = {
        powerOn,
        powerOff,
        doStrategy
    };


} // namespace StorageStrategy

void StorageStrategy::powerOff()
{
    TheveninChargeStrategy::powerOff();
    TheveninDischargeStrategy::powerOff();
    Balancer::powerOff();
}

void StorageStrategy::powerOn()
{
    AnalogInputs::ValueType V;
    bool charge;

    Balancer::powerOn();
    Strategy::setVI(ProgramData::VStorage, true);
    V = Strategy::endV;
    if(Balancer::getConnectedCellsCount() == 0) {
        charge = AnalogInputs::getVbattery() <= V;
    } else {
        charge = Balancer::isMinVout(Strategy::endVperCell);
    }
    if(charge) {
        TheveninChargeStrategy::powerOn();
        state = Charge;
    } else {
        Strategy::setVI(ProgramData::VStorage, false);
        TheveninDischargeStrategy::powerOn();
        TheveninDischargeStrategy::endOnTheveninMethodComplete_ = true;
        state = Discharge;
    }
}


enum Strategy::statusType StorageStrategy::doStrategy()
{
    enum Strategy::statusType status;
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

    if(status == Strategy::COMPLETE && Strategy::doBalance) {
        status = Strategy::RUNNING;
        state = Balance;
        //turn one measurement
        Balancer::powerOn();
    }

    return status;
}
