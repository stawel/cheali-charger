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
#include "ScreenCycle.h"



namespace Screen { namespace Cycle {
    
    //TODO_NJ for cyclehistory  
    uint16_t cyclesHistoryChCapacity[5]   = {0,0,0,0,0};
    uint16_t cyclesHistoryDcCapacity[5]   = {0,0,0,0,0};
    uint16_t cyclesHistoryChTime[5]       = {0,0,0,0,0};
    uint16_t cyclesHistoryDcTime[5]       = {0,0,0,0,0};
    char     cyclesHistoryMode[5]         = {'-','-','-','-','-'}; //C=charge   D=discharge '-' = none

} // namespace Screen
} // namespace Cycle

void Screen::Cycle::displayCycles()
{
    //multiscreen (5x2 cyclenumber, D/C, timeD/timeC, mAhDC/mAhC)
    uint8_t c;
/*    if (++toggleTextCounter>3) toggleTextCounter=0;

    if (toggleTextCounter == 3) {
        toggleTextCycleCounter_++;
        if (toggleTextCycleCounter_ >  ProgramDCcycle::currentCycle) toggleTextCycleCounter_ = 1;
    }
*/
    //TODO: stawel ??
    c = (blink.getBlinkTime()/3) % (ProgramDCcycle::currentCycle);
    lcdSetCursor0_0();
    lcdPrintUnsigned(c+1, 1);
    lcdPrintChar(SCREEN_EMPTY_CELL_CHAR);
    lcdPrintTime(cyclesHistoryDcTime[c]);
    lcdPrintSpaces(1);
    lcdPrintChar(SCREEN_FULL_CELL_CHAR);
    lcdPrintTime(cyclesHistoryChTime[c]);
    lcdPrintSpaces();

    lcdSetCursor0_1();
    lcdPrintCharge(cyclesHistoryDcCapacity[c],8);
    lcdPrintCharge(cyclesHistoryChCapacity[c],8);
    lcdPrintSpaces();
}

void Screen::Cycle::resetCycleHistory()
{
    for (uint8_t i = 0; i < 5; i++) {
        cyclesHistoryMode[i] = '-';
        cyclesHistoryChTime[i] = 0;
        cyclesHistoryDcTime[i] = 0;
        cyclesHistoryChCapacity[i] = 0;
        cyclesHistoryDcCapacity[i] = 0;
    }
}

void Screen::Cycle::storeCycleHistoryInfo()
{
    int8_t c = ProgramDCcycle::currentCycle-1;
    if (ProgramDCcycle::cycleMode == 'C') {
        cyclesHistoryMode[c] = 'C';
        cyclesHistoryChTime[c] = Monitor::getTotalChargeDischargeTimeSec();
        cyclesHistoryChCapacity[c] = AnalogInputs::getRealValue(AnalogInputs::Cout);
    }
    if (ProgramDCcycle::cycleMode == 'D') {
        cyclesHistoryMode[c] = 'D';
        cyclesHistoryDcTime[c] = Monitor::getTotalChargeDischargeTimeSec();
        cyclesHistoryDcCapacity[c] = AnalogInputs::getRealValue(AnalogInputs::Cout);
    }
}
