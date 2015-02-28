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

using namespace settingsMenu;

namespace SettingsMenu {

void changedExternTemp() {
#ifdef ENABLE_EXT_TEMP_AND_UART_COMMON_OUTPUT
    if(settings.externT)
#ifdef ENABLE_TX_HW_SERIAL
        if(settings.UARTinput == Settings::Software)
#endif
            settings.UART = Settings::Disabled;
#endif
}

void changedUART() {
#ifdef ENABLE_EXT_TEMP_AND_UART_COMMON_OUTPUT
    if(settings.UART != Settings::Disabled)
#ifdef ENABLE_TX_HW_SERIAL
        if(settings.UARTinput == Settings::Software)
#endif
            settings.externT = false;
#endif
}

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


const char * const SettingsUARTinput[] PROGMEM = {string_temp, string_pin7};
const cprintf::ArrayData UARTinputData  PROGMEM = {SettingsUARTinput, &settings.UARTinput};

const AnalogInputs::ValueType Tmin = Settings::TempDifference + ANALOG_CELCIUS(1);
const AnalogInputs::ValueType Tmax = ANALOG_CELCIUS(99);
const AnalogInputs::ValueType Tstep =  ANALOG_CELCIUS(1);


/*condition bits:*/
#define COND_FAN_ON_T       1
#define COND_EXTERN_T       2
#define COND_DELTAV         4
#define COND_UART           8
#define COND_ALWAYS         StaticEditMenu::Always

uint16_t getSelector() {
    uint16_t result = StaticEditMenu::Always;
#ifdef ENABLE_FAN
    if(settings.fanOn != Settings::FanProgramTemperature && settings.fanOn != Settings::FanTemperature)
        result -= COND_FAN_ON_T;
#endif
    if(!settings.externT)
        result -= COND_EXTERN_T;
    if(!settings.enable_deltaV)
        result -= COND_DELTAV;
    if(settings.UART == Settings::Disabled)
        result -= COND_UART;
    return result;
}

/*
|static string          |when to display| how to display, see cprintf                   | how to edit |
 */
const StaticEditMenu::StaticEditData editData[] PROGMEM = {
#ifdef ENABLE_LCD_BACKLIGHT
{string_backlight,      COND_ALWAYS,    {CP_TYPE_UNSIGNED,0,&settings.backlight},       {0,100,1}},
#endif
#ifdef ENABLE_FAN
{string_fanOn,          COND_ALWAYS,    {CP_TYPE_STRING_ARRAY,0,&FanOnData},            {0, Settings::FanProgramTemperature, 1}}, //TODO
{string_fanTempOn,      COND_FAN_ON_T,  {CP_TYPE_TEMPERATURE,0,&settings.fanTempOn},    {Tmin,Tmax,Tstep}},
#endif
#ifdef ENABLE_T_INTERNAL
{string_dischOff,       COND_ALWAYS,    {CP_TYPE_TEMPERATURE,0,&settings.dischargeTempOff}, {Tmin,Tmax,Tstep}},
#endif
{string_externT,        COND_ALWAYS,    {CP_TYPE_ON_OFF,0,&settings.externT},           {0,1,1}},
{string_externTCO,      COND_EXTERN_T,  {CP_TYPE_TEMPERATURE,0,&settings.externTCO},    {Tmin,Tmax,Tstep}},
{string_dTdt,           COND_EXTERN_T,  {CP_TYPE_TEMP_MINUT,0,&settings.deltaT},        {ANALOG_CELCIUS(0.1), ANALOG_CELCIUS(10), ANALOG_CELCIUS(0.1)}},
{string_enabledV,       COND_ALWAYS,    {CP_TYPE_ON_OFF,0,&settings.enable_deltaV},     {0,1,1}},
{string_NiMHdV,         COND_DELTAV,    {CP_TYPE_SIGNED_mV,0,(uint16_t*)&settings.deltaV_NiMH}, {-ANALOG_VOLT(0.020), ANALOG_VOLT(0.000), ANALOG_VOLT(0.001)}},
{string_NiCddV,         COND_DELTAV,    {CP_TYPE_SIGNED_mV,0,(uint16_t*)&settings.deltaV_NiCd}, {-ANALOG_VOLT(0.020), ANALOG_VOLT(0.000), ANALOG_VOLT(0.001)}},
{string_cutoffV_NiMH,   COND_ALWAYS,    {CP_TYPE_V,0,&settings.cutoffV_NiMH},           {ANALOG_VOLT(1.200), ANALOG_VOLT(2.000), ANALOG_VOLT(0.001)}},
{string_cutoffV_NiCd,   COND_ALWAYS,    {CP_TYPE_V,0,&settings.cutoffV_NiCd},           {ANALOG_VOLT(1.200), ANALOG_VOLT(2.000), ANALOG_VOLT(0.001)}},
{string_DCcycles,       COND_ALWAYS,    {CP_TYPE_UNSIGNED,0,&settings.DCcycles},        {0,5,1}},
{string_DCRestTime,     COND_ALWAYS,    {CP_TYPE_MINUTES,0,&settings.DCRestTime},       {1,99,1}},
{string_AudioBeep,      COND_ALWAYS,    {CP_TYPE_ON_OFF,0,&settings.audioBeep},         {0,1,1}},
{string_minIoutDiv,     COND_ALWAYS,    {CP_TYPE_UNSIGNED,0,&settings.minIoutDiv},      {1,50,1}},
{string_minIout,        COND_ALWAYS,    {CP_TYPE_A,0,&settings.minIout},                {ANALOG_AMP(0.001),ANALOG_AMP(0.500),ANALOG_AMP(0.001)}},
{string_capCoff,        COND_ALWAYS,    {CP_TYPE_PROCENTAGE,0,&settings.capCutoff},     {1,250,1}},
{string_inputLow,       COND_ALWAYS,    {CP_TYPE_V2,0,&settings.inputVoltageLow},       {ANALOG_VOLT(7),ANALOG_VOLT(30),ANALOG_VOLT(1)}},
{string_overChargeLiXX, COND_ALWAYS,    {CP_TYPE_SIGNED_mV,0,(uint16_t*)&settings.overCharge_LiXX},     {-ANALOG_VOLT(0.035),ANALOG_VOLT(0.035),ANALOG_VOLT(0.001)}},
{string_overDischargeLiXX, COND_ALWAYS, {CP_TYPE_SIGNED_mV,0,(uint16_t*)&settings.overDischarge_LiXX},  {-ANALOG_VOLT(0.999),+ANALOG_VOLT(0.999),+ANALOG_VOLT(0.001)}},
{string_dichAggLiXX,    COND_ALWAYS,    {CP_TYPE_ON_OFF,0,&settings.dischargeAggressive_LiXX},          {0,1,1}},
{string_forceBalanc,    COND_ALWAYS,    {CP_TYPE_ON_OFF,0,&settings.forceBalancePort},  {0,1,1}},
{string_balancErr,      COND_ALWAYS,    {CP_TYPE_SIGNED_mV,0,&settings.balancerError},  {ANALOG_VOLT(0.003),ANALOG_VOLT(0.200),ANALOG_VOLT(0.001)}},
#ifdef ENABLE_ANALOG_INPUTS_ADC_NOISE
{string_adcNoise,       COND_ALWAYS,    {CP_TYPE_ON_OFF,0,&settings.adcNoise},          {0,1,1}},
#endif
{string_UARTview,       COND_ALWAYS,    {CP_TYPE_STRING_ARRAY,0,&UARTData},             {0,Settings::ExtDebugAdc, 1}},
{string_UARTspeed,      COND_UART,      {CP_TYPE_UINT32_ARRAY,0,&UARTSpeedsData},       {0,Settings::UARTSpeeds-1, 1}},
#ifdef ENABLE_TX_HW_SERIAL
{string_UARTinput,      COND_UART,      {CP_TYPE_STRING_ARRAY,0,&UARTinputData},        {0,1,1}},
#endif
#ifdef ENABLE_SETTINGS_MENU_RESET
{string_reset,          StaticEditMenu::Always, {0,0,NULL}},
#endif
{NULL,                  StaticEditMenu::Last}
};

void run() {
#ifdef ENABLE_DEBUG
    settings.UART = Settings::Normal;
    SerialLog::powerOn();
#endif

    StaticEditMenu menu(editData);
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
#endif
            LogDebug("Reset");
            Buzzer::soundSelect();
        } else
#endif
        {
            Settings undo(settings);
            if(!menu.runEdit(item)) {
                settings = undo;
            } else {
                uint16_t * adr =(uint16_t*) menu.getEditAddress(item);
                if(adr == &settings.UART) changedUART();
                else if(adr == &settings.externT)changedExternTemp();

                Buzzer::soundSelect();
                settings.check();
            }
            settings.apply();
        }
    } while(true);
    settings.save();
}

} //namespace SettingsMenu
