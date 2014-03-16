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
#ifndef TIMER_H_
#define TIMER_H_

#include "Hardware.h"

#define TIMER_INTERRUPT_PERIOD_MICROSECONDS     500
#define TIMER_SLOW_INTERRUPT_INTERVAL           225
#define SLOW_INTERRUPT_PERIOD_MILISECONDS ((long)TIMER_INTERRUPT_PERIOD_MICROSECONDS*TIMER_SLOW_INTERRUPT_INTERVAL/1000)

namespace Timer {
    void initialize();
    uint32_t getInterrupts();
    uint32_t getMiliseconds();
    void delay(uint16_t ms);
    void delayIdle(uint16_t ms);

    //private
    void callback();
};


#endif /* TIMER_H_ */
