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
#ifndef EDITMENU_H_
#define EDITMENU_H_

#include <stdint.h>
#include "Menu.h"
#include "cprintf.h"
#include "memory.h"

#define CE_STEP_TYPE_SMART      0x7fff
#define CE_STEP_TYPE_KEY_SPEED  0x7ffd
#define CE_STEP_TYPE_METHOD     0x7ffe


#define EDIT_MENU_ALWAYS        0x7fff
#define EDIT_MENU_MANDATORY     0x8000
#define EDIT_MENU_LAST          0x0000

//TODO: rename
#define STATIC_EDIT_METHOD(method)  {CE_STEP_TYPE_METHOD, {.editMethod=method}}


namespace EditMenu {

    typedef void(*EditCallBack)(uint16_ptr value);
    //TODO: sdcc rename
    typedef void(*SimpleEditMethod)(int dir);

    //TODO: rename EditData
    struct EditData {
        int16_t step;
        union {
            struct {
                int16_t minValue;
                int16_t maxValue;
            };
            SimpleEditMethod editMethod;
        };
    };
    struct StaticEditData {
        const_char_ptr staticString;
        uint16_t enableCondition;
        struct cprintf::PrintData print;
        struct EditData edit;
    };

    void initialize(const PROGMEM_PTR struct StaticEditData * staticEditData, const EditCallBack callback);
    int16_ptr getEditAddress(uint8_t item);
    uint16_t getEnableCondition(uint8_t item);
    void setSelector(uint16_t selector);


    inline int8_t run(bool alwaysRefresh = false)   { return Menu::run(alwaysRefresh); }
    inline bool runEdit()                           { return Menu::runEdit(); }

}
#endif /* EDITMENU_H_ */
