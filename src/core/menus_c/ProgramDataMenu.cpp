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
#include "Buzzer.h"
#include "EditMenu.h"
#include "ProgramData.h"
#include "ScreenStartInfo.h"
#include "memory.h"

using namespace ProgramData;

namespace ProgramDataMenu {

/*condition bits:*/
#define COND_ALWAYS         EDIT_MENU_ALWAYS
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


const AnalogInputs::ValueType Tmin = (SETTINGS_FAN_TEMPERATURE_DIFF/ANALOG_CELCIUS(1))*ANALOG_CELCIUS(1) + ANALOG_CELCIUS(1);
const AnalogInputs::ValueType Tmax = ANALOG_CELCIUS(99);
const AnalogInputs::ValueType Tstep =  ANALOG_CELCIUS(1);


#define BATTERY_N(type, n, x)   {CP_TYPE_ ## type, n, {&battery.x}}
#define BATTERY(type, x)        {CP_TYPE_ ## type, 0, {&battery.x}}

/*
|static string          |when to display    | how to display, see cprintf                   | how to edit |
 */
const EditMenu::StaticEditData editData[] PROGMEM = {
{string_batteryType,    COND_ALWAYS,        EDIT_STRING_ARRAY(batteryTypeData),     {1, 0,LAST_BATTERY_TYPE-1}},
{string_voltage,        COND_BATT_UNKN,     CPRINTF_METHOD(Screen::StartInfo::printVoltageString), STATIC_EDIT_METHOD(changeVoltage)},
{string_Vc_per_cell,    ADV(LiXX_NiZn_Pb),  BATTERY(V, Vc_per_cell),                {CE_STEP_TYPE_KEY_SPEED,ANALOG_VOLT(0.0),ANALOG_VOLT(5.0)}},
{string_Vc_per_cell,    COND_Unknown,       BATTERY(V, Vc_per_cell),                {50,ANALOG_VOLT(0.0),MAX_CHARGE_V}},
{string_Vcutoff,        ADV(NiXX),          BATTERY(V, Vc_per_cell),                {CE_STEP_TYPE_KEY_SPEED, ANALOG_VOLT(1.200), ANALOG_VOLT(2.000)}},
{string_Vcutoff,        COND_LED,           BATTERY(V, Vc_per_cell),                {CE_STEP_TYPE_SMART, ANALOG_VOLT(0.001), MAX_CHARGE_V}},
{string_Vs_per_cell,    ADV(LiXX),          BATTERY(V, Vs_per_cell),                {CE_STEP_TYPE_KEY_SPEED,ANALOG_VOLT(0.0),ANALOG_VOLT(5.0)}},
{string_Vd_per_cell,    COND_BATT_UNKN+COND_advanced, BATTERY(V, Vd_per_cell),      {CE_STEP_TYPE_KEY_SPEED,ANALOG_VOLT(0.0),ANALOG_VOLT(5.0)}},
{string_Vd_per_cell,    COND_Unknown,       BATTERY(V, Vd_per_cell),                {50,ANALOG_VOLT(0.0),MAX_CHARGE_V}},
{string_capacity,       COND_BATTERY,       BATTERY(CHARGE, capacity),              {CE_STEP_TYPE_SMART, ANALOG_MIN_CHARGE, (int16_t)ANALOG_MAX_CHARGE}},
{string_Ic,             COND_BATTERY+COND_LED, BATTERY(A, Ic),                      {CE_STEP_TYPE_SMART, ANALOG_AMP(0.001), MAX_CHARGE_I}},
{string_minIc,          ADV(LiXX_NiZn_Pb_Unkn), BATTERY(A, minIc),                  {CE_STEP_TYPE_SMART, ANALOG_AMP(0.001), MAX_CHARGE_I}},
{string_Id,             COND_BATTERY,       BATTERY(A, Id),                         {CE_STEP_TYPE_SMART, ANALOG_VOLT(0.001), MAX_DISCHARGE_I}},
{string_minId,          ADV(BATTERY),       BATTERY(A, minId),                      {CE_STEP_TYPE_SMART, ANALOG_VOLT(0.001), MAX_DISCHARGE_I}},
{string_balancErr,      ADV(LiXX_NiZn),     BATTERY(SIGNED_mV, balancerError),      {ANALOG_VOLT(0.001), ANALOG_VOLT(0.003), ANALOG_VOLT(0.200)}},

{string_enabledV,       COND_NiXX,          BATTERY(ON_OFF, enable_deltaV),         {1, 0, 1}},
{string_deltaV,         COND_enable_dV,     BATTERY(SIGNED_mV, deltaV),             {ANALOG_VOLT(0.001), -ANALOG_VOLT(0.020), ANALOG_VOLT(0.000)}},
{string_ignoreFirst,    COND_enable_dV,     BATTERY(MINUTES, deltaVIgnoreTime),     {1, 1, 10}},

{string_externT,        COND_BATTERY,       BATTERY(ON_OFF, enable_externT),        {1, 0, 1}},
{string_dTdt,           COND_enable_dT,     BATTERY_N(TEMP_MINUT, 6, deltaT),       {ANALOG_CELCIUS(0.1), ANALOG_CELCIUS(0.1), ANALOG_CELCIUS(9)}},
{string_externTCO,      COND_enableT,       BATTERY_N(TEMPERATURE, 3, externTCO),   {Tstep, Tmin, Tmax}},

{string_timeLimit,      COND_BATTERY+COND_LED, BATTERY(CHARGE_TIME, time),          {CE_STEP_TYPE_SMART, 0, ANALOG_MAX_TIME_LIMIT}},
{string_capCoff,        COND_BATTERY,       BATTERY(PROCENTAGE, capCutoff),         {1, 1, 250}},
{string_DCcycles,       COND_NiXX_Pb,       BATTERY(UNSIGNED, DCcycles),            {1, 0, 5}},
{string_DCRestTime,     ADV(BATTERY),       BATTERY(MINUTES, DCRestTime),           {1, 1, 99}},
{string_adaptiveDis,    ADV(BATTERY),       BATTERY(ON_OFF, enable_adaptiveDischarge),{1, 0, 1}},


{NULL,                  EDIT_MENU_LAST}

};

    void editCallback(uint16_t * value) {
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
        EditMenu::setSelector(getSelector());
    }

    void run() {
        int8_t item;
        EditMenu::initialize(editData, editCallback);

        do {
            EditMenu::setSelector(getSelector());
            item = EditMenu::run();

            if(item < 0) break;
            ProgramData::Battery undo(ProgramData::battery);
            if(!EditMenu::runEdit()) {
                ProgramData::battery = undo;
            } else {
                Buzzer::soundSelect();
                ProgramData::check();
            }
        } while(true);
    }

} //namespace ProgramDataMenu

#undef COND_ALWAYS //needed when all files are packed into one cpp source
