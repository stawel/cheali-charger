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
#include "StaticEditMenu.h"
#include "Keyboard.h"
#include "memory.h"
#include "LcdPrint.h"

//#define ENABLE_DEBUG
#include "debug.h"

void StaticEditMenu::printItem(uint8_t item)
{
    uint8_t dig = lcdPrint_P(staticMenu_, item);
    if(getBlinkIndex() != item) {
        dig = LCD_COLUMNS - dig;
        cprintf::cprintf(&staticEditData[item].print, dig);
    }
}

int16_t * StaticEditMenu::getEditAddress(uint8_t item)
{
    cprintf::Data data = pgm::read(&staticEditData[item].print.data);
    int16_t * valuePtr = data.int16Ptr;
    uint8_t type = pgm::read(&staticEditData[item].print.type);
    if(type == CP_TYPE_STRING_ARRAY || type == CP_TYPE_UINT32_ARRAY) {
        cprintf::ArrayData array;
        pgm::read(array, data.arrayPtr);
        valuePtr = (int16_t*)array.indexPtr;
    }
    return valuePtr;
}

void StaticEditMenu::editItem(uint8_t item, uint8_t key) {

    int16_t * valuePtr = getEditAddress(item);
    EditData d = pgm::read(&staticEditData[item].edit);
    if(key == BUTTON_DEC) *valuePtr -= d.step;
    if(key == BUTTON_INC) *valuePtr += d.step;
    if(*valuePtr < d.minValue) *valuePtr = d.minValue;
    if(*valuePtr > d.maxValue) *valuePtr = d.maxValue;

}
