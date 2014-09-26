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
#ifndef MAINMENU_H_
#define MAINMENU_H_

#include "StaticMenu.h"
#include "ProgramData.h"

class MainMenu : public StaticMenu {
public:
    MainMenu(const char * const* staticMenu,uint8_t staticMenuItems):
            StaticMenu(staticMenu){size_ += MAX_PROGRAMS;}

    virtual uint8_t printItem(uint8_t i) {
        uint8_t staticMenuSize = size_ - MAX_PROGRAMS;
        if(i < staticMenuSize) {
            return StaticMenu::printItem(i);
        } else {
            return lcdPrint_E(ProgramData::getName_E(i - staticMenuSize), PROGRAM_DATA_MAX_NAME);
        }
    }
};

#endif /* MENU_H_ */

