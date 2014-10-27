/*
    cheali-charger - open source firmware for a variety of LiPo chargers
    Copyright (C) 2013  Pawel Stawicki. All right reserved.

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
#include "ScreenStartInfo.h"
#include "ScreenBalancer.h"
#include "ScreenCycle.h"

namespace Screen {

    Blink blink;

    uint8_t screen_nr_;
    uint8_t screen_nr_max_;


    typedef void(*method)();

    method table[] = {
            Screen::StartInfo::displayStartInfo,
            Screen::Methods::displayTemperature,
            NULL,
    };

    void displayAnimation();

    void display() {
        Screen::Cycle::storeCycleHistoryInfo();
        blink.incBlinkTime();

      //  table[screen_nr_]();
    }

void doStrategy(uint8_t key)
{
    if(!PolarityCheck::runReversedPolarityInfo()) {
        Screen::display();
    }

    if(key == BUTTON_INC && screen_nr_ < screen_nr_max_) {
#ifdef ENABLE_SCREEN_ANIMATION
        Screen::displayAnimation();
#endif
        screen_nr_++;
    }
    if(key == BUTTON_DEC && screen_nr_ > 0) {
#ifdef ENABLE_SCREEN_ANIMATION
        Screen::displayAnimation();
#endif
        screen_nr_--;
    }
}
void powerOn()
{
    screen_nr_ = 0;
    screen_nr_max_ = 1;
}

void powerOff()
{

}
    
} // namespace Screen

void Screen::initialize() {
#ifdef SCREEN_START_DELAY_MS
    Time::delay(SCREEN_START_DELAY_MS); //waiting common display charger for display relase
#endif
#ifdef ENABLE_LCD_RAM_CG
    lcdCreateCGRam();
#endif
}

void Screen::displayStrings(const char *s)
{
    lcdClear();
    lcdSetCursor0_0();
    lcdPrint_P(s);
}


namespace Screen {
    void screenEnd(const char * firstLine) {
        lcdSetCursor0_0();
        lcdPrint_P(firstLine);
        lcdSetCursor0_1();
        lcdPrint_P(Program::stopReason_);
        lcdPrintSpaces();
    }
}

void Screen::displayScreenProgramCompleted()
{
    screenEnd(PSTR("program complete"));
    lcdSetCursor0_1();
    lcdPrint_P(PSTR("Time: ")); lcdPrintTime(Monitor::getTimeSec());
}

void Screen::displayMonitorError()
{
    screenEnd(PSTR("Error:"));
}


void Screen::displayNotImplemented()
{
    displayStrings(PSTR("N/A"));
}


void Screen::runNotImplemented()
{
    displayNotImplemented();
    waitButtonPressed();
}

void Screen::displayScreenReversedPolarity()
{
    displayStrings(PSTR("REVERSE POLARITY"));
}

void Screen::displayAnimation()
{
    for (uint8_t i=0; i<16; i++) {
        lcdSetCursor(15-i,1);
        lcdPrintChar(255);
        lcdSetCursor(i,0);
        lcdPrintChar(255);
        Time::delayDoIdle(10);
    }
    Time::delayDoIdle(10);
}

void Screen::displayCalibrationErrorScreen(uint8_t errNo)
{
    lcdClear();
    lcdSetCursor0_0();
    lcdPrint_P(PSTR("Cal.err.  F:"));
    lcdPrintUnsigned(errNo, 2);
    Time::delay(8000);
}

bool Screen::runAskResetEeprom(uint8_t what)
{
    lcdClear();
    lcdSetCursor0_0();
    lcdPrint_P(PSTR("eeprom reset:"));
    lcdPrintUInt(what);
    lcdSetCursor0_1();
    lcdPrint_P(PSTR(" NO         YES"));
    return waitButtonPressed() == BUTTON_START;
}

void Screen::runResetEepromDone(uint8_t before, uint8_t after) {
	if(after != 0) {
	    displayStrings(PSTR("eeprom reset\n"
	                        "error: "));
	    lcdPrintUInt(after);
	} else {
		//TODO
		//if(before )
	    displayStrings(PSTR("please cal."));
	    Time::delay(2000);
	}
}

void Screen::runWelcomeScreen() {
    Screen::displayStrings(PSTR( CHEALI_CHARGER_PROJECT_NAME_STRING "\n"
                                "v" CHEALI_CHARGER_VERSION_STRING));
    Time::delay(1000);
}

void Screen::runNeedForceBalance() {
    Screen::displayStrings(PSTR("NEED force bal.\n"
                                "set. --> YES"));
    Time::delay(2000);
}
