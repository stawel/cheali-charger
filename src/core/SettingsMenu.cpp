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
#include "SettingsMenu.h"
#include "Utils.h"
#include "LcdPrint.h"
#include "Buzzer.h"

#ifdef HAS_LCD_BACKLIGHT
const char string_backlight[]   PROGMEM = "backlight:";
#endif
#ifdef HAS_FAN
const char string_fanOn[]       PROGMEM = "fan on:   ";
#endif
const char string_dischOff[]    PROGMEM = "disch off:";
const char string_externT[]     PROGMEM = "extrn T:   ";
const char string_externTCO[]   PROGMEM = "extrn TCO:";
const char string_dTdt[]        PROGMEM = "dT/dt:";
const char string_NiMHdV[]      PROGMEM = "NiMH -dV:";
const char string_NiCddV[]      PROGMEM = "NiCd -dV:";
const char string_CDcycles[]    PROGMEM = "C/D cycles:";
const char string_capCoff[]     PROGMEM = "cap COff:";


const char string_inputLow[]    PROGMEM = "input low:";
const char string_view[]        PROGMEM = "view: ";
const char string_reset[]       PROGMEM = "   reset";
const char string_save[]        PROGMEM = "    save"    ;

const char * const SettingsStaticMenu[] PROGMEM =
{
#ifdef HAS_LCD_BACKLIGHT
        string_backlight,
#endif
#ifdef HAS_FAN
        string_fanOn,
#endif
        string_dischOff,
        string_externT,
        string_externTCO,
        string_dTdt,
        string_NiMHdV,
        string_NiCddV,
        string_CDcycles,
        string_capCoff,
        string_inputLow,
        string_view,
        string_reset,
        string_save
};


SettingsMenu::SettingsMenu(const Settings &p):
        EditMenu(SettingsStaticMenu, sizeOfArray(SettingsStaticMenu)), p_(p){};


uint8_t SettingsMenu::printItem(uint8_t index)
{
    StaticMenu::printItem(index);
    if(getBlinkIndex() != index) {
        START_CASE_COUNTER;
        switch (index) {
#ifdef HAS_LCD_BACKLIGHT
            case NEXT_CASE:     lcdPrintUnsigned(p_.backlight_, 3);     break;
#endif
#ifdef HAS_FAN
            case NEXT_CASE:     printTemp(p_.fanTempOn_);               break;
#endif
            case NEXT_CASE:     printTemp(p_.dischargeTempOff_);        break;
            case NEXT_CASE:     lcdPrintYesNo(p_.externT_);             break;
            case NEXT_CASE:     printTemp(p_.externTCO_);               break;
            case NEXT_CASE:     printDeltaT(p_.deltaT_);                break;
            case NEXT_CASE:     lcdPrintDeltaV(p_.deltaV_NiMH_, 5);     break;
            case NEXT_CASE:     lcdPrintDeltaV(p_.deltaV_NiCd_, 5);     break;
            case NEXT_CASE:     lcdPrintUnsigned(p_.CDcycles_, 3);      break;
            case NEXT_CASE:     lcdPrintPercentage(p_.capCutoff_, 5);   break;
            case NEXT_CASE:    printVolt(p_.inputVoltageLow_);         break;
            case NEXT_CASE:    printViewType();                        break;
        }
    }
}

void SettingsMenu::editItem(uint8_t index, uint8_t key)
{
    int dir = -1;
    if(key == BUTTON_INC) dir = 1;
//    dir *= keyboard.getSpeedFactor();
    START_CASE_COUNTER;
    switch(index) {
#ifdef HAS_LCD_BACKLIGHT
        case NEXT_CASE:     changeBacklight(dir);                       break;
#endif
#ifdef HAS_FAN
        case NEXT_CASE:     changeTemp(p_.fanTempOn_, dir);             break;
#endif
        case NEXT_CASE:     changeTemp(p_.dischargeTempOff_, dir);      break;
        case NEXT_CASE:     changeMax(p_.externT_, dir, 1);             break;
        case NEXT_CASE:     changeTemp(p_.externTCO_,dir);              break;
        case NEXT_CASE:     changeDeltaTemp(p_.deltaT_,dir);            break;
        case NEXT_CASE:     changeMax(p_.deltaV_NiMH_, dir, 20);        break;
        case NEXT_CASE:     changeMax(p_.deltaV_NiCd_, dir, 20);        break;
        case NEXT_CASE:     change1Max(p_.CDcycles_, dir, 5);           break;
        case NEXT_CASE:     change1Max(p_.capCutoff_, dir, 250);        break;
        case NEXT_CASE:    changeVolt(p_.inputVoltageLow_, dir);       break;
        case NEXT_CASE:    changeViewType(dir);                        break;
    }
}

bool SettingsMenu::run() {
    int8_t index;
    do {
        index = runSimple();

        if(index < 0) return false;
        switch(index) {
        case sizeOfArray(SettingsStaticMenu) - 2:  //reset
            p_.setDefault();
            buzzer.soundSelect();
            break;
        case sizeOfArray(SettingsStaticMenu) - 1:  //save
            return true;
        default:
            Settings undo(p_);
            if(!runEdit(index))
                p_ = undo;
            p_.check();
            p_.apply();
            break;
        }
    } while(true);
}


void SettingsMenu::printTemp(AnalogInputs::ValueType t) {
    lcdPrintUnsigned(t/100, 3);
    lcdPrintChar('C');
}
void SettingsMenu::printVolt(AnalogInputs::ValueType v) {
    lcdPrintUnsigned(v/1000, 3);
    lcdPrintChar('V');
}


void SettingsMenu::printDeltaT(AnalogInputs::ValueType dt)
{
    lcdPrintUnsigned(dt/100, 3);
    lcdPrintChar('.');
    lcdPrintDigit((dt%100)/10);
    lcdPrint_P(PSTR("C/m"));
}


void SettingsMenu::changeBacklight(int dir) {
    changeMaxSmart(p_.backlight_, dir, 99);
    p_.apply();
}

void SettingsMenu::changeViewType(int dir)
{
    uint16_t v = p_.view_;
    if(dir < 0) p_.view_ = Screen::Normal;
    if(dir > 0) p_.view_ = Screen::Debug;
}

void SettingsMenu::printViewType() const
{
    if(p_.view_ == Screen::Normal)
        lcdPrint_P(PSTR("normal"));
    else
        lcdPrint_P(PSTR("debug"));
}

void SettingsMenu::changeTemp(AnalogInputs::ValueType &v, int step) {
    const AnalogInputs::ValueType min = ANALOG_CELCIUS(1);
    const AnalogInputs::ValueType max = ANALOG_CELCIUS(99);
    step *= ANALOG_CELCIUS(1);
    v+=step;
    if(v < min) v = min;
    if(v > max) v = max;
}
void SettingsMenu::changeDeltaTemp(AnalogInputs::ValueType &v, int step) {
    const AnalogInputs::ValueType min = ANALOG_CELCIUS(0.1);
    const AnalogInputs::ValueType max = ANALOG_CELCIUS(10);
    step *= ANALOG_CELCIUS(0.1);
    v+=step;
    if(v < min) v = min;
    if(v > max) v = max;
}

void SettingsMenu::changeVolt(AnalogInputs::ValueType &v, int step) {
    const AnalogInputs::ValueType min = ANALOG_VOLT(7);
    const AnalogInputs::ValueType max = ANALOG_VOLT(30);
    step *= ANALOG_VOLT(1);
    v+=step;
    if(v < min) v = min;
    if(v > max) v = max;
}
