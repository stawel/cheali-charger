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
#include "ScreenStartInfo.h"

namespace Screen {
namespace StartInfo {

    const char programString[] PROGMEM = "ChCBBlDiFCStSBCYCC";

    void printProgram2chars(Program::ProgramType prog)
    {
        STATIC_ASSERT(sizeOfArray(programString)-1 == Program::EditBattery*2);
        for(uint8_t i = 0; i < 2; i++) {
            lcdPrintChar(pgm::read(&programString[prog*2+i]));
        }
    }

    void printVoltageString(int8_t dig)
    {
        uint16_t voltage = ProgramData::getDefaultVoltage();
        if(ProgramData::battery.type == ProgramData::Unknown || ProgramData::battery.type == ProgramData::LED) {
            voltage = ProgramData::getVoltage(ProgramData::VCharge);
            lcdPrintVoltage(voltage, dig);
        } else {
            uint8_t cells = ProgramData::battery.cells;
            uint8_t size = 5+3;
            if(cells > 9) size++;
            lcdPrintSpaces(dig - size);
            lcdPrintVoltage(voltage, 5);
            lcdPrintChar('/');
            lcdPrintUInt(cells);
            lcdPrintChar('C');
        }
    }

    void printBatteryString() {
        lcdPrint_P(ProgramData::batteryString, ProgramData::battery.type);
    }


} // namespace Screen
} // namespace StartInfo


void Screen::StartInfo::displayStartInfo()
{
    lcdSetCursor0_0();
    printBatteryString();
    lcdPrintSpace1();
    printVoltageString(8);
    lcdPrintSpace1();
    printProgram2chars(Program::programType);
    lcdPrintSpace1();

    lcdSetCursor0_1();
    lcdPrintUnsigned(Monitor::getChargeProcent(), 2);
    lcdPrint_P(PSTR("% "));

    int8_t bindex = Screen::blink.getBlinkIndex();
    if(bindex & 1) AnalogInputs::printRealValue(AnalogInputs::Vout, 5);
    else lcdPrintSpaces(5);

    lcdPrintChar(' ');
    if(ProgramData::isLiXX()) {
        //display balance port
        if(bindex & 2) AnalogInputs::printRealValue(AnalogInputs::Vbalancer, 5);
        else lcdPrintSpaces(5);

        if(bindex & 4) lcdPrintDigit(AnalogInputs::getConnectedBalancePortsCount());
        else lcdPrintSpace1();
    } else {

        if(ProgramData::battery.type == ProgramData::LED) {
            lcdPrintCurrent(ProgramData::battery.Ic, 6);
        } else {
            lcdPrintCharge(ProgramData::battery.capacity, 6);
        }
        lcdPrintSpaces();
    }
}
