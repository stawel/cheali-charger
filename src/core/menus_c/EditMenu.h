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


namespace EditMenu {

	struct EditMenu;
	typedef void(*EditMethod)(struct EditMenu *, int8_t, uint8_t) __reentrant;

	struct EditMenu {
		struct StaticMenu::StaticMenu staticMenu;
		EditMethod editItem;
	};

	void initialize(struct EditMenu *d, const char * const staticMenu[], EditMethod editItem);
	void setPrintMethod(struct EditMenu *d, Menu::PrintMethod printItem);
    bool runEdit(struct EditMenu *d);

    inline int8_t runSimple(struct EditMenu *d, bool2 animate = false) {
		return StaticMenu::runSimple(&d->staticMenu, animate);
	}

}

#endif /* EDITMENU_H_ */
