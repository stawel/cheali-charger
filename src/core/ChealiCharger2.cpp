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

const char string_options[] PROGMEM = "options";
const char * const progmemMainMenu[] PROGMEM =
{string_options, NULL };

MainMenu mainMenu(progmemMainMenu, 1);

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
    hardware::initialize();
   // hardware::delay(100); //waiting for internal-PS stabilized (EEPROM protect)
#ifdef ENABLE_STACK_INFO
    StackInfo::initialize();
#endif

    Settings::load();
#ifdef COMMONDISPLAY    
    hardware::delay(5000);    //waiting common display charger display relase
#endif
    Screen::displayStrings(PSTR("ChealiCharger"),
                           PSTR("ver: "  CHEALI_CHARGER_VERSION_STRING));
    hardware::delay(1000);
#ifdef TESTINGALERT
    Screen::displayStrings(PSTR(" Modded version"),
                           PSTR("  FOR TESTING"));
    hardware::delay(1000);
#endif 
    eeprom::restoreDefault();
    
#ifdef RAM_CG
    lcdCreateCGRam();
#endif  


    if (settings.calibratedState_ <7)
    {
    Screen::runCalibrateBeforeUse();
    }
}
