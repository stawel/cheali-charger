/*
    cheali-charger - open source firmware for a variety of LiPo chargers
    Copyright (C) 2014  Paweł Stawicki. All right reserved.

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
#ifndef STRINGS_H_
#define STRINGS_H_

#include "strings_common.h"

// AnalogInputs
    //units
    STRING(A,           "A");
    STRING(V,           "V");
    STRING(W,           "W");
    STRING(Wh,          "Wh");
    STRING(C,           "C");
    STRING(Ah,          "Ah");
    STRING(Ohm,         "\xf4");
    STRING(procent,     "%");
    STRING(C_m,         "C/m");
    STRING(minutes,     "m");
    STRING(unsigned,    "");
    STRING(unknown,     "U");

    STRING(yes,         "yes");
    STRING(no,          " no");
    STRING_SIZE(unlimited,  "nolimit");

// Monitor
    STRING(batteryDisconnected,         "battery disc.");
    STRING(internalTemperatureToHigh,   "int.temp.cutoff");
    STRING(balancePortDisconnected,     "balancer disc.");
    STRING(outputCurrentToHigh,         "HW failure");

    STRING(inputVoltageToLow,           "input V to low");
    STRING(capacityLimit,               "capacity cutoff");
    STRING(timeLimitReached,            "time limit");
    STRING(externalTemperatureCutOff,   "ext.temp.cutoff");

// ProgramMenus
    STRING(charge,              "charge");
    STRING(chargeAndBalance,    "charge+balance");
    STRING(balance,             "balance");
    STRING(discharge,           "discharge");
    STRING(fastCharge,          "fast charge");
    STRING(storage,             "storage");
    STRING(storageAndBalance,   "storage+balanc");
    STRING(dcCycle,             "D>C format");
    STRING(capacityCheck,       "capacity check");
    STRING(editBattery,         "edit battery");

// OptionsMenu
    STRING(settings, "settings");
    STRING(calibrate, "calibrate");
    STRING(resetDefault, "reset default");

//  ProgramData {
//    STRING_SIZE(minutes,    "min.");

    //battery types
    STRING(battery_None,    "None");
    STRING(battery_Unknown, "Unkn");
    STRING(battery_NiCd,    "NiCd");
    STRING(battery_NiMH,    "NiMH");
    STRING(battery_Pb,      "Pb  ");
    STRING(battery_Life,    "Life");
    STRING(battery_Lilo,    "Lilo");
    STRING(battery_Lipo,    "Lipo");
    STRING(battery_Li430,   "L430");
    STRING(battery_Li435,   "L435");
    STRING(battery_NiZn,    "NiZn");
    STRING(battery_LED,     "LED");

// SettingsMenu
    STRING(backlight,   "backlight:");
    STRING(fanOn,       "fan on:");
    STRING(fanTempOn,   "|fan T on:");
    STRING(dischOff,    "disch off:");
    STRING(AudioBeep,   "beep:");
    STRING(minIc,       "min Ic:");
    STRING(maxIc,       "max Ic:");
    STRING(minId,       "min Id:");
    STRING(maxId,       "max Id:");
    STRING(inputLow,    "input low: ");
    STRING(adcNoise,    "adc noise:");
    STRING(UARTview,    "UART:");
    STRING(UARTspeed,   "|speed:");
    STRING(UARToutput,  "|output:");
    STRING(MenuType,    "menus:");
    STRING(reset,       "reset");

    //UARToutput
    STRING(temp,    "temp");
    STRING(pin7,    "pin7");
    STRING(pin38,   "pin38");

    //UART view
    STRING(disable,     "disabled");
    STRING(normal,      "normal");
    STRING(debug,       "debug");
    STRING(extDebug,    "ext. deb");
    STRING(extDebugAdc, "ext. Adc");

    //fanOn reason
//  STRING(disable,     "disabled"); -- defined in UART view
    STRING(always,      "always");
    STRING(FanProgram,  "program");
    STRING(temperature, "Tempera");
    STRING(tempProgram, "T-progr");

    //MenuType
    STRING(simple,      "simple");
    STRING(advanced,    "advanced");


// ProgramDataMenu
    STRING(batteryType, "battery:");
    STRING(batteryVolt, "V:");
    STRING(Vc_per_cell, "Vc:");
    STRING(Vs_per_cell, "Vs:");
    STRING(Vd_per_cell, "Vd:");
    STRING(Vcutoff,     "Vco:");
    STRING(capacity,    "Cap:");
    STRING(Ic,          "Ic:");
    STRING(Id,          "Id:");
    STRING(balancErr,   "bal. err:");

    STRING(enabledV,    "enab dV:");
    STRING(deltaV,      "|dV:");
    STRING(ignoreFirst, "|ignr frst:");

    STRING(externT,     "extrn T:");
    STRING(dTdt,        "|dT/dt:");
    STRING(externTCO,   "|extrn TCO:");

    STRING(timeLimit,   "time:");
    STRING(capCoff,     "cap COff:");
    STRING(DCcycles,    "D/C cycles:");
    STRING(DCRestTime,  "D/C rest:");
    STRING(adaptiveDis, "adapt dis:");


// DeltaChargeStrategy
    STRING(batteryVoltageReachedUpperLimit,         "V limit");
    STRING(batteryVoltageReachedDeltaVLimit,        "-dV");
    STRING(externalTemperatureReachedDeltaTLimit,   "dT/dt");


// Calibrate
#ifdef ENABLE_CALIBRATION
    STRING(connect,     "connect");
    STRING(disconnect,  "disconnect");
    STRING(battery,     "battery!");
    STRING(balancePort, "balance port!");

    //calibration main menu
    STRING(voltage,             "voltage");
    STRING(chargeCurrent,       "I charge");
    STRING(dischargeCurrent,    "I discharge");
    STRING(externalTemperature, "temp extern");
    STRING(internalTemperature, "temp intern");
    STRING(extertsVoltage,      "expert DANGER!");


    //calibration voltage menu
    STRING(v_menu_input,    "Vin:");
    STRING(v_menu_cell1,    "Vb1:");
    STRING(v_menu_cell2,    "Vb2:");
    STRING(v_menu_cell3,    "Vb3:");
    STRING(v_menu_cell4,    "Vb4:");
    STRING(v_menu_cell5,    "Vb5:");
    STRING(v_menu_cell6,    "Vb6:");
    STRING(v_menu_cell7,    "Vb7:");
    STRING(v_menu_cell8,    "Vb8:");
#if MAX_BANANCE_CELLS > 6
    STRING(v_menu_cellSum,  "V1-8:");
#else
    STRING(v_menu_cellSum,  "V1-6:");
#endif
    STRING(v_menu_output,   "Vout:");
    STRING(menu_point,      "calib. p.:");

    //calibration common current menu
    STRING(i_menu_value,    "value:");
    STRING(i_menu_output,   "I:");
    STRING(i_menu_expected, "Iexpec:");

    //calibration temperature select point menu
    STRING(tp_menu_point0,  "point 1.");
    STRING(tp_menu_point1,  "point 2.");

    //calibration temperature menu
    STRING(t_menu_temperature,  "temp:");
    STRING(t_menu_adc,          "adc:");


    //calibration expert voltage menu
    STRING(ev_menu_cell0pin,        "Vb0pin:");
    STRING(ev_menu_cell1pin,        "Vb1pin:");
    STRING(ev_menu_cell2pin,        "Vb2pin:");
    STRING(ev_menu_plusVoltagePin,  "Vplus:");
    STRING(ev_menu_minusVoltagePin, "Vminus:");
#endif //ENABLE_CALIBRATION

#endif /* STRINGS_H_ */
