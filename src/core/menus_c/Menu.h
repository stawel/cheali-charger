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
#ifndef MENU_H_
#define MENU_H_

#include <stdint.h>
#include "PolarityCheck.h"
#include "memory.h"

namespace Menu {
	static const int8_t MENU_EXIT = -1;
	struct Menu;
	typedef void(*PrintMethod)(struct Menu *, int8_t) __reentrant;

	struct Menu {
	    uint8_t pos_;
	    uint8_t begin_;
	    uint8_t size_;
	    bool render_;
	    bool waitRelease_;
	    PrintMethod printItem;
	};

	void initialize(struct Menu *d, uint8_t size, PrintMethod printItem);
	uint8_t run(struct Menu *d);
    int8_t runSimple(struct Menu *d, bool animate = false);
    uint8_t getIndex(struct Menu *d);
    void display(struct Menu *d);
}


#endif /* MENU_H_ */

