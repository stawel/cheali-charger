/*
    cheali-charger - open source firmware for a variety of LiPo chargers
    Copyright (C) 2014 Paweł Stawicki. All right reserved.

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

#include "Utils.h"
#include "clk.h"

namespace Utils
{
    void Delay(uint32_t x)
    {
        asm volatile(
         ".syntax unified;"
         "ldr r3, %[in];"
         "loop:; "
         "subs r3, #1;"
         "bne loop;"
         ".syntax divided;"
         :
         :[in] "m" (x)
         :"r3","cc","memory"
        );
    }

    //This method should be only used by the LCD
    void delayMicroseconds(uint16_t value)
    {
        uint32_t x = value;
        x*=26982;
        x/=4096;
        Delay(x);
    }
}
