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

#include "Hardware.h"
#include "Thevenin.h"
#include "Strategy.h"

class Balancer : public Strategy {
public:
    //TODO: "error" should be changeable
    const static AnalogInputs::ValueType error = 3;
    const static AnalogInputs::ValueType Ibalance = 200; //200mA
    const static uint16_t maxBalanceTime = 30; //30s
    const static uint16_t balancerStartStableCount = 10; //30s
    uint8_t cells_;
    uint8_t minCell_;
    uint16_t balance_;
    bool done_;
    int16_t Von_[MAX_BANANCE_CELLS], Voff_[MAX_BANANCE_CELLS];
    bool savedVon_;
    uint32_t startBalanceTime_;
    uint32_t startSwitchTime_;

    uint32_t IVtime_;
    AnalogInputs::ValueType V_[MAX_BANANCE_CELLS];
    uint16_t stableCount_;


    Balancer();
    virtual void powerOn();
    virtual void powerOff();
    virtual statusType doStrategy();
    void startBalacing();
    void trySaveVon();
    uint16_t getBalanceTime() const;

    uint16_t calculateBalance();
    void setBalance(uint16_t v);
    void setBalance(uint8_t port, bool v);
    uint8_t getCellMinV() const;
    uint8_t getCells() const { return cells_; }
    static AnalogInputs::ValueType getV(uint8_t cell);
    AnalogInputs::ValueType getPresumedV(uint8_t cell) const;
    AnalogInputs::ValueType getRealV(uint8_t cell) const { return getPresumedV(cell); }
    bool isWorking() const { return balance_ != 0; }

    bool isMaxVout(AnalogInputs::ValueType maxV) const;
    bool isMinVout(AnalogInputs::ValueType minV) const;
    bool isStable(const uint16_t stableCount = AnalogInputs::STABLE_MIN_VALUE) const;
    void endBalancing();

    AnalogInputs::ValueType calculatePerCell(AnalogInputs::ValueType v) const;
};

#endif /* BALANCER_H_ */
