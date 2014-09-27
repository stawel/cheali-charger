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
    //TODO: implement, used by LCD
	//x = 10000 == 2592micro second
	void Delay(uint32_t x)
   	{
	volatile uint32_t i;
		for(i = 0; i < x; i++) {
			__NOP();
			__NOP();
		}
	}

    void delayMicroseconds(uint16_t value)
    {
    	//CLK_SysTickDelay(4*((uint32_t)value));

    	uint32_t x = value;
    	x*=10000;
    	x/=2592;
    	Delay(x);
     }
}
