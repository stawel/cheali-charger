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
#include <inttypes.h>
#include "LcdPrint.h"
#include "Menu.h"
#include "StaticMenu.h"


StaticMenu::StaticMenu(const char * const* staticMenu):
            Menu(countElements(staticMenu)), staticMenu_(staticMenu){}


uint8_t StaticMenu::printItem(uint8_t i) {
        return lcdPrint_P(pgm::read(&staticMenu_[i]));
}
