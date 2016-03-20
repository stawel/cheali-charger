/*
    cheali-charger - open source firmware for a variety of LiPo chargers
    Copyright (C) 2016  Paweł Stawicki. All right reserved.

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


#include "MainMenu.h"
#include "memory.h"
#include "Options.h"
#include "ProgramMenus.h"

namespace MainMenu {

	const char string_options[] PROGMEM = "options";

	void printItem(struct Menu::Menu *d, int8_t i) {
		if(i == 0) {
			lcdPrint_P(string_options);
		} else {
			ProgramData::printProgramData(i - 1);
		}
	}
}

void MainMenu::run()
{
	struct Menu::Menu menu;
	Menu::initialize(&menu, MAX_PROGRAMS + 1, printItem);
	while(true) {
		int8_t index = Menu::runSimple(&menu);
		if(index >= 0)  {
			switch(index) {
			case 0:
				Options::run();
				break;
			default:
				ProgramMenus::selectProgram(index - 1);
			}
		}
	}
}
