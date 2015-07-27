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
#include "ChealiCharger2.h"
#include "MainMenu.h"
#include "ProgramData.h"
#include "AnalogInputs.h"
#include "ProgramMenus.h"
#include "Options.h"
#include "Utils.h"
#include "Buzzer.h"
#include "Version.h"
#include "Settings.h"
#include "StackInfo.h"
#include "Hardware.h"
#include "SerialLog.h"
#include "eeprom.h"
#include "cpu.h"
#include "Serial.h"
#include "Screen.h"
#include "Balancer.h"
#include STRINGS_HEADER
#include "atomic.h"
#include "SettingsMenu.h"

namespace AnalogInputsAnalyzer {



STRING_CPP(v_menu_output_plus,  "V+_pin:  ");
STRING_CPP(v_menu_output_minus, "V-_pin:  ");
STRING_CPP(Ismps,           "Ismps:   ");
STRING_CPP(Idischarge,      "Idischa: ");

STRING_CPP(VoutMux,         "VoutMux: ");
STRING_CPP(Tintern,         "Tintern: ");
STRING_CPP(Vin,             "Vin:     ");
STRING_CPP(Textern,         "Textern: ");

STRING_CPP(v_menu_cell0,    "Vb0_pin: ");
STRING_CPP(v_menu_cell1,    "Vb1_pin: ");
STRING_CPP(v_menu_cell2,    "Vb2_pin: ");
STRING_CPP(v_menu_cell3,    "Vb3_pin: ");
STRING_CPP(v_menu_cell4,    "Vb4_pin: ");
STRING_CPP(v_menu_cell5,    "Vb5_pin: ");
STRING_CPP(v_menu_cell6,    "Vb6_pin: ");
#if MAX_BANANCE_CELLS > 6
STRING_CPP(v_menu_cell7,    "Vb7_pin: ");
STRING_CPP(v_menu_cell8,    "Vb8_pin: ");
#endif
STRING_CPP(v_menu_type, "type: ");
STRING_CPP(v_menu_settings, "settings");


const char * const voltageMenu[] PROGMEM = {
        string_v_menu_output_plus,
        string_v_menu_output_minus,
        string_Ismps,
        string_Idischarge,

        string_VoutMux,
        string_Tintern,
        string_Vin,
        string_Textern,

        string_v_menu_cell0, string_v_menu_cell1, string_v_menu_cell2,
        string_v_menu_cell3, string_v_menu_cell4, string_v_menu_cell5, string_v_menu_cell6,
        BALANCER_PORTS_GT_6( string_v_menu_cell7, string_v_menu_cell8,)
        string_v_menu_type,
        string_v_menu_settings, NULL};

const AnalogInputs::Name voltageName[] PROGMEM = {
        AnalogInputs::Vout_plus_pin,
        AnalogInputs::Vout_minus_pin,
        AnalogInputs::Ismps,
        AnalogInputs::Idischarge,

        AnalogInputs::VoutMux,
        AnalogInputs::Tintern,
        AnalogInputs::Vin,
        AnalogInputs::Textern,

        AnalogInputs::Vb0_pin,
        AnalogInputs::Vb1_pin,
        AnalogInputs::Vb2_pin,
        AnalogInputs::Vb3_pin,
        AnalogInputs::Vb4_pin,
        AnalogInputs::Vb5_pin,
        AnalogInputs::Vb6_pin,
        BALANCER_PORTS_GT_6(AnalogInputs::Vb7_pin, AnalogInputs::Vb8_pin,)
};


uint8_t type = 0;
#define MAX_TYPE 3

class VoltageMenu: public StaticMenu {
public:
    VoltageMenu(const char * const* vMenu,  uint8_t dig) :
        StaticMenu(vMenu),
        dig_(dig){};
    virtual void printItem(uint8_t index) {
        StaticMenu::printItem(index);
        if(index < sizeOfArray(voltageName)) {
            AnalogInputs::Name name = pgm::read(&voltageName[index]);
            uint16_t value;
            if(type == 0) {
                value = AnalogInputs::getAvrADCValue(name);
            } else if (type == 1) {
                value = AnalogInputs::getRealValue(name);
            } else {
                ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
                    value = AnalogInputs::getADCValue(name);
                }

            }

            lcdPrintUnsigned(value, dig_);
        }
        if(index == sizeOfArray(voltageName)) {
            lcdPrintUnsigned(type, dig_);
        }
    }
    const uint8_t dig_;
};

void run() {
    SerialLog::powerOn();
    AnalogInputs::powerOn();
    Balancer::powerOn();
    PolarityCheck::checkReversedPolarity_ = false;

    VoltageMenu v(voltageMenu, 5);
    int8_t index;
    do {
        index = v.runSimple(true);
        if((uint8_t)index < sizeOfArray(voltageMenu)-2) {
            type ++;
            if(type >= MAX_TYPE) {
                type = 0;
            }
        } else {
            Balancer::powerOff();
            AnalogInputs::powerOff();
            SerialLog::powerOff();
            SettingsMenu::run();
            SerialLog::powerOn();
            AnalogInputs::powerOn();
            Balancer::powerOn();
        }
    } while(true);
}

} //namespace AnalogInputsAnalyzer

