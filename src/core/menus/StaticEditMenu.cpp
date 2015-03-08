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
        dig = LCD_COLUMNS - dig - 1;
        uint8_t size = pgm::read(&staticEditData[index].print.size);
        if(size) {
            lcdPrintSpaces(dig - size);
            dig = size;
        }
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
    int dir = 1;
    if(key == BUTTON_DEC) dir = -1;

    if(d.step == CE_STEP_TYPE_SMART) {
        change0ToInfSmart((uint16_t*)valuePtr, dir);
    } else if(d.step == CE_STEP_TYPE_METHOD) {
        d.editMethod(dir);
        goto callback; //TODO:: ?? rewrite
    } else {
         *valuePtr += dir*d.step;
    }
    if(*valuePtr < d.minValue) *valuePtr = d.minValue;
    if(*valuePtr > d.maxValue) *valuePtr = d.maxValue;

callback:
    if(editCallback) {
        editCallback(this, (uint16_t*)valuePtr);
    }
}

void StaticEditMenu::setSelector(uint16_t s) {
    uint8_t currentIndex = getSelectedIndexOrSize(begin_ + pos_);
    selector = s;
    size_ = getSelectedIndexOrSize(0xff);
    for(uint8_t item = 0; item < size_ ; item++) {
        if(getSelectedIndexOrSize(item) == currentIndex) {
            begin_ = item - pos_;
            break;
        }
    }
    LogDebug(size_);
}

static bool predicate(uint16_t condition, uint16_t selector){
    bool display = true;
    if(condition & StaticEditMenu::Mandatory) {
        display = selector & StaticEditMenu::Mandatory;
        condition ^= StaticEditMenu::Mandatory;
    }
    return display && (condition & selector);
}

uint8_t StaticEditMenu::getSelectedIndexOrSize(uint8_t item)
{
    uint8_t index = 0, size = 0;
    do {
        uint16_t condition = pgm::read(&staticEditData[index].enableCondition);
        if(condition == Last) {
            return size;
        }
        if(predicate(condition, selector)) {
            if(item == 0) {
                return index;
            }
            item--;
            size++;
        }
        index++;
    } while(true);
}

