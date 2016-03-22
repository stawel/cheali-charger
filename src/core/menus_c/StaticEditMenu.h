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
#ifndef STATICEDITMENU_H_
#define STATICEDITMENU_H_

#include <stdint.h>
#include "EditMenu.h"
#include "cprintf.h"


#define CE_STEP_TYPE_SMART  0x7fff
#define CE_STEP_TYPE_METHOD 0x7ffe

#ifdef SDCC_COMPILER
//TODO: sdcc fix me !!!
#define STATIC_EDIT_METHOD(method) {CE_STEP_TYPE_METHOD,  {0}}
#else
#define STATIC_EDIT_METHOD(method) {CE_STEP_TYPE_METHOD,  {.editMethod=method}}
#endif

#define STATIC_EDIT_MENU_ALWAYS 0x7fff


namespace StaticEditMenu {
    static const uint16_t Mandatory = 0x8000;
    struct StaticEditMenu;
    typedef void(*EditCallBack)(struct StaticEditMenu *, uint16_t * value) __reentrant;
    typedef void(*StaticEditMethod)(int dir) __reentrant;

    struct EditData {
        int16_t step;
        union {
            struct {
                int16_t minValue;
                int16_t maxValue;
            };
            StaticEditMethod editMethod;
        };
    };

    struct StaticEditData {
        const char * staticString;
        uint16_t enableCondition;
        cprintf::PrintData print;
        struct EditData edit;
    };

    struct StaticEditMenu {
    	struct EditMenu::EditMenu editMenu;
        const struct StaticEditData * staticEditData;
        uint16_t selector;
        EditCallBack editCallback;
    };

    void initialize(struct StaticEditMenu *d, const struct StaticEditData * staticEditData, const EditCallBack callback = 0);
    int16_t * getEditAddress(struct StaticEditMenu *d, uint8_t item);
    uint16_t getEnableCondition(struct StaticEditMenu *d, uint8_t item);

    void setSelector(struct StaticEditMenu *d, uint16_t selector);
    static inline int8_t runSimple(struct StaticEditMenu *d, bool animate = false) { return Menu::runSimple(&d->editMenu.staticMenu.menu, animate); }
    inline bool runEdit(struct StaticEditMenu *d) { return EditMenu::runEdit(&d->editMenu); }
};

#endif /* STATICEDITMENU_H_ */
