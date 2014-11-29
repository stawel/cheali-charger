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

using namespace settingsMenu;

const char * const SettingsStaticMenu[] PROGMEM =
{
#ifdef ENABLE_LCD_BACKLIGHT
        string_backlight,
#endif
#ifdef ENABLE_FAN
        string_fanOn,
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


SettingsMenu::SettingsMenu(const Settings &p):
        EditMenu(SettingsStaticMenu), p_(p){};


void SettingsMenu::printItem(uint8_t index)
{
    StaticMenu::printItem(index);
    if(getBlinkIndex() != index) {
        START_CASE_COUNTER;
        switch (index) {
#ifdef ENABLE_LCD_BACKLIGHT
            case NEXT_CASE:     lcdPrintUnsigned(p_.backlight, 3);      break;
#endif
#ifdef ENABLE_FAN
            case NEXT_CASE:     printTemp(p_.fanTempOn);                break;
#endif
#ifdef ENABLE_T_INTERNAL
            case NEXT_CASE:     printTemp(p_.dischargeTempOff);         break;
#endif
            case NEXT_CASE:     lcdPrintYesNo(p_.externT);              break;
            case NEXT_CASE:     printTemp(p_.externTCO);                break;
            case NEXT_CASE:     printDeltaT(p_.deltaT);                 break;
            case NEXT_CASE:     lcdPrintYesNo(p_.enable_deltaV);        break;
            case NEXT_CASE:     lcdPrint_mV(p_.deltaV_NiMH, 6);         break;
            case NEXT_CASE:     lcdPrint_mV(p_.deltaV_NiCd, 6);         break;
            case NEXT_CASE:     lcdPrintUnsigned(p_.DCcycles, 3);       break;
            case NEXT_CASE:     lcdPrintUnsigned(p_.DCRestTime, 4);
                                lcdPrintChar('m');                      break;
            case NEXT_CASE:     lcdPrintYesNo(p_.audioBeep);            break;
            case NEXT_CASE:     lcdPrintUnsigned(p_.minIoutDiv, 2);     break;
            case NEXT_CASE:     lcdPrintCurrent(p_.minIout, 5);         break;
            case NEXT_CASE:     lcdPrintPercentage(p_.capCutoff, 5);    break;
            case NEXT_CASE:     printVolt(p_.inputVoltageLow);          break;
            case NEXT_CASE:     lcdPrint_mV(p_.overCharge_LiXX,5);      break;
            case NEXT_CASE:     lcdPrint_mV(p_.overDischarge_LiXX,6);   break;
            case NEXT_CASE:     lcdPrintYesNo(p_.dischargeAggressive_LiXX);break;
            case NEXT_CASE:     lcdPrintYesNo(p_.forceBalancePort);     break;
            case NEXT_CASE:     lcdPrint_mV(p_.balancerError, 5);       break;
            case NEXT_CASE:     printUART();                            break;
            case NEXT_CASE:     printUARTSpeed();                       break;
#ifdef ENABLE_TX_HW_SERIAL
            case NEXT_CASE:     printUARTinput();                       break;
#endif
        }
    }
}

void SettingsMenu::editItem(uint8_t index, uint8_t key)
{
    int dir = -1;
    if(key == BUTTON_INC) dir = 1;
    START_CASE_COUNTER;
    switch(index) {
#ifdef ENABLE_LCD_BACKLIGHT
        case NEXT_CASE:     changeBacklight(dir);                                   break;
#endif
#ifdef ENABLE_FAN
        case NEXT_CASE:     changeTemp(p_.fanTempOn, dir);                          break;
#endif
#ifdef ENABLE_T_INTERNAL
        case NEXT_CASE:     changeTemp(p_.dischargeTempOff, dir);                   break;
#endif
        case NEXT_CASE:     changeExternTemp(dir);                                  break;
        case NEXT_CASE:     changeTemp(p_.externTCO,dir);                           break;
        case NEXT_CASE:     changeDeltaTemp(p_.deltaT,dir);                         break;
        case NEXT_CASE:     change0ToMax(p_.enable_deltaV, dir, 1);                 break;
        case NEXT_CASE:     changeDeltaV(p_.deltaV_NiMH, dir);                      break;
        case NEXT_CASE:     changeDeltaV(p_.deltaV_NiCd, dir);                      break;
        case NEXT_CASE:     change1ToMax(p_.DCcycles, dir, 5);                      break;
        case NEXT_CASE:     change1ToMax(p_.DCRestTime, dir, 99);                   break;
        case NEXT_CASE:     change0ToMax(p_.audioBeep, dir, 1);                     break;
        case NEXT_CASE:     change1ToMax(p_.minIoutDiv, dir, 50);                   break;
        case NEXT_CASE:     change1ToMax(p_.minIout, dir, 200);                     break;
        case NEXT_CASE:     change1ToMax(p_.capCutoff, dir, 250);                   break;
        case NEXT_CASE:     changeInputVolt(p_.inputVoltageLow, dir);               break;
        case NEXT_CASE:     changeOverCharge(dir);                                  break;
        case NEXT_CASE:     changeOverDischarge(dir);                               break;
        case NEXT_CASE:     change0ToMax(p_.dischargeAggressive_LiXX, dir, 1);      break;
        case NEXT_CASE:     change0ToMax(p_.forceBalancePort, dir, 1);              break;
        case NEXT_CASE:     changeBalanceError(p_.balancerError, dir);              break;
        case NEXT_CASE:     changeUART(dir);                                        break;
        case NEXT_CASE:     change0ToMax(p_.UARTspeed, dir, Settings::UARTSpeeds-1);break;
#ifdef ENABLE_TX_HW_SERIAL
        case NEXT_CASE:     change0ToMax(p_.UARTinput, dir, 1);                     break;
#endif
     }
}

void SettingsMenu::run() {
    int8_t index;
    do {
        index = runSimple();

        if(index < 0) return;
        switch(index) {
#ifdef ENABLE_SETTINGS_MENU_RESET
        case sizeOfArray(SettingsStaticMenu) - 2:  //reset
            p_.setDefault();
            Buzzer::soundSelect();
            break;
#endif
        default:
            Settings undo(p_);
            if(!runEdit(index)) {
                p_ = undo;
            } else {
                Buzzer::soundSelect();
                p_.check();
            }
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
    lcdPrint_P(string_unitDeltaT);
}

void SettingsMenu::changeBacklight(int dir) {
    change0ToMax(p_.backlight, dir, 99);
    p_.apply();
}

void SettingsMenu::changeExternTemp(int dir) {
    change0ToMax(p_.externT, dir, 1);
#ifdef ENABLE_EXT_TEMP_AND_UART_COMMON_OUTPUT
    if(p_.externT)
#ifdef ENABLE_TX_HW_SERIAL
        if(p_.UARTinput == Settings::Software)
#endif
        	p_.UART = Settings::Disabled;
#endif
}

void SettingsMenu::changeUART(int dir) {
    change0ToMax(p_.UART, dir, Settings::ExtDebugAdc);
#ifdef ENABLE_EXT_TEMP_AND_UART_COMMON_OUTPUT
    if(p_.UART != Settings::Disabled)
#ifdef ENABLE_TX_HW_SERIAL
        if(p_.UARTinput == Settings::Software)
#endif
            p_.externT = false;
#endif
}

const char * const SettingsUART[] PROGMEM = {
        string_disable,
        string_normal,
        string_debug,
        string_extDebug,
        string_extDebugAdc
};

const char * const SettingsUARTinput[] PROGMEM = {
		string_temp,
		string_pin7,
};

void SettingsMenu::printUART() const
{
    lcdPrint_P(pgm::read(&SettingsUART[p_.UART]));
}

void SettingsMenu::printUARTinput() const
{
    lcdPrint_P(pgm::read(&SettingsUARTinput[p_.UARTinput]));
}

void SettingsMenu::printUARTSpeed() const
{
    //TODO: add printULong
    uint32_t s = p_.getUARTspeed();
    s/=100;
    lcdPrintUnsigned(s,5);
    lcdPrintChar('0');
    lcdPrintChar('0');
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

void SettingsMenu::changeInputVolt(AnalogInputs::ValueType &v, int step) {
    const AnalogInputs::ValueType min = ANALOG_VOLT(7);
    const AnalogInputs::ValueType max = ANALOG_VOLT(30);
    step *= ANALOG_VOLT(1);
    v+=step;
    if(v < min) v = min;
    if(v > max) v = max;
}

void SettingsMenu::changeBalanceError(AnalogInputs::ValueType &v, int step) {
    const AnalogInputs::ValueType min = ANALOG_VOLT(0.003);
    const AnalogInputs::ValueType max = ANALOG_VOLT(0.200);
    v+=step;
    if(v < min) v = min;
    if(v > max) v = max;
}

void SettingsMenu::changeOverCharge(int dir)
{
    int16_t &v = p_.overCharge_LiXX;
    const int16_t min = -ANALOG_VOLT(0.035);
    const int16_t max = +ANALOG_VOLT(0.035);
    v+=dir;
    if(v < min) v = min;
    if(v > max) v = max;
}
void SettingsMenu::changeOverDischarge(int dir)
{
    int16_t &v = p_.overDischarge_LiXX;
    const int16_t min = -ANALOG_VOLT(0.999);
    const int16_t max = +ANALOG_VOLT(0.999);
    v+=dir;
    if(v < min) v = min;
    if(v > max) v = max;
}
void SettingsMenu::changeDeltaV(int16_t &v, int dir)
{
    const int16_t min = -ANALOG_VOLT(0.020);
    const int16_t max = +ANALOG_VOLT(0.000);
    v+=dir;
    if(v < min) v = min;
    if(v > max) v = max;
}

