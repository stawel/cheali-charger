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
    uint8_t index = getSelectedIndexOrSize(item);
    const char * str = pgm::read(&staticEditData[index].staticString);
    uint8_t dig = lcdPrint_P(str);
    if(getBlinkIndex() != item) {
        dig = LCD_COLUMNS - dig;
        cprintf::cprintf(&staticEditData[index].print, dig);
    }
}

int16_t * StaticEditMenu::getEditAddress(uint8_t item)
{
    uint8_t index = getSelectedIndexOrSize(item);
    cprintf::Data data = pgm::read(&staticEditData[index].print.data);
    int16_t * valuePtr = data.int16Ptr;
    uint8_t type = pgm::read(&staticEditData[index].print.type);
    if(type == CP_TYPE_STRING_ARRAY || type == CP_TYPE_UINT32_ARRAY) {
        cprintf::ArrayData array;
        pgm::read(array, data.arrayPtr);
        valuePtr = (int16_t*)array.indexPtr;
    }
    return valuePtr;
}

void StaticEditMenu::editItem(uint8_t item, uint8_t key)
{
    int16_t * valuePtr = getEditAddress(item);
    uint8_t index = getSelectedIndexOrSize(item);
    EditData d = pgm::read(&staticEditData[index].edit);
    if(key == BUTTON_DEC) *valuePtr -= d.step;
    if(key == BUTTON_INC) *valuePtr += d.step;
    if(*valuePtr < d.minValue) *valuePtr = d.minValue;
    if(*valuePtr > d.maxValue) *valuePtr = d.maxValue;
}

void StaticEditMenu::setSelector(uint16_t s) {
    //TODO: on selector change restore begin_ and pos_
    selector = s;
    size_ = getSelectedIndexOrSize(0xff);
    if(begin_ + pos_ >= size_ ) {
        begin_ = size_ - pos_ - 1;
    }
    LogDebug(size_);
}

uint8_t StaticEditMenu::getSelectedIndexOrSize(uint8_t item)
{
    uint8_t index = 0, size = 0;
    do {
        uint16_t condition = pgm::read(&staticEditData[index].enableCondition);
        if(condition == Last) {
            return size;
        }
        if(condition & selector) {
            if(item == 0) {
                return index;
            }
            item--;
            size++;
        }
        index++;
    } while(true);
}

