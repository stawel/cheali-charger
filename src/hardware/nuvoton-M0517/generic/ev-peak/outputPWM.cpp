/*
    cheali-charger - open source firmware for a variety of LiPo chargers
    Copyright (C) 2016  Paweł Stawicki. All right reserved.

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

#include "LcdPrint.h"
#include "outputPWM.h"
#include "irq_priority.h"

//#define ENABLE_DEBUG
#include "debug.h"

extern "C" {
#include "M051Series.h"
}

#define PWM_GET_CNR(pwm, u32ChannelNum)  (*((__IO uint32_t *) ((((uint32_t)&((pwm)->CNR0)) + (u32ChannelNum) * 12))))


//based on http://www.nuvoton-m0.com/code/en/Init_M051/wiz/index.htm

namespace outputPWM {



void initialize(void)
{
}

void setPWM(uint8_t pin, uint32_t value)
{
}

void disablePWM(uint8_t pin)
{
}

} //namespace outputPWM


