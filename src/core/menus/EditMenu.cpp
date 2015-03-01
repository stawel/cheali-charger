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
#include "EditMenu.h"
#include "Keyboard.h"

bool EditMenu::runEdit()
{
    startBlinkOff(getIndex());
    uint8_t key;
    render_ = true;
    do {
        key =  Keyboard::getPressedWithSpeed();
        if(key == BUTTON_DEC || key == BUTTON_INC) {
            editItem(getIndex(), key);
            startBlinkOn(getIndex());
            render_ = true;
        } else if(key == BUTTON_STOP || key == BUTTON_START) {
            break;
        }
        if(getBlinkChanged())
            render_ = true;
        if(render_)
            display();
        incBlinkTime();
    } while(true);

    stopBlink();
    waitRelease_ = true;
    render_ = true;
    return key == BUTTON_START;
}
