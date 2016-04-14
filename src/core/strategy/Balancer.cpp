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
#define __STDC_LIMIT_MACROS
#include "ProgramData.h"
#include "Balancer.h"
#include "Screen.h"
#include "Settings.h"
#include "AnalogInputsPrivate.h"
#include "Hardware.h"
#include "memory.h"
#include "Utils.h"

//#define ENABLE_DEBUG
#include "debug.h"

namespace Balancer {
    int8_t minCell;
    uint16_t balance;
    bool done;
    AnalogInputs::ValueType Von_[MAX_BANANCE_CELLS], Voff_[MAX_BANANCE_CELLS];
    bool savedVon;
    uint16_t startBalanceTimeSecondsU16_;
    uint16_t balancingEnded;

    uint32_t IVtime_;
    AnalogInputs::ValueType V_[MAX_BANANCE_CELLS];

    bool isWorking()  {
        if(balance != 0)
            return true;
        //wait until the balance port voltage stabilize
        if(!AnalogInputs::isPowerOn())
            return false;
        uint16_t isOff = AnalogInputs::getFullMeasurementCount() - balancingEnded;
        return isOff < balancerStartStableCount/2;
    }

    const Strategy::VTable vtable PROGMEM = {
        powerOn,
        powerOff,
        doStrategy
    };

} // namespace Balancer

bool Balancer::isCalibrationRequired() {
    AnalogInputs::ValueType Vmin = UINT16_MAX, Vmax = 0;
    for(uint8_t i = 0; i < MAX_BANANCE_CELLS; i++) {
        if(AnalogInputs::connectedBalancePortCells & (1<<i)) {
            AnalogInputs::ValueType vi = getPresumedV(i);
            if(Vmax < vi) Vmax = vi;
            if(vi < Vmin) Vmin = vi;
        }
    }

    return Vmax - Vmin > ProgramData::battery.balancerError;
}


void Balancer::powerOn()
{
    hardware::setBalancerOutput(true);

    for(uint8_t i = 0; i < MAX_BANANCE_CELLS; i++) {
        AnalogInputs::ValueType vi = getV(i);
        Voff_[i] = Von_[i] = vi;
    }
    balance = 0;
    done = false;
    setBalance(0);
    balancingEnded = 0;
    resetMinCell();
}

uint8_t Balancer::getCellMinV()
{
    int8_t c = -1;
    AnalogInputs::ValueType vmin = UINT16_MAX;
    for(uint8_t i = 0; i < MAX_BANANCE_CELLS; i++) {
        if(AnalogInputs::connectedBalancePortCells & (1<<i)) {
            AnalogInputs::ValueType v = getV(i);
            if(vmin > v) {
                c = i;
                vmin = v;
            }
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
    if(balance == 0)
        return getV(cell);

    if(savedVon)
        return (getV(cell) + Voff_[cell]) - Von_[cell] ;
    else
        return Voff_[cell];
}

void Balancer::endBalancing()
{
    setBalance(0);
    done = true;
}


void Balancer::powerOff()
{
    endBalancing();
    hardware::setBalancerOutput(false);
}


void Balancer::setBalance(uint16_t v)
{
    if(balance != 0 && v == 0)
        balancingEnded = AnalogInputs::getFullMeasurementCount();

    balance = v;
    AnalogInputs::resetStable();
    if(!done)
        hardware::setBalancer(v);
}

void Balancer::startBalacing()
{
    //test if battery has recovered after last balancing
    if(!isStable(balancerStartStableCount) || !AnalogInputs::isOutStable())
        return;

    if(minCell < 0) {
        minCell = getCellMinV();
    }
    AnalogInputs::ValueType vmin = getV(minCell);

    //test if we can still discharge
    bool off = true;
    AnalogInputs::ValueType VdisMin =  ProgramData::battery.Vd_per_cell;
    for(uint8_t i = 0; i < MAX_BANANCE_CELLS; i++) {
        if(AnalogInputs::connectedBalancePortCells & (1<<i)) {
            //save voltage values
            AnalogInputs::ValueType v = getV(i);
            if(v < VdisMin) {
                off = true;
                break;
            }
            if(v > vmin) {
                off = false;
            }
            Von_[i] = Voff_[i] = v;
        }
    }
    LogDebug("off:", off);

    savedVon = false;
    startBalanceTimeSecondsU16_ = Time::getSecondsU16();
    if(off) {
        endBalancing();
    } else {
        setBalance(calculateBalance());
    }
}

uint16_t Balancer::calculateBalance()
{
    if(minCell < 0) {
        return 0;
    }
    AnalogInputs::ValueType vmin = getPresumedV(minCell);
    uint16_t retu = 0, cell = 1;
    for(uint8_t c = 0; c < MAX_BANANCE_CELLS; c++) {
        if(AnalogInputs::connectedBalancePortCells & cell) {
            AnalogInputs::ValueType v = getPresumedV(c);
            if(v > vmin) {
                retu |= cell;
            }
        }
        cell <<= 1;
    }
    return retu;
}

bool Balancer::isStable(const uint16_t stableCount)
{
    for(uint8_t c = 0; c < MAX_BANANCE_CELLS; c++) {
        if(AnalogInputs::connectedBalancePortCells & (1<<c)) {
            if(AnalogInputs::getStableCount(AnalogInputs::Name(AnalogInputs::Vb1+c)) < stableCount)
                return false;
        }
    }
    return true;
}

void Balancer::trySaveVon() {
    if(savedVon)
        return;
    savedVon = true;
    for(uint8_t c = 0; c < MAX_BANANCE_CELLS; c++) {
        Von_[c] = getV(c);
    }
}

uint16_t Balancer::getBalanceTime()
{
    return Time::diffU16(startBalanceTimeSecondsU16_, Time::getSecondsU16());
}


Strategy::statusType Balancer::doStrategy()
{
    LogDebug("minCell=", minCell, " balance=", balance, " conCells=", connectedCells);
    if(balance == 0) {
            startBalacing();
    } else {
        trySaveVon();
        if(getBalanceTime() > maxBalanceTime) {
            setBalance(0);
        }
    }
    if((!isWorking()) && done)
        return Strategy::COMPLETE;
    return Strategy::RUNNING;
}


bool Balancer::isMaxVout(AnalogInputs::ValueType maxV)
{
    for(uint8_t c = 0; c < MAX_BANANCE_CELLS; c++) {
        if(AnalogInputs::connectedBalancePortCells & (1<<c)) {
            if(getPresumedV(c) >= maxV)
                return true;
        }

    }
    return false;
}

bool Balancer::isMinVout(AnalogInputs::ValueType minV)
{
    for(uint8_t c = 0; c < MAX_BANANCE_CELLS; c++) {
        if(AnalogInputs::connectedBalancePortCells & (1<<c)) {
            if(getPresumedV(c) <= minV)
                return true;
        }
    }
    return false;
}

AnalogInputs::ValueType Balancer::calculatePerCell(AnalogInputs::ValueType v)
{
    uint8_t cells = AnalogInputs::getConnectedBalancePortCellsCount();
    if(cells == 0)
        return 0;
    return v/cells;
}

