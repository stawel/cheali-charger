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

const char * const SettingsStaticMenu[] PROGMEM =
{
#ifdef ENABLE_LCD_BACKLIGHT
        string_backlight,
#endif
#ifdef ENABLE_FAN
        string_fanOn,
        string_fanTempOn,
#endif
#ifdef ENABLE_T_INTERNAL
        string_dischOff,
#endif
        string_externT,
        string_externTCO,
        string_dTdt,
        string_enabledV,
        string_NiMHdV,
        string_NiCddV,
        string_cutoffV_NiMH,
        string_cutoffV_NiCd,
        string_DCcycles,
        string_DCRestTime,
        string_AudioBeep,
        string_minIoutDiv,
        string_minIout,
        string_capCoff,
        string_inputLow,
        string_overChargeLiXX,
        string_overDischargeLiXX,
        string_dichAggLiXX,
        string_forceBalanc,
        string_balancErr,
#ifdef ENABLE_ANALOG_INPUTS_ADC_NOISE
        string_adcNoise,
#endif
        string_UARTview,
        string_UARTspeed,
#ifdef ENABLE_TX_HW_SERIAL
        string_UARTinput,
#endif
#ifdef ENABLE_SETTINGS_MENU_RESET
        string_reset,
#endif
        NULL
};


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


const StaticEditMenu::StaticEditData editData[] PROGMEM = {


#ifdef ENABLE_LCD_BACKLIGHT
        {{CP_TYPE_UNSIGNED,0,&settings.backlight}, {0,100,1}}, //TODO
#endif
#ifdef ENABLE_FAN
        {{CP_TYPE_STRING_ARRAY,0,&FanOnData}, {0, Settings::FanProgramTemperature, 1}}, //TODO
        {{CP_TYPE_TEMPERATURE,0,&settings.fanTempOn}, {Tmin,Tmax,Tstep}},
#endif
#ifdef ENABLE_T_INTERNAL
        {{CP_TYPE_TEMPERATURE,0,&settings.dischargeTempOff}, {Tmin,Tmax,Tstep}},
#endif
        {{CP_TYPE_ON_OFF,0,&settings.externT}, {0,1,1}}, //TODO // TDOO: changeExternTemp(dir);
        {{CP_TYPE_TEMPERATURE,0,&settings.externTCO}, {Tmin,Tmax,Tstep}},
        {{CP_TYPE_TEMP_MINUT,0,&settings.deltaT}, {ANALOG_CELCIUS(0.1), ANALOG_CELCIUS(10), ANALOG_CELCIUS(0.1)}},
        {{CP_TYPE_ON_OFF,0,&settings.enable_deltaV}, {0,1,1}},
        {{CP_TYPE_SIGNED_mV,0,(uint16_t*)&settings.deltaV_NiMH}, {-ANALOG_VOLT(0.020), ANALOG_VOLT(0.000), ANALOG_VOLT(0.001)}},
        {{CP_TYPE_SIGNED_mV,0,(uint16_t*)&settings.deltaV_NiCd}, {-ANALOG_VOLT(0.020), ANALOG_VOLT(0.000), ANALOG_VOLT(0.001)}},
        {{CP_TYPE_V,0,&settings.cutoffV_NiMH}, {ANALOG_VOLT(1.200), ANALOG_VOLT(2.000), ANALOG_VOLT(0.001)}},
        {{CP_TYPE_V,0,&settings.cutoffV_NiCd}, {ANALOG_VOLT(1.200), ANALOG_VOLT(2.000), ANALOG_VOLT(0.001)}},
        {{CP_TYPE_UNSIGNED,0,&settings.DCcycles}, {0,5,1}},
        {{CP_TYPE_MINUTES,0,&settings.DCRestTime}, {1,99,1}},
        {{CP_TYPE_ON_OFF,0,&settings.audioBeep}, {0,1,1}},
        {{CP_TYPE_UNSIGNED,0,&settings.minIoutDiv}, {1,50,1}},
        {{CP_TYPE_A,0,&settings.minIout}, {ANALOG_AMP(0.001),ANALOG_AMP(0.500),ANALOG_AMP(0.001)}},
        {{CP_TYPE_PROCENTAGE,0,&settings.capCutoff}, {1,250,1}},
        {{CP_TYPE_V2,0,&settings.inputVoltageLow}, {ANALOG_VOLT(7),ANALOG_VOLT(30),ANALOG_VOLT(1)}},
        {{CP_TYPE_SIGNED_mV,0,(uint16_t*)&settings.overCharge_LiXX}, {-ANALOG_VOLT(0.035),ANALOG_VOLT(0.035),ANALOG_VOLT(0.001)}},
        {{CP_TYPE_SIGNED_mV,0,(uint16_t*)&settings.overDischarge_LiXX}, {-ANALOG_VOLT(0.999),+ANALOG_VOLT(0.999),+ANALOG_VOLT(0.001)}},
        {{CP_TYPE_ON_OFF,0,&settings.dischargeAggressive_LiXX}, {0,1,1}},
        {{CP_TYPE_ON_OFF,0,&settings.forceBalancePort}, {0,1,1}},
        {{CP_TYPE_SIGNED_mV,0,&settings.balancerError}, {ANALOG_VOLT(0.003),ANALOG_VOLT(0.200),ANALOG_VOLT(0.001)}},
#ifdef ENABLE_ANALOG_INPUTS_ADC_NOISE
        {{CP_TYPE_ON_OFF,0,&settings.adcNoise}, {0,1,1}},
#endif
        {{CP_TYPE_STRING_ARRAY,0,&UARTData}, {0,Settings::ExtDebugAdc, 1}}, //TODO: changeUART(dir);
        {{CP_TYPE_UINT32_ARRAY,0,&UARTSpeedsData}, {0,Settings::UARTSpeeds-1, 1}},
#ifdef ENABLE_TX_HW_SERIAL
        {{CP_TYPE_STRING_ARRAY,0,&UARTinputData}, {0,1,1}},
#endif

};

void run() {
    SerialLog::powerOn();
    StaticEditMenu menu(SettingsStaticMenu, editData);
    int8_t index;

    do {
        index = menu.runSimple();

        if(index < 0) return;
        switch(index) {
#ifdef ENABLE_SETTINGS_MENU_RESET
        case sizeOfArray(SettingsStaticMenu) - 2:  //reset
            settings.setDefault();
            Buzzer::soundSelect();
            break;
#endif
        default:
            Settings undo(settings);
            if(!menu.runEdit(index)) {
                settings = undo;
            } else {
                uint16_t * adr =(uint16_t*) menu.getEditAddress(index);
                if(adr == &settings.UART) changedUART();
                else if(adr == &settings.externT)changedExternTemp();

                Buzzer::soundSelect();
                settings.check();
            }
            settings.apply();
            break;
        }
    } while(true);
    settings.save();
}

} //namespace SettingsMenu
