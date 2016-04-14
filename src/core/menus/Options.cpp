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
#include "SettingsMenu.h"
#include "memory.h"

using namespace options;

const char * const optionsStaticMenu[] PROGMEM = {
        string_settings,
        string_calibrate,
#ifdef ENABLE_EEPROM_RESTORE_DEFAULT
        string_resetDefault,
#endif
        NULL
};

void Options::resetDefault()
{
    eeprom::restoreDefault();
}

void Options::run()
{
    StaticMenu optionsMenu(optionsStaticMenu);
    int8_t i;

    do {
        i = optionsMenu.runSimple();
        START_CASE_COUNTER;
        switch(i) {
            case NEXT_CASE: SettingsMenu::run(); break;
#ifdef ENABLE_CALIBRATION
            case NEXT_CASE: Calibrate::run(); break;
#endif
#ifdef ENABLE_EEPROM_RESTORE_DEFAULT
            case NEXT_CASE: resetDefault(); break;
#endif
        }
    } while(i>=0);
}
