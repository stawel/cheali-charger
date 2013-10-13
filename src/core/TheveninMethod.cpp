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


namespace TheveninMethod {
    uint16_t minValue_;
    uint16_t maxValue_;
    AnalogInputs::ValueType Vend_;
    AnalogInputs::ValueType valueTh_;

    Thevenin tVout_;
    Thevenin tBal_[MAX_BANANCE_CELLS];
    FallingState Ifalling_;
    uint8_t fullCount_;
    uint8_t cells_;
    AnalogInputs::Name iName_;
    bool balance_;
    Strategy::statusType bstatus_;

    void setMinI(AnalogInputs::ValueType i) {    minValue_ = i; };

    uint16_t getMinValueB() {
        if(bstatus_ != Strategy::COMPLETE)
            return 0;
        else return minValue_;
    }
    AnalogInputs::ValueType getImax()
    {
        return AnalogInputs::calibrateValue(iName_, maxValue_);
    }

}

AnalogInputs::ValueType TheveninMethod::getReadableRthCell(uint8_t cell)
{
    return tBal_[cell].Rth_.getReadableRth_calibrateI(iName_);
}
AnalogInputs::ValueType TheveninMethod::getReadableBattRth()
{
    return tVout_.Rth_.getReadableRth_calibrateI(iName_);
}

AnalogInputs::ValueType TheveninMethod::getReadableWiresRth()
{
    Resistance R;
    R.V_ =  AnalogInputs::getRealValue(AnalogInputs::Vout);
    R.V_ -= AnalogInputs::getRealValue(AnalogInputs::Vbalancer);
    R.I_ = AnalogInputs::getRealValue(AnalogInputs::Iout);
    return R.getReadableRth();

}



void TheveninMethod::setVIB(AnalogInputs::ValueType Vend, AnalogInputs::ValueType i, bool balance)
{
    Vend_ = Vend;
    maxValue_ = i;
    minValue_ = i/10;
    balance_ = balance;
}

void TheveninMethod::initialize(AnalogInputs::Name iName)
{
    bstatus_ = Strategy::COMPLETE;

    iName_ = iName;
    AnalogInputs::ValueType Vout = AnalogInputs::getVout();
    tVout_.init(Vout, Vend_, minValue_);

    cells_ = Balancer::getCells();
    AnalogInputs::ValueType Vend_per_cell = Balancer::calculatePerCell(Vend_);

    for(uint8_t c = 0; c < cells_; c++) {
        AnalogInputs::ValueType v = Balancer::getPresumedV(c);
        tBal_[c].init(v, Vend_per_cell, minValue_);
    }

    Ifalling_ = NotFalling;
    fullCount_ = 0;
}


bool TheveninMethod::isComlete(bool isEndVout, AnalogInputs::ValueType value)
{
    if(balance_) {
        if(Ifalling_ == NotFalling)
            Balancer::done_ = false;
        bstatus_ = Balancer::doStrategy();
    }

    if(bstatus_ != Strategy::COMPLETE)
        return false;

    isEndVout |= (Ifalling_ == Falling)  && value == 0;

    if(value <= getMinValueB() && isEndVout) {
        if(fullCount_++ >= 10) {
            return true;
        }
    } else {
        fullCount_ = 0;
    }
    return false;
}


AnalogInputs::ValueType TheveninMethod::calculateNewValue(bool isEndVout, AnalogInputs::ValueType oldValue)
{
    AnalogInputs::ValueType i;

    //test for maximum output voltage reached
    if(isEndVout) {
        switch(Ifalling_) {
        case NotFalling:
            Ifalling_ = LastRthMesurment;
            //temporarily turn off
            storeOldValue(oldValue);
            return 0;
        default:
            Ifalling_ = Falling;
            break;
        }
    }

    calculateRthVth(oldValue);
    i = calculateI();
    return normalizeI(i, oldValue);
}


void TheveninMethod::calculateRthVth(AnalogInputs::ValueType oldValue)
{
    tVout_.calculateRthVth(AnalogInputs::getVout(),oldValue);

    for(uint8_t c = 0; c < cells_; c++) {
        tBal_[c].calculateRthVth(Balancer::getPresumedV(c),oldValue);
    }
}

AnalogInputs::ValueType TheveninMethod::calculateI()
{
    AnalogInputs::ValueType i = tVout_.calculateI(Vend_);
    AnalogInputs::ValueType Vend_per_cell = Balancer::calculatePerCell(Vend_);
    for(uint8_t c = 0; c < cells_; c++) {
        i = min(i, tBal_[c].calculateI(Vend_per_cell));
    }
    return i;
}

AnalogInputs::ValueType TheveninMethod::normalizeI(AnalogInputs::ValueType value, AnalogInputs::ValueType oldValue)
{
    valueTh_ = value;
    if(value > maxValue_) {
        value = maxValue_;
    }
    if(value < getMinValueB()) {
        value = getMinValueB();
    }

    if(oldValue != value) {
        if(Ifalling_ != Falling
            || value < oldValue
            || value <= minValue_) {

            storeOldValue(oldValue);
            return value;
        }
    }
    return oldValue;
}

void TheveninMethod::storeOldValue(AnalogInputs::ValueType oldValue)
{
    tVout_.storeLast(AnalogInputs::getVout(), oldValue);

    for(uint8_t i = 0; i < cells_; i++) {
        AnalogInputs::ValueType vi = Balancer::getPresumedV(i);
        tBal_[i].storeLast(vi, oldValue);
    }
}
