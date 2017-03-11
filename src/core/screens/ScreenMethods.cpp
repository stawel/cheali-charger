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
#include "Balancer.h"

namespace Screen { namespace Methods {

    void printCharge() {
        lcdPrintCharge(AnalogInputs::getRealValue(AnalogInputs::Cout), 8);
    }

    void printCharAndTime() {
        char c = 'N';
        if(SMPS::isPowerOn()) {
            c = 'C';
        } else if(Discharger::isPowerOn()) {
            c = 'D';
            if(SMPS::isPowerOn()) c = 'E';
        } else if(::Balancer::isWorking()) {
            c = 'B';
        }

        if(DelayStrategy::isDelay()) {
            c = 'W';
        }
        lcdPrintChar(c);
        lcdPrintTime(Monitor::getTimeSec(), 7);
        lcdPrintSpace1();
    }

    void printDeltaV() {
        AnalogInputs::printRealValue(AnalogInputs::deltaVout, 7);
        lcdPrintSpaces();

    }
    void printDeltaT() {
        if(ProgramData::battery.enable_externT) {
            AnalogInputs::printRealValue(AnalogInputs::deltaTextern, 8);
        } else {
            lcdPrintR_P(PSTR("N/A"), 6);
        }
        lcdPrintSpaces();
    }

    void printText() {
        lcdSetCursor0_0();
        lcdPrint_P(PSTR("Text="));
        if(ProgramData::battery.enable_externT) {
            lcdPrintTemperature(AnalogInputs::getDeltaLastT(), 8);
        } else {
            lcdPrintR_P(PSTR("N/A"), 8);
        }
        lcdPrintSpaces();
        lcdSetCursor0_1();
    }

} }// namespace Screen::Methods

void Screen::Methods::displayFirstScreen()
{
    lcdSetCursor0_0();
    printCharge();
    AnalogInputs::printRealValue(AnalogInputs::Iout,     8);
    lcdPrintSpaces();

    lcdSetCursor0_1();
    printCharAndTime();
    AnalogInputs::printRealValue(AnalogInputs::VoutBalancer,     7);
    lcdPrintSpaces();
}

void Screen::Methods::displayCIVlimits()
{
    lcdSetCursor0_0();
    lcdPrintCharge(ProgramData::getCapacityLimit(), 8);
    lcdPrintCurrent(Strategy::maxI, 8);
    lcdPrintSpaces();

    lcdSetCursor0_1();
    lcdPrint_P(PSTR("Limits: "));
    lcdPrintVoltage(Strategy::endV, 7);
    lcdPrintSpaces();
}

void Screen::Methods::displayTime()
{
    lcdSetCursor0_0();
    int8_t dig = 8;
#ifdef ENABLE_TIME_LIMIT
    lcdPrintAnalog(ProgramData::battery.time, 8, AnalogInputs::TimeLimitMinutes);
#else
    lcdPrint_P(PSTR("time:"));
    dig += 3;
#endif
    lcdPrintTime(Monitor::getTimeSec(), dig);
    lcdSetCursor0_1();
    lcdPrintChar('b');
    lcdPrintTime(Monitor::getTotalBalanceTimeSec(), 7);
    lcdPrintTime(Monitor::getTotalChargeDischargeTimeSec(), 8);
}


void Screen::Methods::displayR()
{
    lcdSetCursor0_0();
    lcdPrint_P(PSTR("batt. R="));
    lcdPrintResistance(TheveninMethod::getReadableBattRth(), 8);
    lcdPrintSpaces();
    lcdSetCursor0_1();
    if(Monitor::isBalancePortConnected) {
        lcdPrint_P(PSTR("wires R="));
        lcdPrintResistance(TheveninMethod::getReadableWiresRth(),8);
    }
    lcdPrintSpaces();
}

void Screen::Methods::displayVinput()
{
    lcdSetCursor0_0();
    lcdPrint_P(PSTR("Vinput="));
    AnalogInputs::printRealValue(AnalogInputs::Vin, 7);
    lcdPrintSpaces();
    lcdSetCursor0_1();
    lcdPrint_P(PSTR(" limit="));
    lcdPrintAnalog(settings.inputVoltageLow, 7, AnalogInputs::Voltage);
    lcdPrintSpaces();
}

void Screen::Methods::displayVout()
{
    lcdSetCursor0_0();
    lcdPrint_P(PSTR("Vout ="));
    AnalogInputs::printRealValue(AnalogInputs::Vout, 7);
    lcdPrintSpaces();
    lcdSetCursor0_1();
    lcdPrint_P(PSTR("Vbal.="));
    AnalogInputs::printRealValue(AnalogInputs::Vbalancer, 7);
    lcdPrintSpaces();
}

void Screen::Methods::displayTemperature()
{
    printText();
#ifdef ENABLE_T_INTERNAL
    lcdPrint_P(PSTR("Tint="));
    AnalogInputs::printRealValue(AnalogInputs::Tintern,    8);
#endif
    lcdPrintSpaces();
}

void Screen::Methods::displayDeltaFirst()
{
    lcdSetCursor0_0();
    printCharge();
    printDeltaT();

    lcdSetCursor0_1();
    printCharAndTime();
    printDeltaV();
}


void Screen::Methods::displayDeltaVout()
{
    lcdSetCursor0_0();
    lcdPrint_P(PSTR("maxV="));
    AnalogInputs::printRealValue(AnalogInputs::deltaVoutMax, 7);
    lcdPrintSpaces();

    lcdSetCursor0_1();
    lcdPrint_P(PSTR("dV=   "));
    printDeltaV();
}

void Screen::Methods::displayDeltaTextern()
{
    printText();
    lcdPrint_P(PSTR("dT/dt= "));
    printDeltaT();
}

void Screen::Methods::displayEnergy()
{
    lcdSetCursor0_0();
    AnalogInputs::printRealValue(AnalogInputs::Pout, 8);
    lcdPrintSpace1();
    AnalogInputs::printRealValue(AnalogInputs::Iout, 7);
    lcdPrintSpaces();
    lcdSetCursor0_1();
    AnalogInputs::printRealValue(AnalogInputs::Eout, 8);
    lcdPrintSpaces(2);

    uint8_t procent = Monitor::getChargeProcent();
    lcdPrintUnsigned(procent, 4);
    lcdPrint_P(PSTR("%"));
    lcdPrintSpaces();
}

