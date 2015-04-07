/*
    cheali-charger - open source firmware for a variety of LiPo chargers
    Copyright (C) 2013  Pawel Stawicki. All right reserved.

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
#ifndef PULSESTRATEGY_H_
#define PULSESTRATEGY_H_

#include "Strategy.h"
#ifdef ENABLE_PULSE_CHARGE_STRATEGY
namespace PulseChargeStrategy {
    extern const Strategy::VTable vtable;
	enum ActionType {NONE, WORKING, PAUSE};
	void switchMode();
    void powerOn();
    void powerOff();
	bool isPause();
	Strategy::statusType tuneCurrent();
	Strategy::statusType doStrategy();
    void setPeriodAndDuty(uint8_t seconds, uint8_t duty_); // duty from 1..99%
};
#endif

#endif /* PULSESTRATEGY_H_ */
