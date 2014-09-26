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

#include "StaticMenu.h"
#include "Blink.h"


class EditMenu : public StaticMenu, public Blink {
public:
    EditMenu(const char * const* staticMenu): StaticMenu(staticMenu){}
    virtual uint8_t printItem(uint8_t item) {}
    virtual void editItem(uint8_t item, uint8_t key) {}
    bool runEdit(uint8_t index);
};

#endif /* EDITMENU_H_ */
