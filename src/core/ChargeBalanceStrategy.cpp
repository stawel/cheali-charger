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
#include "ChargeBalanceStrategy.h"
#include "TheveninChargeStrategy.h"

ChargeBalanceStrategy chargeBalanceStrategy;


// This is a simple implementation of "Charge + Balance",
// the "Charge" and "Balance" phases are separated.
// Theoretically, it is possible to "Balance" and "Charge"
// at the same time, but when we "Balance" the voltage
// on the balance port drops, so we would have to do it "blindly".
// (may be implemented in the future)

void ChargeBalanceStrategy::powerOff()
{
    theveninChargeStrategy.powerOff();
    balancer.powerOff();
    SMPS::setClearCharge(true);
}

void ChargeBalanceStrategy::powerOn()
{
    theveninChargeStrategy.powerOn();
    state_ = Charge;
    SMPS::setClearCharge(false);
    count_ = CHARGE_BALANCE_COUNT;
}


Strategy::statusType ChargeBalanceStrategy::doStrategy()
{
    Strategy::statusType status;
    if(state_ == Charge) {
        status = theveninChargeStrategy.doStrategy();
    } else {
        status = balancer.doStrategy();
    }
    if(status == COMPLETE && count_) {
        if(state_ == Charge) {
            theveninChargeStrategy.powerOff();
            balancer.powerOn();
            state_ = Balance;
        } else {
            theveninChargeStrategy.powerOn();
            state_ = Charge;
        }

        status = RUNNING;
        count_--;
    }
    return status;
}

void ChargeBalanceStrategy::setVI(AnalogInputs::ValueType V, AnalogInputs::ValueType Ic)
{
    theveninChargeStrategy.setVI(V, Ic);
}
