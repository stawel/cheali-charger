/*
    cheali-charger - open source firmware for a variety of LiPo chargers
    Copyright (C) 2013  Tucker Kern. All right reserved.

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
// Need F_CPU to be defined. Which is done elsewhere
#include <util/delay.h>

namespace Utils
{
    // Accuracy of this is probably low but does that matter for us?
    void delayTenMicroseconds(uint16_t value)
    {
        if (value >= 100) {
            Utils::delayMilliseconds(value/100);
        } else {
            while (value--) {
                _delay_us(10);
            }
        }
        return;
    }

    void delayMicroseconds(uint16_t value)
    {
        Utils::delayTenMicroseconds(value/10);
        return;
    }
    
    void delayMilliseconds(uint16_t value)
    {
        while (value--) {
            // Delay function can't accept variables
            _delay_ms(1);
        }
        return;
    }
}
