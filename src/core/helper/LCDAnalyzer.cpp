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
#include "Time.h"

namespace LCDAnalyzer {


void run() {

    uint32_t t;
    int j = 0;
    int dir = 1;
    do {
        t = Time::getMiliseconds();
        Utils::delayMicroseconds(50000);
        t = Time::getMiliseconds() - t;
        lcdSetCursor(0,0);
        lcdPrintUInt(t);
        lcdPrintSpaces();
        lcdSetCursor(0,1);
        for(int i=0;i<16;i++) {
            lcdPrintChar(' '+i+j);
        }
        j+=dir;
        if(j>128-16 - ' ' || j < 1) dir *= -1;

    } while(true);
}

} //namespace LCDAnalyzer

