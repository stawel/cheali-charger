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
#include "Utils.h"
#include "Buzzer.h"
#include "StaticEditMenu.h"
#include "ProgramData.h"
#include "ScreenStartInfo.h"
#include "cprintfhelper.h"

using namespace programDataMenu;
using namespace ProgramData;

namespace ProgramDataMenu {

/*condition bits:*/
#define COND_ALWAYS         StaticEditMenu::Always
#define COND_ClassNiXX      1
#define COND_ClassPb        2
#define COND_ClassLiXX      4
#define COND_ClassNiZn      8
#define COND_BATTERY        (COND_ClassNiXX+COND_ClassPb+COND_ClassLiXX+COND_ClassNiZn)

uint16_t getSelector() {
    STATIC_ASSERT(LAST_BATTERY_CLASS == 4);
    uint16_t result = 1<<15;
    if(battery.type != None) {
        result += 1 << getBatteryClass();
    }
    return result;
}

void changeVoltage(int dir)
{
    uint16_t step = (battery.type == Unknown) ? 50 : 1;
    changeMinToMaxStep(&battery.cells, dir, 1, getMaxCells(), step);
}

const cprintf::ArrayData batteryTypeData  PROGMEM = {batteryString, &battery.type};

/*
|static string          |when to display| how to display, see cprintf                   | how to edit |
 */
const StaticEditMenu::StaticEditData editData[] PROGMEM = {
{string_batteryType,    COND_ALWAYS,    {CP_TYPE_STRING_ARRAY,0,&batteryTypeData},      {1, 0,LAST_BATTERY_TYPE-1}},
{string_voltage,        COND_BATTERY,   CPRINTF_METHOD(Screen::StartInfo::printVoltageString), STATIC_EDIT_METHOD(changeVoltage)},
{string_capacity,       COND_BATTERY,   {CP_TYPE_CHARGE,0,&battery.C},                  {CE_STEP_TYPE_SMART, PROGRAM_DATA_MIN_CHARGE, PROGRAM_DATA_MAX_CHARGE/2}},
{string_chargeCurrent,  COND_BATTERY,   {CP_TYPE_A,0,&battery.Ic},                      {CE_STEP_TYPE_SMART, ANALOG_AMP(0.001), MAX_CHARGE_I}},
{string_dischargeCurrent,COND_BATTERY,  {CP_TYPE_A,0,&battery.Id},                      {CE_STEP_TYPE_SMART, ANALOG_VOLT(0.001), MAX_DISCHARGE_I}},
#ifdef ENABLE_TIME_LIMIT
{string_timeLimit,      COND_BATTERY,   {CP_TYPE_CHARGE_TIME,0,&battery.time},          {CE_STEP_TYPE_SMART, 0, PROGRAM_DATA_MAX_TIME}},
#endif
{NULL,                  StaticEditMenu::Last}

};

void changedCharge()
{
    ProgramData::check();
    ProgramData::battery.Ic = ProgramData::battery.C;
    if(ProgramData::isPb())
        ProgramData::battery.Ic/=4; //0.25C
    ProgramData::battery.Id = ProgramData::battery.C;
}

void editCallback(StaticEditMenu * menu, uint16_t * value) {
    if(value == &ProgramData::battery.type) {
        ProgramData::loadDefault();
        menu->setSelector(getSelector());
    } else if(value == &ProgramData::battery.C) {
        changedCharge();
    }
    ProgramData::check();
}

void run() {
    StaticEditMenu menu(editData, editCallback);
    int8_t item;

    do {
        menu.setSelector(getSelector());
        item = menu.runSimple();

        if(item < 0) break;
        ProgramData::Battery undo(ProgramData::battery);
        if(!menu.runEdit()) {
            ProgramData::battery = undo;
        } else {
            Buzzer::soundSelect();
            ProgramData::check();
        }
    } while(true);
}

} //namespace ProgramDataMenu
