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

struct StaticEditMenu {
    struct EditMenu::EditMenu editMenu;
    const PROGMEM struct StaticEditData * staticEditData;
    uint16_t selector;
    EditCallBack editCallback;
};


struct StaticEditMenu menu;

    int8_t runSimple(bool animate) { return Menu::runSimple(&menu.editMenu.staticMenu.menu, animate); }
    bool runEdit() { return EditMenu::runEdit(&menu.editMenu); }


    uint8_t getSelectedIndexOrSize(uint8_t item);
    void printItem(int8_t item);
    void editItem(uint8_t item, uint8_t key);

}

void StaticEditMenu::initialize(const PROGMEM struct StaticEditData * staticEditData, const EditCallBack callback) {
        //TODO: sdcc remove
        const_char_ptr const PROGMEM *  staticMenu  = NULL;
    	EditMenu::initialize(&menu.editMenu,
    	        staticMenu
    	        , (EditMenu::EditMethod)editItem);
    	EditMenu::setPrintMethod(&menu.editMenu, (Menu::PrintMethod)printItem);
    	menu.staticEditData = staticEditData;
    	menu.selector = STATIC_EDIT_MENU_ALWAYS;
    	menu.editCallback = callback;
}


void StaticEditMenu::printItem(int8_t item)
{
    uint8_t index = getSelectedIndexOrSize(item);
    const_char_ptr str;
    uint8_t dig;
    uint8_t size;
    pgm_read(str, &menu.staticEditData[index].staticString);
    dig = lcdPrint_P(str);
    if(Blink::getBlinkIndex() != item) {
        dig = LCD_COLUMNS - dig - 1;
        pgm_read(size, &menu.staticEditData[index].print.size);
        if(size) {
            lcdPrintSpaces(dig - size);
            dig = size;
        }
        cprintf::cprintf(&menu.staticEditData[index].print, dig);

    }
}

int16_ptr StaticEditMenu::getEditAddress(uint8_t item)
{
    int16_ptr valuePtr;
    uint8_t type;
    uint8_t index = getSelectedIndexOrSize(item);
    cprintf::cprintf_data data;
    pgm_read(data, &menu.staticEditData[index].print.data);
    valuePtr = data.int16Ptr;
    pgm_read(type, &menu.staticEditData[index].print.type);
    if(type == CP_TYPE_STRING_ARRAY || type == CP_TYPE_UINT32_ARRAY) {
        cprintf::ArrayData array;
        pgm_read(array, data.arrayPtr);
        valuePtr = (int16_ptr)array.indexPtr;
    }
    return valuePtr;
}

uint16_t StaticEditMenu::getEnableCondition(uint8_t item)
{
    uint8_t index = getSelectedIndexOrSize(item);
    uint16_t enableCondition;
    pgm_read(enableCondition, &menu.staticEditData[index].enableCondition);
    return enableCondition;
}


void StaticEditMenu::editItem(uint8_t item, uint8_t key)
{
    int16_ptr valuePtr = getEditAddress(item);
    uint8_t index = getSelectedIndexOrSize(item);
    struct EditData d;
    int dir;
    pgm_read(d, &menu.staticEditData[index].edit);

    dir = 1;
    if(key == BUTTON_DEC) dir = -1;

    if(d.step == CE_STEP_TYPE_SMART) {
        changeMinToMaxSmart((uint16_ptr)valuePtr, dir, d.minValue, d.maxValue);
    } else if(d.step == CE_STEP_TYPE_METHOD) {
        d.editMethod(dir);
    } else {
         *valuePtr += dir*d.step;
         if(*valuePtr < d.minValue) *valuePtr = d.minValue;
         if(*valuePtr > d.maxValue) *valuePtr = d.maxValue;
    }

    if(menu.editCallback) {
    	menu.editCallback((uint16_ptr)valuePtr);
    }
}

void StaticEditMenu::setSelector(uint16_t s) {
	uint8_t index = Menu::getIndex(&menu.editMenu.staticMenu.menu);
    uint8_t currentIndex = getSelectedIndexOrSize(index);
    uint8_t size;
    uint8_t item;
    menu.selector = s;
    size = getSelectedIndexOrSize(0xff);
    menu.editMenu.staticMenu.menu.size_ = size;
    for(item = 0; item < size ; item++) {
        if(getSelectedIndexOrSize(item) == currentIndex) {
        	menu.editMenu.staticMenu.menu.begin_ = item - menu.editMenu.staticMenu.menu.pos_;
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
        uint16_t condition;
        pgm_read(condition, &menu.staticEditData[index].enableCondition);
        if(condition == 0) {
            return size;
        }
        if(predicate(condition, menu.selector)) {
            if(item == 0) {
                return index;
            }
            item--;
            size++;
        }
        index++;
    } while(true);
}

