/*
    cheali-charger - open source firmware for a variety of LiPo chargers
    Copyright (C) 2018  Paweł Stawicki. All right reserved.

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
#ifndef TIMER_1_H_
#define TIMER_1_H_

#include "HardwareConfig.h"

#define TIMER1_PRECISION 5
#define TIMER1_PRECISION_PERIOD (unsigned(TIMER1_PERIOD)<<TIMER1_PRECISION)

namespace Timer1
{
    void initialize();
    void disablePWM(char pin);
    void setPWM(char pin, unsigned int duty);
};

#endif //TIMER_1_H_
