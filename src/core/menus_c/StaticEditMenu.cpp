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


namespace StaticEditMenu {
    uint8_t getSelectedIndexOrSize(struct StaticEditMenu *d, uint8_t item);
    void printItem(struct StaticEditMenu *d, int8_t item);
    void editItem(struct StaticEditMenu *d, uint8_t item, uint8_t key);

}

void StaticEditMenu::initialize(struct StaticEditMenu *d, const StaticEditData * staticEditData, const EditCallBack callback) {
    	EditMenu::initialize(&d->editMenu, NULL, (EditMenu::EditMethod)editItem);
    	EditMenu::setPrintMethod(&d->editMenu, (EditMenu::PrintMethod)printItem);
    	d->staticEditData = staticEditData;
    	d->selector = Always;
    	d->editCallback = callback;
};


void StaticEditMenu::printItem(struct StaticEditMenu *d, int8_t item)
{
    uint8_t index = getSelectedIndexOrSize(d, item);
    const char * str = pgm::read(&d->staticEditData[index].staticString);
    uint8_t dig = lcdPrint_P(str);
    if(Blink::getBlinkIndex() != item) {
        dig = LCD_COLUMNS - dig - 1;
        uint8_t size = pgm::read(&d->staticEditData[index].print.size);
        if(size) {
            lcdPrintSpaces(dig - size);
            dig = size;
        }
        cprintf::cprintf(&d->staticEditData[index].print, dig);
    }
}

int16_t * StaticEditMenu::getEditAddress(struct StaticEditMenu *d, uint8_t item)
{
    uint8_t index = getSelectedIndexOrSize(d, item);
    cprintf::Data data = pgm::read(&d->staticEditData[index].print.data);
    int16_t * valuePtr = data.int16Ptr;
    uint8_t type = pgm::read(&d->staticEditData[index].print.type);
    if(type == CP_TYPE_STRING_ARRAY || type == CP_TYPE_UINT32_ARRAY) {
        cprintf::ArrayData array;
        pgm::read(array, data.arrayPtr);
        valuePtr = (int16_t*)array.indexPtr;
    }
    return valuePtr;
}

uint16_t StaticEditMenu::getEnableCondition(struct StaticEditMenu *d, uint8_t item)
{
    uint8_t index = getSelectedIndexOrSize(d, item);
    return pgm::read(&d->staticEditData[index].enableCondition);
}


void StaticEditMenu::editItem(struct StaticEditMenu *menu, uint8_t item, uint8_t key)
{
    int16_t * valuePtr = getEditAddress(menu, item);
    uint8_t index = getSelectedIndexOrSize(menu, item);
    EditData d = pgm::read(&menu->staticEditData[index].edit);
    int dir = 1;
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

void StaticEditMenu::setSelector(struct StaticEditMenu *menu, uint16_t s) {
	uint8_t index = Menu::getIndex(&menu->editMenu.staticMenu.menu);
    uint8_t currentIndex = getSelectedIndexOrSize(menu, index);
    menu->selector = s;
    uint8_t size = getSelectedIndexOrSize(menu, 0xff);
    menu->editMenu.staticMenu.menu.size_ = size;
    for(uint8_t item = 0; item < size ; item++) {
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

uint8_t StaticEditMenu::getSelectedIndexOrSize(struct StaticEditMenu *menu, uint8_t item)
{
    uint8_t index = 0, size = 0;
    do {
        uint16_t condition = pgm::read(&menu->staticEditData[index].enableCondition);
        if(condition == Last) {
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

