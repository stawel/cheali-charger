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
#include "ProgramData.h"
#include "Balancer.h"
#include "Screen.h"
#include "Settings.h"
#include "AnalogInputsPrivate.h"
#include "Hardware.h"
#include "memory.h"


namespace Balancer {
    uint8_t cells_;
    uint8_t minCell_;
    uint8_t balance_;
    bool done_;
    AnalogInputs::ValueType Von_[MAX_BANANCE_CELLS], Voff_[MAX_BANANCE_CELLS];
    bool savedVon_;
    uint16_t startBalanceTimeSecondsU16_;
    uint16_t balancingEnded_;

    uint32_t IVtime_;
    AnalogInputs::ValueType V_[MAX_BANANCE_CELLS];

    uint8_t getCells() { return cells_; }
    bool isWorking()  {
        if(balance_ != 0)
            return true;
        //wait until the balance port voltage stabilize
        if(!AnalogInputs::isPowerOn())
            return false;
        uint16_t isOff = AnalogInputs::getFullMeasurementCount() - balancingEnded_;
        return isOff < balancerStartStableCount/2;
    }

    const Strategy::VTable vtable PROGMEM = {
        powerOn,
        powerOff,
        doStrategy
    };


} // namespace Balancer

void Balancer::powerOn()
{
    hardware::setBalancerOutput(true);

    cells_ = AnalogInputs::getRealValue(AnalogInputs::VbalanceInfo);
    for(int i = 0; i < cells_; i++) {
        AnalogInputs::ValueType vi = getV(i);
        Voff_[i] = Von_[i] = vi;
    }
    balance_ = 0;
    done_ = false;
    setBalance(0);
    balancingEnded_ = 0;
}

uint8_t Balancer::getCellMinV()
{
    uint8_t c = 0;
    AnalogInputs::ValueType vmin = 65535;
    for(uint8_t i = 0; i < cells_; i++) {
        AnalogInputs::ValueType v = getV(i);
        if(vmin > v) {
            c = i;
            vmin = v;
        }
    }
    return c;
}


AnalogInputs::ValueType Balancer::getV(uint8_t cell)
{
    return AnalogInputs::getRealValue(AnalogInputs::Name(AnalogInputs::Vb1+cell));
}

AnalogInputs::ValueType Balancer::getPresumedV(uint8_t cell)
{
    if(balance_ == 0)
        return getV(cell);

    if(savedVon_)
        return (getV(cell) + Voff_[cell]) - Von_[cell] ;
    else
        return Voff_[cell];
}

void Balancer::endBalancing()
{
    setBalance(0);
    done_ = true;
}


void Balancer::powerOff()
{
    endBalancing();
    hardware::setBalancerOutput(false);
}


void Balancer::setBalance(uint8_t v)
{
    if(balance_ && v == 0)
        balancingEnded_ = AnalogInputs::getFullMeasurementCount();

    balance_ = v;
    AnalogInputs::resetStable();
    if(!done_)
        hardware::setBalancer(v);
}

void Balancer::startBalacing()
{
    //test if battery has recovered after last balancing
    if(!isStable(balancerStartStableCount))
        return;

    minCell_ = getCellMinV();
    AnalogInputs::ValueType vmin = getV(minCell_);

    //test if we can still discharge
    bool off = true;
    if(vmin >= ProgramData::currentProgramData.getVoltagePerCell(ProgramData::VDischarge)) {
        for(int i = 0; i < cells_; i++) {
            //save voltage values
            Von_[i] = Voff_[i] = getV(i);
            if(Von_[i] - vmin > settings.balancerError_)
                off = false;
        }
    }

    savedVon_ = false;
    startBalanceTimeSecondsU16_ = Time::getSecondsU16();
    if(off) {
        endBalancing();
    } else {
        setBalance(calculateBalance());
    }
}

uint8_t Balancer::calculateBalance()
{
    AnalogInputs::ValueType vmin = getPresumedV(minCell_);
    uint8_t retu = 0, b = 1;
    for(uint8_t c = 0; c < cells_; c++) {
        AnalogInputs::ValueType v = getPresumedV(c);
        if(b & balance_) {
            // if ON
            if(v > vmin)
                retu |=b;
        } else {
            // if OFF
            if(v > vmin + (settings.balancerError_+1)/2)
                retu |=b;
        }
        b<<=1;
    }
    return retu;
}

bool Balancer::isStable(const uint16_t stableCount)
{
    for(uint8_t c = 0; c < cells_; c++) {
        if(AnalogInputs::getStableCount(AnalogInputs::Name(AnalogInputs::Vb1+c)) < stableCount)
            return false;
    }
    return true;
}

void Balancer::trySaveVon() {
    if(savedVon_)
        return;
    savedVon_ = true;
    for(uint8_t c = 0; c < cells_; c++) {
        Von_[c] = getV(c);
    }
}

uint16_t Balancer::getBalanceTime()
{
    return Time::diffU16(startBalanceTimeSecondsU16_, Time::getSecondsU16());
}


Strategy::statusType Balancer::doStrategy()
{
    if(isStable()) {
        if(balance_ == 0) {
            startBalacing();
        } else {
            trySaveVon();
            uint8_t balance = calculateBalance();
            if((balance_ & (~balance)) || getBalanceTime() > maxBalanceTime) {
                setBalance(0);
            }
        }
    }
    if((!isWorking()) && done_)
        return Strategy::COMPLETE;
    return Strategy::RUNNING;
}


bool Balancer::isMaxVout(AnalogInputs::ValueType maxV)
{
    for(uint8_t c = 0; c < cells_; c++) {
        if(getV(c) >= maxV)
            return true;
        if(getPresumedV(c) >= maxV)
            return true;

    }
    return false;
}

bool Balancer::isMinVout(AnalogInputs::ValueType minV)
{
    for(uint8_t c = 0; c < cells_; c++) {
        if(getV(c) <= minV)
            return true;
        if(getPresumedV(c) <= minV)
            return true;

    }
    return false;
}

AnalogInputs::ValueType Balancer::calculatePerCell(AnalogInputs::ValueType v)
{
    if(cells_ == 0)
        return 0;
    return v/cells_;
}

