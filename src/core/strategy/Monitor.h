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
#ifndef MONITOR_H_
#define MONITOR_H_

#include "Strategy.h"

namespace Monitor {
    extern uint16_t etaDeltaSec;
    extern bool isBalancePortConnected;

    Strategy::statusType run();
    void doIdle();
    void powerOn();
    void powerOff();

    uint16_t getTimeSec();
    uint16_t getTotalBalanceTimeSec();
    uint16_t getTotalChargeDischargeTimeSec();
    uint16_t getTotalChargeDischargeTimeMin();
    uint16_t getETATime();

    uint8_t getChargeProcent();

    void resetAccumulatedMeasurements();


    void doSlowInterrupt();
};


#endif /* MONITOR_H_ */

