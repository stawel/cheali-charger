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


namespace Monitor {
    STRING(batteryDisconnected,         "battery disc.");
    STRING(internalTemperatureToHigh,   "int.temp.cutoff");
    STRING(balancePortDisconnected,     "balancer disc.");
    STRING(outputCurrentToHigh,         "HW failure");

    STRING(inputVoltageToLow,           "input V to low");
    STRING(capacityLimit,               "capacity cutoff");
    STRING(timeLimit,                   "time limit");
    STRING(externalTemperatureCutOff,   "ext.temp.cutoff");
};

namespace ProgramMenus {
    STRING(charge,              "charge");
    STRING(chargeAndBalance,    "charge+balance");
    STRING(balance,             "balance");
    STRING(discharge,           "discharge");
    STRING(fastCharge,          "fast charge");
    STRING(storage,             "storage");
    STRING(storageAndBalance,   "storage+balanc");
    STRING(dcCycle,             "D>C format");
    STRING(editBattery,         "edit battery");
}

namespace options {
    STRING(settings, "settings");
    STRING(calibrate, "calibrate");
    STRING(resetDefault, "reset default");
}

namespace programData {
    STRING(unlimited,   "unlimited");
    STRING(minutes,     "min.");

    //battery types
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
}

namespace settingsMenu {
    STRING(unitDeltaT,  "C/m");

    //settings menu
    STRING(backlight,   "backlight:");
    STRING(fanOn,       "fan on:   ");
    STRING(dischOff,    "disch off:");
    STRING(externT,     "extrn T:   ");
    STRING(externTCO,   "extrn TCO:");
    STRING(dTdt,        "dT/dt:");
    STRING(enabledV,    "enab dV:   ");
    STRING(NiMHdV,      "NiMH dV:");
    STRING(NiCddV,      "NiCd dV:");
    STRING(DCcycles,    "D/C cycles:");
    STRING(DCRestTime,  "D/C rest:");
    STRING(AudioBeep,   "beep:      ");
    STRING(minIoutDiv,  "min Iout: I/");
    STRING(minIout,     "min Iout:");
    STRING(capCoff,     "cap COff:");
    STRING(inputLow,    "input low:");
    STRING(overChargeLiXX,      "o.charge:");
    STRING(overDischargeLiXX,   "o.disch:");
    STRING(dichAggLiXX, "dis agres: ");
    STRING(forceBalanc, "force bal: ");
    STRING(balancErr,   "bal. err:");
    STRING(UARTview,    "UART: ");
    STRING(UARTspeed,   "speed: ");
    STRING(UARTinput,   "UART inp: ");
    STRING(reset,       "   reset");

    //UART input menu
    STRING(temp,    "temp");
    STRING(pin7,    "pin7");

    //UART view menu
    STRING(disable,     "disabled");
    STRING(normal,      "normal");
    STRING(debug,       "debug");
    STRING(extDebug,    "ext. deb");
    STRING(extDebugAdc, "ext. Adc");
}

namespace lcd_print {
    STRING(infinity,    "Inf");
    STRING(yes,         "yes");
    STRING(no,          " no");
    STRING(unit_mV,     "mV");
}

namespace programDataMenu {
    STRING(editedName,  "Edit name:");
    STRING(name,        "Name:");

    //menu
    STRING(batteryType,         "Bat:  ");
    STRING(voltage,             "V:  ");
    STRING(capacity,            "Ch: ");
    STRING(chargeCurrent,       "Ic: ");
    STRING(dischargeCurrent,    "Id: ");
    STRING(timeLimit,           "Tlim: ");
    STRING(createName,          "Create name");
    STRING(editName,            "Edit name");
    STRING(resetName,           "Reset name");
}

namespace DeltaChargeStrategy {
    STRING(batteryVoltageReachedUpperLimit,         "V limit");
    STRING(batteryVoltageReachedDeltaVLimit,        "-dV");
    STRING(externalTemperatureReachedDeltaTLimit,   "dT/dt");
}

namespace Calibrate {
    STRING(connect, "connect");
    STRING(disconnect, "disconnect");
    STRING(battery, "battery!");
    STRING(balancePort,  "balance port!");

    //calibration main menu
    STRING(voltage,             "voltage");
    STRING(chargeCurrent,       "I charge");
    STRING(dischargeCurrent,    "I discharge");
    STRING(externalTemperature, "temp extern");
    STRING(internalTemperature, "temp intern");
    STRING(extertsVoltage,      "expert DANGER!");


    //calibration voltage menu
    STRING(v_menu_input,    "Vin: ");
    STRING(v_menu_cell1,    "Vb1: ");
    STRING(v_menu_cell2,    "Vb2: ");
    STRING(v_menu_cell3,    "Vb3: ");
    STRING(v_menu_cell4,    "Vb4: ");
    STRING(v_menu_cell5,    "Vb5: ");
    STRING(v_menu_cell6,    "Vb6: ");
    STRING(v_menu_cell7,    "Vb7: ");
    STRING(v_menu_cell8,    "Vb8: ");
#if MAX_BANANCE_CELLS > 6
    STRING(v_menu_cellSum,  "V1-8:");
#else
    STRING(v_menu_cellSum,  "V1-6:");
#endif
    STRING(v_menu_output,   "Vout:");


    //calibration charge current menu
    STRING(ic_menu_current0,    CHEALI_CHARGER_STRING(CALIBRATION_CHARGE_POINT0_mA) "mA");
    STRING(ic_menu_current1,    CHEALI_CHARGER_STRING(CALIBRATION_CHARGE_POINT1_mA) "mA");

    //calibration discharge current menu
    STRING(id_menu_current0,    CHEALI_CHARGER_STRING(CALIBRATION_DISCHARGE_POINT0_mA) "mA");
    STRING(id_menu_current1,    CHEALI_CHARGER_STRING(CALIBRATION_DISCHARGE_POINT1_mA) "mA");

    //calibration common current menu
    STRING(i_menu_value,    "value: ");
    STRING(i_menu_output,   "Iout:  ");


    //calibration temperature select point menu
    STRING(tp_menu_point0,  "point 1.");
    STRING(tp_menu_point1,  "point 2.");

    //calibration temperature menu
    STRING(t_menu_temperature,  "temp: ");
    STRING(t_menu_value,        "value:");


    //calibration expert voltage menu
    STRING(ev_menu_cell0pin,        "Vb0pin:");
    STRING(ev_menu_cell1pin,        "Vb1pin:");
    STRING(ev_menu_cell2pin,        "Vb2pin:");
    STRING(ev_menu_plusVoltagePin,  "Vplus: ");
    STRING(ev_menu_minusVoltagePin, "Vminus:");
}

#endif /* STRINGS_H_ */
