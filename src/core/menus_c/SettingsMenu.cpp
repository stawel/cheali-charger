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
#include "Hardware.h"
#include "SettingsMenu.h"
#include "Utils.h"
#include "LcdPrint.h"
#include "Buzzer.h"
#include "SerialLog.h"
#include "StaticEditMenu.h"
#include "memory.h"

namespace SettingsMenu {


const PROGMEM char * const SettingsFanOn[] = {
        string_disable,
        string_always,
        string_FanProgram,
        string_temperature,
        string_tempProgram
};
const PROGMEM cprintf::ArrayData FanOnData = {{SettingsFanOn}, &settings.fanOn};

const  PROGMEM char * const SettingsUART[] = {
        string_disable,
        string_normal,
        string_debug,
        string_extDebug,
        string_extDebugAdc
};
const PROGMEM cprintf::ArrayData UARTData  = {{SettingsUART}, &settings.UART};
const PROGMEM cprintf::ArrayData UARTSpeedsData = {{Settings::UARTSpeedValue}, &settings.UARTspeed};


const PROGMEM char * const SettingsUARToutput[] = {string_temp, string_pin7, string_pin38};
const PROGMEM cprintf::ArrayData UARToutputData  = {{SettingsUARToutput}, &settings.UARToutput};


const PROGMEM char * const SettingsMenuType[] = {string_simple, string_advanced};
const PROGMEM cprintf::ArrayData menuTypeData  = {{SettingsMenuType}, &settings.menuType};

#define Tmin    ((Settings::TempDifference/ANALOG_CELCIUS(1) + 1)*ANALOG_CELCIUS(1))
#define Tmax    ANALOG_CELCIUS(99)
#define Tstep   ANALOG_CELCIUS(1)


/*condition bits:*/
#define COND_FAN_ON_T       1
#define COND_UART_ON        2
#define COND_ALWAYS         STATIC_EDIT_MENU_ALWAYS

uint16_t getSelector() {
    uint16_t result = STATIC_EDIT_MENU_ALWAYS;
#ifdef ENABLE_FAN
    if(settings.fanOn != Settings::FanProgramTemperature && settings.fanOn != Settings::FanTemperature)
        result -= COND_FAN_ON_T;
#endif
    if(settings.UART == Settings::Disabled)
        result -= COND_UART_ON;

    return result;
}

/*
|static string          |when to display| how to display, see cprintf                   | how to edit |
 */
const PROGMEM struct StaticEditMenu::StaticEditData editData[] = {
#ifdef ENABLE_LCD_BACKLIGHT
{string_backlight,      COND_ALWAYS,    {CP_TYPE_UNSIGNED,0,{&settings.backlight}},       {1, 0, 100}},
#endif
#ifdef ENABLE_FAN
//{string_fanOn,          COND_ALWAYS,    {CP_TYPE_STRING_ARRAY,0,{&FanOnData}},            {1, 0, Settings::FanProgramTemperature}},
{string_fanTempOn,      COND_FAN_ON_T,  {CP_TYPE_TEMPERATURE,0,{&settings.fanTempOn}},    {Tstep, Tmin, Tmax}},
#endif
#ifdef ENABLE_T_INTERNAL
{string_dischOff,       COND_ALWAYS,    {CP_TYPE_TEMPERATURE,3,{&settings.dischargeTempOff}}, {Tstep, Tmin, Tmax}},
#endif
{string_AudioBeep,      COND_ALWAYS,    {CP_TYPE_ON_OFF,0,{&settings.audioBeep}},         {1, 0, 1}},
{string_minIc,          COND_ALWAYS,    {CP_TYPE_A,0,{&settings.minIc}},                  {ANALOG_AMP(0.001), ANALOG_AMP(0.001), ANALOG_AMP(0.500)}},
{string_maxIc,          COND_ALWAYS,    {CP_TYPE_A,0,{&settings.maxIc}},                  {CE_STEP_TYPE_SMART, ANALOG_AMP(0.001), MAX_CHARGE_I}},
{string_minId,          COND_ALWAYS,    {CP_TYPE_A,0,{&settings.minId}},                  {ANALOG_AMP(0.001), ANALOG_AMP(0.001), ANALOG_AMP(0.500)}},
{string_maxId,          COND_ALWAYS,    {CP_TYPE_A,0,{&settings.maxId}},                  {CE_STEP_TYPE_SMART, ANALOG_AMP(0.001), MAX_DISCHARGE_I}},
{string_inputLow,       COND_ALWAYS,    {CP_TYPE_V,3,{&settings.inputVoltageLow}},        {ANALOG_VOLT(1), ANALOG_VOLT(7), ANALOG_VOLT(30)}},
#ifdef ENABLE_ANALOG_INPUTS_ADC_NOISE
{string_adcNoise,       COND_ALWAYS,    {CP_TYPE_ON_OFF,0,{&settings.adcNoise}},          {1, 0, 1}},
#endif
//TODO: sdcc fix
//{string_UARTview,       COND_ALWAYS,    {CP_TYPE_STRING_ARRAY,0,&UARTData},             {1, 0, Settings::ExtDebugAdc}},
//{string_UARTspeed,      COND_UART_ON,   {CP_TYPE_UINT32_ARRAY,0,{&UARTSpeedsData}},       {1, 0, SETTINGS_UART_SPEEDS_COUNT-1}},
#ifdef ENABLE_TX_HW_SERIAL
//{string_UARToutput,     COND_UART_ON,   {CP_TYPE_STRING_ARRAY,0,{&UARToutputData}},        {1, 0, 2}},
#endif
//{string_MenuType,       COND_ALWAYS,    {CP_TYPE_STRING_ARRAY,0,{&menuTypeData}},         {1, 0, 1}},

#ifdef ENABLE_SETTINGS_MENU_RESET
//{string_reset,          STATIC_EDIT_MENU_ALWAYS, {0,0,NULL}},
#endif
{NULL,                  0}
};

void editCallback(struct StaticEditMenu::StaticEditMenu * menu, uint16_t * adr) {
	StaticEditMenu::setSelector(menu, getSelector());
    Settings::check();
}

void run() {
    //TODO: sdcc fix
/*    int8_t item;
	struct StaticEditMenu::StaticEditMenu menu;
	StaticEditMenu::initialize(&menu, editData, editCallback);

    do {
    	StaticEditMenu::setSelector(&menu, getSelector());
        item = StaticEditMenu::runSimple(&menu);

        if(item < 0) {
            break;
        }

#ifdef ENABLE_SETTINGS_MENU_RESET
        if(StaticEditMenu::getEditAddress(&menu, item) == NULL)  //reset
        {
            Settings::setDefault();
            Buzzer::soundSelect();
        } else
#endif
        {
            struct Settings::SettingsData undo(settings);
            if(!StaticEditMenu::runEdit(&menu)) {
                settings = undo;
            } else {
                Buzzer::soundSelect();
            }
            Settings::apply();
        }
    } while(true);
    Settings::save();*/
}

#undef COND_ALWAYS
} //namespace SettingsMenu
