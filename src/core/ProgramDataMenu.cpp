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

const char string_PDMM0[] PROGMEM = "Bat:  ";
const char string_PDMM1[] PROGMEM = "V:  ";
const char string_PDMM2[] PROGMEM = "Ch: ";
const char string_PDMM3[] PROGMEM = "Ic: ";
const char string_PDMM4[] PROGMEM = "Id: ";
const char string_PDMM5[] PROGMEM = "Tlim: ";
const char string_PDMM6[] PROGMEM = "Create name";
const char string_PDMM7[] PROGMEM = "Edit name";
const char string_PDMM8[] PROGMEM = "Reset name";

const char * const ProgramDataStaticMenu[] PROGMEM =
{
        string_PDMM0,
        string_PDMM1,
        string_PDMM2,
        string_PDMM3,
        string_PDMM4,
#ifdef ENABLE_TIME_LIMIT
        string_PDMM5,
#endif
        string_PDMM6,
#ifdef ENABLE_PROGRAM_MENU_EDIT_NAME
        string_PDMM7,
#endif
#ifdef ENABLE_PROGRAM_MENU_RESET_NAME
        string_PDMM8,
#endif
        NULL
};


ProgramDataMenu::ProgramDataMenu(const ProgramData &p, int programIndex):
        EditMenu(ProgramDataStaticMenu), p_(p), programIndex_(programIndex){};


void ProgramDataMenu::editName()
{
    EditName editName(p_.name, PROGRAM_DATA_MAX_NAME, PSTR("Edit name:"));
    editName.run();
    render();
}

void ProgramDataMenu::createName()
{
    p_.createName(programIndex_+1);
    waitName();
}

void ProgramDataMenu::resetName()
{
    p_.resetName(programIndex_+1);
    waitName();
}
void ProgramDataMenu::waitName()
{
    lcdClear();
    lcdSetCursor0_0();
    lcdPrint_P(PSTR("Name:"));
    lcdSetCursor0_1();
    lcdPrint(p_.name, PROGRAM_DATA_MAX_NAME);
    waitButtonPressed();
    Buzzer::soundSelect();
}


void ProgramDataMenu::printItem(uint8_t index)
{
    StaticMenu::printItem(index);
    if(getBlinkIndex() != index) {
        START_CASE_COUNTER;
        switch (index) {
            case NEXT_CASE:    p_.printBatteryString(); break;
            case NEXT_CASE:    p_.printVoltageString(); break;
            case NEXT_CASE:    p_.printChargeString();  break;
            case NEXT_CASE:    p_.printIcString();      break;
            case NEXT_CASE:    p_.printIdString();      break;
#ifdef ENABLE_TIME_LIMIT
            case NEXT_CASE:    p_.printTimeString();    break;
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
        case NEXT_CASE: p_.changeBattery(dir);    break;
        case NEXT_CASE: p_.changeVoltage(dir);    break;
        case NEXT_CASE: p_.changeCharge(dir);     break;
        case NEXT_CASE: p_.changeIc(dir);         break;
        case NEXT_CASE: p_.changeId(dir);         break;
#ifdef ENABLE_TIME_LIMIT
        case NEXT_CASE: p_.changeTime(dir);       break;
#endif
    }
}

void ProgramDataMenu::run() {
    int8_t index;
    do {
        index = runSimple();

        if(index < 0) return;

        START_CASE_COUNTER_FROM(sizeOfArray(ProgramDataStaticMenu)
#ifdef ENABLE_PROGRAM_MENU_EDIT_NAME
                  -1
#endif
#ifdef ENABLE_PROGRAM_MENU_RESET_NAME
                  -1
#endif
                  -1 -1);
        switch(index) {
            case NEXT_CASE: createName(); break;
#ifdef ENABLE_PROGRAM_MENU_EDIT_NAME
            case NEXT_CASE: editName();   break;
#endif
#ifdef ENABLE_PROGRAM_MENU_RESET_NAME
            case NEXT_CASE: resetName();  break;
#endif
        default:
            ProgramData undo(p_);
            if(!runEdit(index)) {
                p_ = undo;
            } else {
                Buzzer::soundSelect();
                p_.check();
            }
            break;
        }
    } while(true);
}

