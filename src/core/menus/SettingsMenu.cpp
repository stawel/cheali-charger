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

//#define ENABLE_DEBUG
#include "debug.h"

namespace SettingsMenu {


const char * const SettingsFanOn[] PROGMEM = {
        string_disable,
        string_always,
        string_FanProgram,
        string_temperature,
        string_tempProgram
};
const cprintf::ArrayData FanOnData  PROGMEM = {SettingsFanOn, &settings.fanOn};

const char * const SettingsUART[] PROGMEM = {
        string_disable,
        string_normal,
        string_debug,
        string_extDebug,
        string_extDebugAdc
};
const cprintf::ArrayData UARTData  PROGMEM = {SettingsUART, &settings.UART};
const cprintf::ArrayData UARTSpeedsData PROGMEM = {Settings::UARTSpeedValue, &settings.UARTspeed};


const char * const SettingsUARToutput[] PROGMEM = {string_temp, string_pin7};
const cprintf::ArrayData UARToutputData  PROGMEM = {SettingsUARToutput, &settings.UARToutput};


const char * const SettingsMenuType[] PROGMEM = {string_simple, string_advanced};
const cprintf::ArrayData menuTypeData  PROGMEM = {SettingsMenuType, &settings.menuType};

const AnalogInputs::ValueType Tmin = (Settings::TempDifference/ANALOG_CELCIUS(1) + 1)*ANALOG_CELCIUS(1);
const AnalogInputs::ValueType Tmax = ANALOG_CELCIUS(99);
const AnalogInputs::ValueType Tstep =  ANALOG_CELCIUS(1);


/*condition bits:*/
#define COND_FAN_ON_T       1
#define COND_UART_ON        2
#define COND_ALWAYS         StaticEditMenu::Always

uint16_t getSelector() {
    uint16_t result = StaticEditMenu::Always;
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
const StaticEditMenu::StaticEditData editData[] PROGMEM = {
#ifdef ENABLE_LCD_BACKLIGHT
{string_backlight,      COND_ALWAYS,    {CP_TYPE_UNSIGNED,0,&settings.backlight},       {1, 0, 100}},
#endif
#ifdef ENABLE_FAN
{string_fanOn,          COND_ALWAYS,    {CP_TYPE_STRING_ARRAY,0,&FanOnData},            {1, 0, Settings::FanProgramTemperature}},
{string_fanTempOn,      COND_FAN_ON_T,  {CP_TYPE_TEMPERATURE,0,&settings.fanTempOn},    {Tstep, Tmin, Tmax}},
#endif
#ifdef ENABLE_T_INTERNAL
{string_dischOff,       COND_ALWAYS,    {CP_TYPE_TEMPERATURE,3,&settings.dischargeTempOff}, {Tstep, Tmin, Tmax}},
#endif
{string_AudioBeep,      COND_ALWAYS,    {CP_TYPE_ON_OFF,0,&settings.audioBeep},         {1, 0, 1}},
{string_minIc,          COND_ALWAYS,    {CP_TYPE_A,0,&settings.minIc},                  {ANALOG_AMP(0.001), ANALOG_AMP(0.001), ANALOG_AMP(0.500)}},
{string_minId,          COND_ALWAYS,    {CP_TYPE_A,0,&settings.minId},                  {ANALOG_AMP(0.001), ANALOG_AMP(0.001), ANALOG_AMP(0.500)}},
{string_inputLow,       COND_ALWAYS,    {CP_TYPE_V,3,&settings.inputVoltageLow},        {ANALOG_VOLT(1), ANALOG_VOLT(7), ANALOG_VOLT(30)}},
#ifdef ENABLE_ANALOG_INPUTS_ADC_NOISE
{string_adcNoise,       COND_ALWAYS,    {CP_TYPE_ON_OFF,0,&settings.adcNoise},          {1, 0, 1}},
#endif
{string_UARTview,       COND_ALWAYS,    {CP_TYPE_STRING_ARRAY,0,&UARTData},             {1, 0, Settings::ExtDebugAdc}},
{string_UARTspeed,      COND_UART_ON,   {CP_TYPE_UINT32_ARRAY,0,&UARTSpeedsData},       {1, 0, Settings::UARTSpeeds-1}},
#ifdef ENABLE_TX_HW_SERIAL
{string_UARToutput,     COND_UART_ON,   {CP_TYPE_STRING_ARRAY,0,&UARToutputData},        {1, 0, 1}},
#endif
{string_MenuType,       COND_ALWAYS,    {CP_TYPE_STRING_ARRAY,0,&menuTypeData},         {1, 0, 1}},

#ifdef ENABLE_SETTINGS_MENU_RESET
{string_reset,          StaticEditMenu::Always, {0,0,NULL}},
#endif
{NULL,                  StaticEditMenu::Last}
};

void editCallback(StaticEditMenu * menu, uint16_t * adr) {
    menu->setSelector(getSelector());
}

void run() {
#ifdef ENABLE_DEBUG
    settings.UART = Settings::Normal;
    SerialLog::powerOn();
#endif

    StaticEditMenu menu(editData, editCallback);
    int8_t item;

    do {
        menu.setSelector(getSelector());
        item = menu.runSimple();

        if(item < 0) break;

#ifdef ENABLE_SETTINGS_MENU_RESET
        if(menu.getEditAddress(item) == NULL)  //reset
        {
            settings.setDefault();
#ifdef ENABLE_DEBUG
            settings.UART = Settings::Normal;
            LogDebug("Reset");
#endif
            Buzzer::soundSelect();
        } else
#endif
        {
            Settings undo(settings);
            if(!menu.runEdit()) {
                settings = undo;
            } else {
                Buzzer::soundSelect();
            }
            settings.apply();
        }
    } while(true);
    settings.save();
}

} //namespace SettingsMenu
