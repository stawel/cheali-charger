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
#include "memory.h"
#include "LcdPrint.h"
#include "Blink.h"

//#define ENABLE_DEBUG
#include "debug.h"


namespace EditMenu {

    uint16_t selector;
    EditCallBack editCallback;
    const struct StaticEditData * staticEditData_;

    void printItem(uint8_t item);
    void editItem(uint8_t item, uint8_t key);
    uint8_t getSelectedIndexOrSize(uint8_t item);

    void initialize(const struct StaticEditData * staticEditData, const EditCallBack callback) {
        staticEditData_ = staticEditData;
        selector = EDIT_MENU_ALWAYS;
        editCallback = callback;
        Menu::initialize(0);
        Menu::printMethod_ = printItem;
        Menu::editMethod_ = editItem;
        Blink::initialize();
    }



    void printItem(uint8_t item)
    {
        uint8_t index = getSelectedIndexOrSize(item);
        const char * str;
        pgm_read(str, &staticEditData_[index].staticString);
        uint8_t dig = lcdPrint_P(str);
        if(Blink::getBlinkIndex() != item) {
            dig = LCD_COLUMNS - dig - 1;
            uint8_t size;
            pgm_read(size, &staticEditData_[index].print.size);
            if(size) {
                lcdPrintSpaces(dig - size);
                dig = size;
            }
            cprintf::cprintf(&staticEditData_[index].print, dig);
        }
    }

    int16_t * getEditAddress(uint8_t item)
    {
        uint8_t index = getSelectedIndexOrSize(item);
        cprintf::Data data;
        pgm_read(data, &staticEditData_[index].print.data);
        int16_t * valuePtr = data.int16Ptr;
        uint8_t type;
        pgm_read(type, &staticEditData_[index].print.type);
        if(type == CP_TYPE_STRING_ARRAY || type == CP_TYPE_UINT32_ARRAY) {
            cprintf::ArrayData array;
            pgm_read(array, data.arrayPtr);
            valuePtr = (int16_t*)array.indexPtr;
        }
        return valuePtr;
    }

    uint16_t getEnableCondition(uint8_t item)
    {
        uint8_t index = getSelectedIndexOrSize(item);
        uint16_t enableCond;
        pgm_read(enableCond, &staticEditData_[index].enableCondition);
        return enableCond;
    }


    void editItem(uint8_t item, uint8_t key)
    {
        int16_t * valuePtr = getEditAddress(item);
        uint8_t index = getSelectedIndexOrSize(item);
        int dir = 1;
        EditData d;
        pgm_read(d, &staticEditData_[index].edit);
        if(key == BUTTON_DEC) dir = -1;

        if(d.step == CE_STEP_TYPE_SMART) {
            changeMinToMaxSmart((uint16_t*)valuePtr, dir, d.minValue, d.maxValue);
        } else if(d.step == CE_STEP_TYPE_METHOD) {
            d.editMethod(dir);
        } else {
            if(d.step == CE_STEP_TYPE_KEY_SPEED) {
                d.step = Keyboard::getSpeedFactor();
            }
            *valuePtr += dir*d.step;
            if(*valuePtr < d.minValue) *valuePtr = d.minValue;
            if(*valuePtr > d.maxValue) *valuePtr = d.maxValue;
        }

        if(editCallback) {
            editCallback((uint16_t*)valuePtr);
        }
    }

    void setSelector(uint16_t s) {
//        uint8_t currentIndex = getSelectedIndexOrSize(Menu::getIndex());
        selector = s;
        Menu::size_ = getSelectedIndexOrSize(0xff);
/*        for(uint8_t item = 0; item < Menu::size_ ; item++) {
            if(getSelectedIndexOrSize(item) == currentIndex) {
                Menu::setIndex(item);
                break;
            }
        }*/
        LogDebug(Menu::size_);
    }

    static bool predicate(uint16_t condition, uint16_t selector){
        bool display = true;
        if(condition & EDIT_MENU_MANDATORY) {
            display = selector & EDIT_MENU_MANDATORY;
            condition ^= EDIT_MENU_MANDATORY;
        }
        return display && (condition & selector);
    }

    uint8_t getSelectedIndexOrSize(uint8_t item)
    {
        uint8_t index = 0, size = 0;
        do {
            uint16_t condition;
            pgm_read(condition, &staticEditData_[index].enableCondition);
            if(condition == EDIT_MENU_LAST) {
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

}
