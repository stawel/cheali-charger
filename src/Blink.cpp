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
#include "Timer.h"
#include "Blink.h"

bool Blink::getBlinkOff() const
{
    if(blinkIndex_ >= 0) {
        uint8_t mili = blinkTime_;
        mili/=getBlinkTime();
        if((mili+1)%2) return true;
    }
    return false;
}

bool Blink::getBlinkChanged() const
{
    if(blinkIndex_ >= 0) {
        uint8_t mili1 = blinkTime_-1;
        mili1/=getBlinkTime();
        uint8_t mili2 = blinkTime_;
        mili2/=getBlinkTime();
        return mili1 != mili2;
    }
    return false;
}


