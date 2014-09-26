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
#include "Options.h"
#include "LcdPrint.h"
#include "StaticMenu.h"
#include "SMPS.h"
#include "Utils.h"
#include "Calibrate.h"
#include "Screen.h"
#include "Version.h"
#include "Settings.h"
#include "ProgramData.h"
#include "eeprom.h"

const char string_o1[] PROGMEM = "settings";
const char string_o2[] PROGMEM = "calibrate";
const char string_o3[] PROGMEM = "reset default";

const char * const optionsStaticMenu[] PROGMEM =
{ string_o1,
  string_o2,
  string_o3,
  NULL
};

void Options::resetDefault()
{
    eeprom::restoreDefault(true);
}

void Options::run()
{
    StaticMenu optionsMenu(optionsStaticMenu);
    int8_t i;

    do {
        i = optionsMenu.runSimple();
        switch(i) {
        case 0: settings.edit(); break;
        case 1: Calibrate::run(); break;
        case 2: resetDefault(); break;
        }
    } while(i>=0);
}
