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

using namespace ProgramData;

namespace ProgramDataMenu {

/*condition bits:*/
#define COND_ALWAYS         StaticEditMenu::Always
#define COND_NiXX           1
#define COND_Pb             2
#define COND_LiXX           4
#define COND_NiZn           8
#define COND_Unknown        16
#define COND_LED            32

#define COND_enableT        256
#define COND_enable_dV      512
#define COND_enable_dT      1024
#define COND_advanced       32768
#define ADV(x)              (COND_advanced + COND_ ## x)

#define COND_LiXX_NiZn              (COND_LiXX + COND_NiZn)
#define COND_LiXX_NiZn_Pb           (COND_LiXX + COND_NiZn + COND_Pb)
#define COND_LiXX_NiZn_Pb_Unkn      (COND_LiXX + COND_NiZn + COND_Pb + COND_Unknown)
#define COND_NiXX_Pb                (COND_NiXX + COND_Pb)

#define COND_BATTERY                (COND_NiXX + COND_Pb + COND_LiXX + COND_NiZn + COND_Unknown)
#define COND_BATT_UNKN              (COND_NiXX + COND_Pb + COND_LiXX + COND_NiZn)

uint16_t getSelector() {
    STATIC_ASSERT(LAST_BATTERY_CLASS == 6);
    uint16_t result = 1<<14;
    if(battery.type != None) {
        result += 1 << getBatteryClass();

        if(battery.enable_externT) {
            result += COND_enableT;
            if(isNiXX()) {
                result += COND_enable_dT;
            }
        }
        if(isNiXX() && battery.enable_deltaV) {
            result += COND_enable_dV;
        }
        if(settings.menuType) {
            result += COND_advanced;
        }
    }
    return result;
}

void changeVoltage(int dir)
{
    changeMinToMaxStep(&battery.cells, dir, 1, getMaxCells(), 1);
}

const cprintf::ArrayData batteryTypeData  PROGMEM = {batteryString, &battery.type};


const AnalogInputs::ValueType Tmin = (Settings::TempDifference/ANALOG_CELCIUS(1))*ANALOG_CELCIUS(1) + ANALOG_CELCIUS(1);
const AnalogInputs::ValueType Tmax = ANALOG_CELCIUS(99);
const AnalogInputs::ValueType Tstep =  ANALOG_CELCIUS(1);
/*
|static string          |when to display    | how to display, see cprintf                   | how to edit |
 */
const StaticEditMenu::StaticEditData editData[] PROGMEM = {
{string_batteryType,    COND_ALWAYS,        {CP_TYPE_STRING_ARRAY,0,&batteryTypeData},      {1, 0,LAST_BATTERY_TYPE-1}},
{string_voltage,        COND_BATT_UNKN,     CPRINTF_METHOD(Screen::StartInfo::printVoltageString), STATIC_EDIT_METHOD(changeVoltage)},
{string_Vc_per_cell,    ADV(LiXX_NiZn_Pb),  {CP_TYPE_V,0,&battery.Vc_per_cell},             {1,ANALOG_VOLT(0.0),ANALOG_VOLT(5.0)}},
{string_Vc_per_cell,    COND_Unknown,       {CP_TYPE_V,0,&battery.Vc_per_cell},             {50,ANALOG_VOLT(0.0),MAX_CHARGE_V}},
{string_Vcutoff,        ADV(NiXX),          {CP_TYPE_V,0,&battery.Vc_per_cell},             {ANALOG_VOLT(0.001), ANALOG_VOLT(1.200), ANALOG_VOLT(2.000)}},
{string_Vcutoff,        COND_LED,           {CP_TYPE_V,0,&battery.Vc_per_cell},             {CE_STEP_TYPE_SMART, ANALOG_VOLT(0.001), MAX_CHARGE_V}},
{string_Vs_per_cell,    ADV(LiXX),          {CP_TYPE_V,0,&battery.Vs_per_cell},             {1,ANALOG_VOLT(0.0),ANALOG_VOLT(5.0)}},
{string_Vd_per_cell,    COND_BATT_UNKN+COND_advanced,{CP_TYPE_V,0,&battery.Vd_per_cell},    {1,ANALOG_VOLT(0.0),ANALOG_VOLT(5.0)}},
{string_Vd_per_cell,    COND_Unknown,       {CP_TYPE_V,0,&battery.Vd_per_cell},             {50,ANALOG_VOLT(0.0),MAX_CHARGE_V}},
{string_capacity,       COND_BATTERY,       {CP_TYPE_CHARGE,0,&battery.capacity},           {CE_STEP_TYPE_SMART, ANALOG_MIN_CHARGE, ANALOG_MAX_CHARGE/2}},
{string_Ic,             COND_BATTERY+COND_LED,{CP_TYPE_A,0,&battery.Ic},                    {CE_STEP_TYPE_SMART, ANALOG_AMP(0.001), MAX_CHARGE_I}},
{string_minIc,          ADV(LiXX_NiZn_Pb_Unkn),{CP_TYPE_A,0,&battery.minIc},           {CE_STEP_TYPE_SMART, ANALOG_AMP(0.001), MAX_CHARGE_I}},
{string_Id,             COND_BATTERY,       {CP_TYPE_A,0,&battery.Id},                      {CE_STEP_TYPE_SMART, ANALOG_VOLT(0.001), MAX_DISCHARGE_I}},
{string_minId,          ADV(BATTERY),       {CP_TYPE_A,0,&battery.minId},                   {CE_STEP_TYPE_SMART, ANALOG_VOLT(0.001), MAX_DISCHARGE_I}},
{string_balancErr,      ADV(LiXX_NiZn),     {CP_TYPE_SIGNED_mV,0,&battery.balancerError},   {ANALOG_VOLT(0.001), ANALOG_VOLT(0.003), ANALOG_VOLT(0.200)}},

{string_enabledV,       COND_NiXX,          {CP_TYPE_ON_OFF,0,&battery.enable_deltaV},      {1, 0, 1}},
{string_deltaV,         COND_enable_dV,     {CP_TYPE_SIGNED_mV,0,&battery.deltaV},          {ANALOG_VOLT(0.001), -ANALOG_VOLT(0.020), ANALOG_VOLT(0.000)}},
{string_ignoreFirst,    COND_enable_dV,     {CP_TYPE_MINUTES,0,&battery.deltaVIgnoreTime},  {1, 1, 10}},

{string_externT,        COND_BATTERY,       {CP_TYPE_ON_OFF,0,&battery.enable_externT},     {1, 0, 1}},
{string_dTdt,           COND_enable_dT,     {CP_TYPE_TEMP_MINUT,6,&battery.deltaT},         {ANALOG_CELCIUS(0.1), ANALOG_CELCIUS(0.1), ANALOG_CELCIUS(9)}},
{string_externTCO,      COND_enableT,       {CP_TYPE_TEMPERATURE,3,&battery.externTCO},     {Tstep, Tmin, Tmax}},

{string_timeLimit,      COND_BATTERY+COND_LED,{CP_TYPE_CHARGE_TIME,0,&battery.time},        {CE_STEP_TYPE_SMART, 0, ANALOG_MAX_TIME_LIMIT}},
{string_capCoff,        COND_BATTERY,       {CP_TYPE_PROCENTAGE,0,&battery.capCutoff},      {1, 1, 250}},
{string_DCcycles,       COND_NiXX_Pb,       {CP_TYPE_UNSIGNED,0,&battery.DCcycles},         {1, 0, 5}},
{string_DCRestTime,     ADV(BATTERY),       {CP_TYPE_MINUTES,0,&battery.DCRestTime},        {1, 1, 99}},
{string_adaptiveDis,    ADV(BATTERY),       {CP_TYPE_ON_OFF,0,&battery.enable_adaptiveDischarge},{1, 0, 1}},


{NULL,                  StaticEditMenu::Last}

};

void editCallback(StaticEditMenu * menu, uint16_t * value) {
    if(value == &ProgramData::battery.type) {
        ProgramData::changedType();
    } else if(value == &ProgramData::battery.capacity) {
        ProgramData::changedCapacity();
    } else if(value == &ProgramData::battery.Ic) {
        ProgramData::changedIc();
    } else if(value == &ProgramData::battery.Id) {
        ProgramData::changedId();
    }
    ProgramData::check();
    menu->setSelector(getSelector());
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

#undef COND_ALWAYS