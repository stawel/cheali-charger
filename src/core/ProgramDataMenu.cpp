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
        string_PDMM5,
        string_PDMM6,
        string_PDMM7,
        string_PDMM8,
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


uint8_t ProgramDataMenu::printItem(uint8_t index)
{
    StaticMenu::printItem(index);
    if(getBlinkIndex() != index) {
        switch (index) {
            case 0:    p_.printBatteryString(); break;
            case 1:    p_.printVoltageString(); break;
            case 2:    p_.printChargeString();  break;
            case 3:    p_.printIcString();      break;
            case 4:    p_.printIdString();      break;
#ifdef ENABLE_TIME_LIMIT               
            case 5:    p_.printTimeString();    break;
#endif            
        }
    }
    return 0;
}

void ProgramDataMenu::editItem(uint8_t index, uint8_t key)
{
    int dir = -1;
    if(key == BUTTON_INC) dir = 1;
//    dir *= keyboard.getSpeedFactor();

    switch(index) {
    case 0: p_.changeBattery(dir);     break;
    case 1: p_.changeVoltage(dir);     break;
    case 2: p_.changeCharge(dir);     break;
    case 3: p_.changeIc(dir);         break;
    case 4: p_.changeId(dir);         break;
#ifdef ENABLE_TIME_LIMIT      
    case 5: p_.changeTime(dir);         break;
#endif     
    }
}


void ProgramDataMenu::run() {
    int8_t index;
    do {
        index = runSimple();

        if(index < 0) return;
        switch(index) {
#ifdef ENABLE_TIME_LIMIT              
        case 6: createName(); break;
        case 7: editName(); break;
        case 8: resetName(); break;
#else
        case 5: createName(); break;
        case 6: editName(); break;
        case 7: resetName(); break;    
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

