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
#ifndef BALANCER_H_
#define BALANCER_H_

#if MAX_BANANCE_CELLS > 6
#define BALANCER_PORTS_GT_6(x...) x
#else
#define BALANCER_PORTS_GT_6(x...)
#endif

#ifndef BALANCER_I
#define BALANCER_I ANALOG_AMP(0.160) //default 160mA
#endif


#include "Strategy.h"

namespace Balancer {

    const static uint16_t maxBalanceTime = 15; //15s
    const static uint16_t balancerStartStableCount = 6; //6*0.7s

    extern const Strategy::VTable vtable;

    extern uint16_t balance;
    extern bool savedVon;
    extern int8_t minCell;
    extern bool done;
    extern uint16_t balancingEnded;

    void powerOn();
    void powerOff();
    Strategy::statusType doStrategy();
    void startBalacing();
    void trySaveVon();
    uint16_t getBalanceTime();

    uint16_t calculateBalance();
    void setBalance(uint16_t v);
    uint8_t getCellMinV();

    AnalogInputs::ValueType getV(uint8_t cell);
    AnalogInputs::ValueType getPresumedV(uint8_t cell);
    inline AnalogInputs::ValueType getRealV(uint8_t cell) { return getPresumedV(cell); }
    inline void resetMinCell() { minCell = -1; }
    bool isWorking();

    bool isMaxVout(AnalogInputs::ValueType maxV);
    bool isMinVout(AnalogInputs::ValueType minV);
    bool isStable(const uint16_t stableCount = AnalogInputs::STABLE_MIN_VALUE);
    bool isCalibrationRequired();
    void endBalancing();

    AnalogInputs::ValueType calculatePerCell(AnalogInputs::ValueType v);
};

#endif /* BALANCER_H_ */
