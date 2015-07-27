/*
    cheali-charger - open source firmware for a variety of LiPo chargers
    Copyright (C) 2015  Pawel Stawicki. All right reserved.

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
#include "memory.h"
#include "Screen.h"
#include "LcdPrint.h"
#include "ProgramData.h"
#include "TheveninMethod.h"
#include "Settings.h"
#include "Hardware.h"
#include "Program.h"
#include "DelayStrategy.h"
#include "Version.h"
#include "ProgramDCcycle.h"
#include "Monitor.h"
#include "PolarityCheck.h"
#include "ScreenMethods.h"
#include "Balancer.h"
#include "ScreenEditable.h"


namespace Screen { namespace Editable {

    int getKeyboardDir() {
        if(Screen::keyboardButton == BUTTON_INC) {
            return 1;
        }
        if(Screen::keyboardButton == BUTTON_DEC) {
            return -1;
        }
        return 0;
    }

} } //namespace Screen::Editable


void Screen::Editable::displayLEDScreen()
{
    static uint8_t displayMaxI = 0;
    int dir = getKeyboardDir();
    if(dir) {
        change0ToInfSmart(&Strategy::maxI, dir);
        Screen::keyboardButton = BUTTON_NONE;
        displayMaxI = 10;
    }

    lcdSetCursor0_0();
    if(displayMaxI) {
        displayMaxI--;
        lcdPrintCurrent(Strategy::maxI, 8);
        lcdPrintSpace1();
    } else {
        Screen::Methods::printCharAndTime();
    }
    AnalogInputs::printRealValue(AnalogInputs::Iout, 7);
    lcdPrintSpaces();

    lcdSetCursor0_1();
    AnalogInputs::printRealValue(AnalogInputs::Pout, 8);
    lcdPrintSpace1();
    AnalogInputs::printRealValue(AnalogInputs::VoutBalancer,     7);
    lcdPrintSpaces();
}

