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
#ifndef DISCHARGER_H
#define DISCHARGER_H

#include "AnalogInputs.h"
#include "Timer.h"

#define DISCHARGER_UPPERBOUND_VALUE ((F_CPU / 2000000) * INTERRUPT_PERIOD_MICROSECONDS)

class Discharger {
public:
    enum STATE { DISCHARGING, DISCHARGING_COMPLETE, ERROR};
    static const AnalogInputs::Name VName = AnalogInputs::VoutBalancer;
    static const AnalogInputs::Name IName = AnalogInputs::Idischarge;

    Discharger();

    static AnalogInputs::ValueType getVout();
    static AnalogInputs::ValueType getIdischarge();

    STATE getState() const { return state_; }
    bool isPowerOn() const { return getState() == DISCHARGING; }
    bool isWorking() const { return value_ != 0; }


    uint16_t getValue() const { return value_; }
    void setValue(uint16_t value);
    void setRealValue(uint16_t I);

    void powerOn();
    void powerOff(STATE reason = DISCHARGING_COMPLETE);

    void doSlowInterrupt();

    uint16_t getDischarge() const;
    uint16_t correctValueTintern(uint16_t v);

protected:
    void finalizeValueTintern(bool force);
    STATE state_;
    uint16_t value_,valueSet_;
    bool tempcutoff_;
    uint32_t discharge_;
};


#endif //DISCHARGER_H
