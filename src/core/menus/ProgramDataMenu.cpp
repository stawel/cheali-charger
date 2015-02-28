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
#include "ProgramDataMenu.h"
#include "LcdPrint.h"
#include "EditName.h"
#include "Utils.h"
#include "Buzzer.h"

using namespace programDataMenu;

const char * const ProgramDataStaticMenu[] PROGMEM =
{
        string_batteryType,
        string_voltage,
        string_capacity,
        string_chargeCurrent,
        string_dischargeCurrent,
#ifdef ENABLE_TIME_LIMIT
        string_timeLimit,
#endif
        NULL
};


ProgramDataMenu::ProgramDataMenu(int programIndex):
        EditMenu(ProgramDataStaticMenu), programIndex_(programIndex){};



void ProgramDataMenu::printItem(uint8_t index)
{
    StaticMenu::printItem(index);
    if(getBlinkIndex() != index) {
        START_CASE_COUNTER;
        switch (index) {
            case NEXT_CASE:    ProgramData::printBatteryString(); break;
            case NEXT_CASE:    ProgramData::printVoltageString(); break;
            case NEXT_CASE:    ProgramData::printChargeString();  break;
            case NEXT_CASE:    ProgramData::printIcString();      break;
            case NEXT_CASE:    ProgramData::printIdString();      break;
#ifdef ENABLE_TIME_LIMIT
            case NEXT_CASE:    ProgramData::printTimeString();    break;
#endif
        }
    }
}

void ProgramDataMenu::editItem(uint8_t index, uint8_t key)
{
    int dir = -1;
    if(key == BUTTON_INC) dir = 1;

    START_CASE_COUNTER;
    switch(index) {
        case NEXT_CASE: ProgramData::changeBatteryType(dir);    break;
        case NEXT_CASE: ProgramData::changeVoltage(dir);    break;
        case NEXT_CASE: ProgramData::changeCharge(dir);     break;
        case NEXT_CASE: ProgramData::changeIc(dir);         break;
        case NEXT_CASE: ProgramData::changeId(dir);         break;
#ifdef ENABLE_TIME_LIMIT
        case NEXT_CASE: ProgramData::changeTime(dir);       break;
#endif
    }
}

void ProgramDataMenu::run() {
    int8_t index;
    do {
        index = runSimple();

        if(index < 0) return;

        ProgramData::Battery undo(ProgramData::battery);
        if(!runEdit(index)) {
            ProgramData::battery = undo;
        } else {
            Buzzer::soundSelect();
            ProgramData::check();
        }
        break;
    } while(true);
}

