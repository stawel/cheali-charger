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
#include <Arduino.h>
#include "ProgramData.h"
#include "Balancer.h"
#include "Screen.h"

void Balancer::powerOn()
{
    hardware::setBatteryOutput(true);
    if(!analogInputs.isPowerOn()) {
        analogInputs.powerOn();
        analogInputs.doFullMeasurement();
    }

    cells_ = analogInputs.getRealValue(AnalogInputs::VbalanceInfo);
    for(int i = 0; i < cells_; i++) {
        AnalogInputs::ValueType vi = getV(i);
        Voff_[i] = Von_[i] = vi;
    }
    balance_ = 0;
    done_ = false;
    setBalance(0);
    startSwitchTime_ = 0;
}

uint8_t Balancer::getCellMinV() const
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
    return analogInputs.getRealValue(AnalogInputs::Name(AnalogInputs::Vb1+cell));
}

AnalogInputs::ValueType Balancer::getPresumedV(uint8_t cell) const
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
    analogInputs.powerOff();
    hardware::setBatteryOutput(false);
}

Balancer::Balancer()
{
    powerOff();
}

void Balancer::setBalance(uint16_t v)
{
//    if(timer.getMiliseconds() % 1024 > 900)
    //    v = 0;
    balance_ = v;
    startSwitchTime_ = timer.getMiliseconds();
    analogInputs.resetStable();
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

    bool off = true;

    if(vmin >= ProgramData::currentProgramData.getVoltagePerCell(ProgramData::VDischarge))
    {
        for(int i = 0; i < cells_; i++) {
            Von_[i] = Voff_[i] = getV(i);
            if(Von_[i] - vmin > error)
                off = false;
        }
    }

    savedVon_ = false;
    startBalanceTime_ = timer.getMiliseconds();
    if(off) {
        endBalancing();
    } else {
        setBalance(calculateBalance());
    }
}

uint16_t Balancer::calculateBalance()
{
    int16_t vmin = getPresumedV(minCell_);
    uint16_t retu = 0, b = 1;
    for(uint8_t c = 0; c < cells_; c++) {
        int16_t v = getPresumedV(c);
        if(b & balance_) {
            // if ON
            if(v > vmin)
                retu |=b;
        } else {
            // if OFF
            if(v > vmin + (error+1)/2)
                retu |=b;
        }
        b<<=1;
    }
    return retu;
}

bool Balancer::isStable(const uint16_t stableCount) const
{
    for(uint8_t c = 0; c < cells_; c++) {
        if(analogInputs.stableCount_[AnalogInputs::Vb1+c] < stableCount)
            return false;
    }
    return true;
}

void Balancer::trySaveVon() {
    if(savedVon_)
        return;
    savedVon_ = true;
    AnalogInputs::ValueType vmin = getV(minCell_);
    for(uint8_t c = 0; c < cells_; c++) {
        Von_[c] = getV(c);
    }
}

uint16_t Balancer::getBalanceTime() const
{
    return (timer.getMiliseconds() - startBalanceTime_) / 1000;
}


Strategy::statusType Balancer::doStrategy()
{
    if(done_)
        return COMPLETE;
    if(isStable()) {
        if(balance_ == 0) {
            startBalacing();
        } else {
            trySaveVon();
            uint16_t balance = calculateBalance();
            if(balance_ != balance || getBalanceTime() > maxBalanceTime) {
                setBalance(0);
            }
        }
    }
    return RUNNING;
}


bool Balancer::isMaxVout(AnalogInputs::ValueType maxV) const
{
    for(uint8_t c = 0; c < cells_; c++) {
        if(getV(c) >= maxV)
            return true;
        if(getPresumedV(c) >= maxV)
            return true;

    }
    return false;
}

bool Balancer::isMinVout(AnalogInputs::ValueType minV) const
{
    for(uint8_t c = 0; c < cells_; c++) {
        if(getV(c) <= minV)
            return true;
        if(getPresumedV(c) <= minV)
            return true;

    }
    return false;
}

AnalogInputs::ValueType Balancer::calculatePerCell(AnalogInputs::ValueType v) const
{
    if(cells_ == 0)
        return 0;
    return v/cells_;
}

