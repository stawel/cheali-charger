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
#include "OptionsMenu.h"
#include "Menu.h"
#include "Calibrate.h"
#include "SettingsMenu.h"
#include "Hardware.h"
#include "eeprom.h"
#include "memory.h"

const PROGMEM struct Menu::StaticMenu optionsStaticMenu[] = {
        {string_settings,       SettingsMenu::run },
#ifdef ENABLE_CALIBRATION
        {string_calibrate,      Calibrate::run  },
#endif
#ifdef ENABLE_EEPROM_RESTORE_DEFAULT
        {string_resetDefault,   OptionsMenu::resetDefault },
#endif
        {NULL, NULL}
};

#ifdef ENABLE_EEPROM_RESTORE_DEFAULT
void OptionsMenu::resetDefault()
{
    eeprom::restoreDefault();
}
#endif

void OptionsMenu::run()
{
    int8_t i;
    do {
        Menu::initializeStatic(optionsStaticMenu);
        i = Menu::runStatic();
    } while(i >= 0);
}
