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
    uint16_t minBalanceValue_;
    uint16_t maxValue_;
    AnalogInputs::ValueType Vend_;
    AnalogInputs::ValueType valueTh_;
    uint16_t lastBallancingEnded_;
    uint16_t getReadableRth_ = 65535;
    uint16_t tempRth = 0;


    Thevenin tVout_;
    Thevenin tBal_[MAX_BANANCE_CELLS];
    FallingState Ifalling_;
    uint8_t fullCount_;
    uint8_t cells_;
    AnalogInputs::Name iName_;
    bool balance_;
    Strategy::statusType bstatus_;
    AnalogInputs::ValueType idebug_;
    
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

    bool isBelowMin(AnalogInputs::ValueType value)
    {
        if(Ifalling_ == LastRthMesurment)
            return false;
        return value < minValue_;
    }

}

AnalogInputs::ValueType TheveninMethod::getReadableRthCell(uint8_t cell)
{
    return tBal_[cell].Rth_.getReadableRth_calibrateI(iName_);
//      return tBal_[cell].ILastDiff_;
}
AnalogInputs::ValueType TheveninMethod::getReadableBattRth()
{
    return tVout_.Rth_.getReadableRth_calibrateI(iName_);
}

AnalogInputs::ValueType TheveninMethod::getReadableWiresRth()
{
    //only is poweron
    if (!SMPS::isPowerOn() && !Discharger::isPowerOn()) 
    {
    getReadableRth_ = 65535;
    return tempRth;
    }
    Resistance R;
    R.iV_ =  AnalogInputs::getRealValue(AnalogInputs::Vout);
    R.iV_ -= AnalogInputs::getRealValue(AnalogInputs::Vbalancer);
    R.uI_ = AnalogInputs::getRealValue(AnalogInputs::Iout);
    
    if (R.uI_ > 0)
    {
      if (getReadableRth_ > R.getReadableRth()) 
      {
       //return smallest value
       getReadableRth_ = R.getReadableRth();
      }
    } 
    
    if (getReadableRth_ == 65535) 
    { 
      return tempRth;
    }
    
    tempRth = getReadableRth_;
    return tempRth;
}



void TheveninMethod::setVIB(AnalogInputs::ValueType Vend, AnalogInputs::ValueType i, bool balance)
{
    Vend_ = Vend;
    maxValue_ = i;
    //minValue_ = i/10;
    minValue_ = i/20;
    if(minValue_ < 50) minValue_ = 50;
    balance_ = balance;
}


void TheveninMethod::initialize(AnalogInputs::Name iName)
{
    bstatus_ = Strategy::COMPLETE;
    bool charge = (iName == AnalogInputs::IsmpsValue);

    iName_ = iName;
    minBalanceValue_ = AnalogInputs::reverseCalibrateValue(iName_, Balancer::Ibalance);
    AnalogInputs::ValueType Vout = AnalogInputs::getVout();
    tVout_.init(Vout, Vend_, minValue_, charge);

    cells_ = Balancer::getCells();
    AnalogInputs::ValueType Vend_per_cell = Balancer::calculatePerCell(Vend_);

    for(uint8_t c = 0; c < cells_; c++) {
        AnalogInputs::ValueType v = Balancer::getPresumedV(c);
        tBal_[c].init(v, Vend_per_cell, minValue_, charge);
    }

    Ifalling_ = NotFalling;
    fullCount_ = 0;
}

//TODO: the TheveninMethod  is too complex, should be refactored, maybe when switching to mAmps


bool TheveninMethod::isComlete(bool isEndVout, AnalogInputs::ValueType value)
{
    if(balance_) {
        if(value > max(minBalanceValue_, minValue_))
            Balancer::done_ = false;
        if(Ifalling_ != LastRthMesurment)
            bstatus_ = Balancer::doStrategy();
    }

    if(bstatus_ != Strategy::COMPLETE)
        return false;

    isEndVout |= (Ifalling_ == Falling)  && value < minValue_;

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

    calculateRthVth(oldValue);
    storeOldValue(oldValue);

    i = calculateI();
    idebug_ = i;
    i = normalizeI(i, oldValue);

    //test if maximum output voltage reached
    switch(Ifalling_) {
    case NotFalling:
        if(!isEndVout)
            break;
        if(balance_) {
            Balancer::endBalancing();
            Balancer::done_ = false;
        }
        Ifalling_ = LastRthMesurment;
        //temporarily turn off
        i = 0;
        break;
    default:
        Ifalling_ = Falling;
        break;
    }
    return i;
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
            || (oldValue <= minValue_ && lastBallancingEnded_ != Balancer::balancingEnded_)) {

            lastBallancingEnded_ = Balancer::balancingEnded_;
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
