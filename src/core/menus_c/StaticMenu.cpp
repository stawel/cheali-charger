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
#include <stdint.h>
#include "LcdPrint.h"
#include "Menu.h"
#include "StaticMenu.h"



void StaticMenu::printItem(StaticMenu::Data *md, int8_t i) {
	StaticMenu::Data * d = md;
	lcdPrint_P(d->staticMenu_, i);
}

void StaticMenu::initialize(Data *d, const char * const staticMenu[]) {
	Menu::initialize(&d->d, countElements(staticMenu), (Menu::PrintMethod) printItem);
}


