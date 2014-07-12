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
#include "EditName.h"
#include "Program.h"
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

extern "C" {
#include "M051Series.h"
}

const char string_options[] PROGMEM = "options";
const char * const progmemMainMenu[] PROGMEM =
{string_options, NULL };

MainMenu mainMenu(progmemMainMenu, 1);

namespace Utils {
void Delay(uint32_t x);
}

extern uint32_t pwm_n;

void clkInfo() {
	while(1) {
		AnalogInputs::powerOn();
		lcdSetCursor0_0();
		uint32_t t = Timer::getMiliseconds();
		uint32_t t2 = AnalogInputs::getFullMeasurementCount();

		Timer::delayIdle(10000);
		t = Timer::getMiliseconds() - t;
		t2 = AnalogInputs::getFullMeasurementCount() - t2;
		lcdPrintUnsigned(Timer::getMiliseconds()/1000,8);
		lcdPrintUnsigned(pwm_n, 8);

		lcdSetCursor0_1();
		lcdPrintUnsigned(t/100,8);
		lcdPrintUnsigned(t2, 8);
//		AnalogInputs::printRealValue(AnalogInputs::Vout_plus_pin, 8);
//		AnalogInputs::printRealValue(AnalogInputs::Vout_minus_pin, 8);
	}
}

void loop()
{
    int8_t index = mainMenu.runSimple();
    if(index >= 0)  {
        switch(index) {
        case 0:
            Options::run();
            break;
        default:
            Program::selectProgram(index - 1);
        }
    }
}


void setup()
{
    cpu::init();

    hardware::initialize();
    Timer::initialize();
    SMPS::initialize();
    Discharger::initialize();
    AnalogInputs::initialize();

#ifdef ENABLE_STACK_INFO
    StackInfo::initialize();
#endif

    Settings::load();
    Screen::displayStrings(PSTR("  ChealiCharger"),
                           PSTR("    ver: "  CHEALI_CHARGER_VERSION_STRING));
    Timer::delay(1000);

    eeprom::restoreDefault();
    AnalogInputs::powerOn();
//    clkInfo();
}


int main()
{
    setup();
    while(true) {
        loop();
    }
}
