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
        lcdPrintSpace1();
    }

    void printTime() {
        lcdPrintSpace1();
        lcdPrintTime(Monitor::getTimeSec());
        lcdPrintSpace1();
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
        printTime();
    }

    void printDeltaV() {
        int16_t x = AnalogInputs::getRealValue(AnalogInputs::deltaVout);
        lcdPrintSigned(x, 5);
        lcdPrintChar('m');
        lcdPrintChar('V');
        lcdPrintSpaces();

    }
    void printDeltaT() {
        if(ProgramData::battery.enable_externT) {
            int16_t x = AnalogInputs::getRealValue(AnalogInputs::deltaTextern);
            lcdPrintSigned(x*10, 5);
            lcdPrintChar('m');
            lcdPrintChar('C');
        } else {
            lcdPrint_P(PSTR("no dT/t"));
        }
        lcdPrintSpaces();
    }


} }// namespace Screen::Methods

void Screen::Methods::displayFirstScreen()
{
    lcdSetCursor0_0();
    printCharge();
    AnalogInputs::printRealValue(AnalogInputs::Iout,     7);
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
    lcdPrintSpace1();
    lcdPrintCurrent(Strategy::maxI, 7);
    lcdPrintSpaces();

    lcdSetCursor0_1();
    lcdPrint_P(PSTR("Limits: "));
    lcdPrintVoltage(Strategy::endV, 7);
    lcdPrintSpaces();
}

void Screen::Methods::displayTime()
{
    lcdSetCursor0_0();
#ifdef ENABLE_TIME_LIMIT
    lcdPrintAnalog(ProgramData::battery.time, 8, AnalogInputs::TimeLimitMinutes);
    lcdPrintSpaces(2);
#else
    lcdPrint_P(PSTR("time:     "));
#endif
    lcdPrintTime(Monitor::getTimeSec());
    lcdSetCursor0_1();
    lcdPrint_P(PSTR("b "));
    lcdPrintTime(Monitor::getTotalBalanceTimeSec());
    lcdPrintSpaces(2);
    lcdPrintTime(Monitor::getTotalChargeDischargeTimeSec());
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
    lcdSetCursor0_0();
    lcdPrint_P(PSTR("Text="));
    if(ProgramData::battery.enable_externT)
        AnalogInputs::printRealValue(AnalogInputs::Textern,    5);
    else
        lcdPrint_P(PSTR("-"));
    lcdPrintSpaces();

    lcdSetCursor0_1();
#ifdef ENABLE_T_INTERNAL
    lcdPrint_P(PSTR("Tint="));
    AnalogInputs::printRealValue(AnalogInputs::Tintern,    5);
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
    lcdPrint_P(PSTR("maxVout="));
    AnalogInputs::printRealValue(AnalogInputs::deltaVoutMax, 7);
    lcdPrintSpaces();

    lcdSetCursor0_1();
    lcdPrint_P(PSTR("delta V= "));
    printDeltaV();
}

void Screen::Methods::displayDeltaTextern()
{
    lcdSetCursor0_0();
    lcdPrint_P(PSTR("Text="));
    if(ProgramData::battery.enable_externT) {
        lcdPrintTemperature(AnalogInputs::getDeltaLastT(), 9);
    } else {
        lcdPrint_P(PSTR("N/A"));
    }
    lcdPrintSpaces();

    lcdSetCursor0_1();
    lcdPrint_P(PSTR("delta T= "));
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

