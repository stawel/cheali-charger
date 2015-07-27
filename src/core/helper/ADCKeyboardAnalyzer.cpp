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

namespace ADCKeyboardAnalyzer {

void run() {
    SerialLog::powerOn();
    AnalogInputs::powerOn();
    Balancer::powerOn();

    while(1) {
        lcdSetCursor(0,0);
        for(int i=0;i<4;i++) {
            lcdPrintUnsigned(AnalogInputs::getADCValue(AnalogInputs::Name(AnalogInputs::Vb1_pin+i)), 7);
            lcdPrintSpace1();
            if(i==1) {
                lcdSetCursor(0,1);
            }
        }
        Time::delayDoIdle(100);
    }
}

} //namespace ADCKeyboardAnalyzer

