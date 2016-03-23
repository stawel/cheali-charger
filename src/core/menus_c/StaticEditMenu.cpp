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
#include "cprintf.h"

//#define ENABLE_DEBUG
#include "debug.h"


namespace StaticEditMenu {
    uint8_t getSelectedIndexOrSize(StaticEditMenuPtr d, uint8_t item);
    void printItem(StaticEditMenuPtr d, int8_t item);
    void editItem(StaticEditMenuPtr d, uint8_t item, uint8_t key);

}

void StaticEditMenu::initialize(StaticEditMenuPtr d, const PROGMEM struct StaticEditData * staticEditData, const EditCallBack callback) {
        //TODO: sdcc remove
        const char * const  *  staticMenu  = NULL;
    	EditMenu::initialize(&d->editMenu,
    	        staticMenu
    	        , (EditMenu::EditMethod)editItem);
    	EditMenu::setPrintMethod(&d->editMenu, (Menu::PrintMethod)printItem);
    	d->staticEditData = staticEditData;
    	d->selector = STATIC_EDIT_MENU_ALWAYS;
    	d->editCallback = callback;
}


void StaticEditMenu::printItem(StaticEditMenuPtr d, int8_t item)
{
    uint8_t index = getSelectedIndexOrSize(d, item);
    const char * str;
    uint8_t dig;
    uint8_t size;
    pgm_read(str, &d->staticEditData[index].staticString);
    dig = lcdPrint_P(str);
    if(Blink::getBlinkIndex() != item) {
        dig = LCD_COLUMNS - dig - 1;
        pgm_read(size, &d->staticEditData[index].print.size);
        if(size) {
            lcdPrintSpaces(dig - size);
            dig = size;
        }
        cprintf::cprintf(&d->staticEditData[index].print, dig);

    }
}

int16_t * StaticEditMenu::getEditAddress(StaticEditMenuPtr d, uint8_t item)
{
    int16_t * valuePtr;
    uint8_t type;
    uint8_t index = getSelectedIndexOrSize(d, item);
    cprintf::cprintf_data data;
    pgm_read(data, &d->staticEditData[index].print.data);
    valuePtr = data.int16Ptr;
    pgm_read(type, &d->staticEditData[index].print.type);
    if(type == CP_TYPE_STRING_ARRAY || type == CP_TYPE_UINT32_ARRAY) {
        cprintf::ArrayData array;
        pgm_read(array, data.arrayPtr);
        valuePtr = (int16_t*)array.indexPtr;
    }
    return valuePtr;
}

uint16_t StaticEditMenu::getEnableCondition(StaticEditMenuPtr d, uint8_t item)
{
    uint8_t index = getSelectedIndexOrSize(d, item);
    uint16_t enableCondition;
    pgm_read(enableCondition, &d->staticEditData[index].enableCondition);
    return enableCondition;
}


void StaticEditMenu::editItem(StaticEditMenuPtr menu, uint8_t item, uint8_t key)
{
    int16_t * valuePtr = getEditAddress(menu, item);
    uint8_t index = getSelectedIndexOrSize(menu, item);
    struct EditData d;
    int dir;
    pgm_read(d, &menu->staticEditData[index].edit);

    dir = 1;
    if(key == BUTTON_DEC) dir = -1;

    if(d.step == CE_STEP_TYPE_SMART) {
        changeMinToMaxSmart((uint16_t*)valuePtr, dir, d.minValue, d.maxValue);
    } else if(d.step == CE_STEP_TYPE_METHOD) {
        d.editMethod(dir);
    } else {
         *valuePtr += dir*d.step;
         if(*valuePtr < d.minValue) *valuePtr = d.minValue;
         if(*valuePtr > d.maxValue) *valuePtr = d.maxValue;
    }

    if(menu->editCallback) {
    	menu->editCallback(menu, (uint16_t*)valuePtr);
    }
}

void StaticEditMenu::setSelector(StaticEditMenuPtr menu, uint16_t s) {
	uint8_t index = Menu::getIndex(&menu->editMenu.staticMenu.menu);
    uint8_t currentIndex = getSelectedIndexOrSize(menu, index);
    uint8_t size;
    uint8_t item;
    menu->selector = s;
    size = getSelectedIndexOrSize(menu, 0xff);
    menu->editMenu.staticMenu.menu.size_ = size;
    for(item = 0; item < size ; item++) {
        if(getSelectedIndexOrSize(menu, item) == currentIndex) {
        	menu->editMenu.staticMenu.menu.begin_ = item - menu->editMenu.staticMenu.menu.pos_;
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

uint8_t StaticEditMenu::getSelectedIndexOrSize(StaticEditMenuPtr menu, uint8_t item)
{
    uint8_t index = 0, size = 0;
    do {
        uint16_t condition;
        pgm_read(condition, &menu->staticEditData[index].enableCondition);
        if(condition == 0) {
            return size;
        }
        if(predicate(condition, menu->selector)) {
            if(item == 0) {
                return index;
            }
            item--;
            size++;
        }
        index++;
    } while(true);
}

