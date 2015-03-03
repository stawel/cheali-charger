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

#include "cprintfhelper.h"

namespace Screen {

    void printCharge() {
        lcdPrintCharge(AnalogInputs::getRealValue(AnalogInputs::Cout), 8);
        lcdPrintSpace1();
    }

    void printCharAndTime() {
        char c = 'N';
        if(SMPS::isPowerOn()) {
            c = 'C';
        } else if(Discharger::isPowerOn()) {
            c = 'D';
            if(SMPS::isPowerOn()) c = 'E';
        } else if(Balancer::isWorking()) {
            c = 'B';
        }

        if(DelayStrategy::isDelay()) {
            c = 'W';
        }
        lcdPrintChar(c);
        lcdPrintSpace1();
        lcdPrintTime(Monitor::getTimeSec());
        lcdPrintSpace1();
    }

    void printDeltaV() {
        int16_t x = AnalogInputs::getRealValue(AnalogInputs::deltaVout);
        lcdPrintSigned(x, 5);
        lcdPrintChar('m');
        lcdPrintChar('V');
        lcdPrintSpaces();

    }
    void printDeltaT() {
        if(settings.externT) {
            int16_t x = AnalogInputs::getRealValue(AnalogInputs::deltaTextern);
            lcdPrintSigned(x*10, 5);
            lcdPrintChar('m');
            lcdPrintChar('C');
        } else {
            lcdPrint_P(PSTR("no dT/t"));
        }
        lcdPrintSpaces();
    }


namespace Methods {

    STRING_CPP(Vinput, "Vinput=");
    STRING_CPP(VinLimit, "limit=");

    void displayVinput() {
        CPRINTF(STR(Vinput),        ANALOG_REAL(V, Vin, 7),                 NEWLINE,
                STR(VinLimit),      VOLT(settings.inputVoltageLow, 7),      SPACES_END);
    }

    uint16_t getStateChar() {
        char c = 'N';
        if(SMPS::isPowerOn()) {
            c = 'C';
        } else if(Discharger::isPowerOn()) {
            c = 'D';
            if(SMPS::isPowerOn()) c = 'E';
        } else if(Balancer::isWorking()) {
            c = 'B';
        }

        if(DelayStrategy::isDelay()) {
            c = 'W';
        }
        return c;
    }


    CPRINTF_DEF_CHAIN(printCharge2,      ANALOG_REAL(CHARGE, Cout, 8),   SPACE, END);
    CPRINTF_DEF_CHAIN(printCharAndTime2, METHOD(CHAR, getStateChar, 1),  SPACE, METHOD(TIME, Monitor::getTimeSec, 7), SPACE, END);

    void displayFirstScreen() {
        CPRINTF(CHAIN(printCharge2),        ANALOG_REAL(A, Iout, 7),            NEWLINE,
                CHAIN(printCharAndTime2),   ANALOG_REAL(V, VoutBalancer, 7),    END);
    }


    STRING_CPP(Limits, "Limits: ");

    void displayCIVlimits()
    {
        CPRINTF(METHOD(CHARGE,ProgramData::getCapacityLimit, 8), SPACE,  AMP(Strategy::maxI, 7), NEWLINE,
                STR(Limits),   VOLT(Strategy::endV, 7),    SPACES_END);
    }

    STRING_CPP(timestr, "time:     ");
    STRING_CPP(bstr,    "b ");

    void displayTime()
    {
        CPRINTF(
#ifdef ENABLE_TIME_LIMIT
                SPACE, CHARGE_TIME(ProgramData::battery.time, 7), SPACES(2),
#else
                STR(timestr),
#endif
                METHOD(TIME, Monitor::getTimeSec, 6), NEWLINE,
                STR(bstr), METHOD(TIME, Monitor::getTotalBalanceTimeSec, 6),
                SPACES(2), METHOD(TIME, Monitor::getTotalChargeDischargeTimeSec, 6), SPACES_END);
    }


    STRING_CPP(battR,    "batt. R=");
    STRING_CPP(wiresR,    "wires R=");

    void displayR()
    {
        CPRINTF(STR(battR), METHOD(R, TheveninMethod::getReadableBattRth, 8), NEWLINE, END);
        if(Monitor::isBalancePortConnected) {
            CPRINTF(STR(wiresR), METHOD(R, TheveninMethod::getReadableWiresRth, 8), END);
        }
        CPRINTF(SPACES_END);
    }

    STRING_CPP(Text,    "Text=");
    STRING_CPP(non,   "-");
    STRING_CPP(Tint,   "Tint=");


    void displayTemperature()
    {
        CPRINTF1(STR(Text));
        if(settings.externT) {
            CPRINTF1(ANALOG_REAL(TEMPERATURE, Textern, 5));
        } else {
            CPRINTF1(STR(non));
        }

        CPRINTF(NEWLINE,
#ifdef ENABLE_T_INTERNAL
                STR(Tint), ANALOG_REAL(TEMPERATURE, Tintern, 5),
#endif
                SPACES_END);
    }

} // namespace Methods
} // namespace Screen


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
    if(settings.externT) {
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
    bool displayBlink_ = (blink.blinkTime_/16)&1;

    lcdSetCursor0_0();
    if (displayBlink_ == true) {
        printCharge();
        AnalogInputs::printRealValue(AnalogInputs::Iout, 7);

        lcdPrintSpaces();
        lcdSetCursor0_1();
        printCharAndTime();
        lcdPrintSpace1();
    } else {
        AnalogInputs::printRealValue(AnalogInputs::Pout, 8);
        lcdPrintSpace1();
        AnalogInputs::printRealValue(AnalogInputs::Iout, 7);
        lcdPrintSpaces();
        lcdSetCursor0_1();
        AnalogInputs::printRealValue(AnalogInputs::Eout, 8);
        lcdPrintSpaces(2);

    }

    uint8_t procent = Monitor::getChargeProcent();
    if (displayBlink_ == true && SMPS::isPowerOn() && procent < 99) {
        //display calculated simple ETA

        if(Monitor::etaDeltaSec > 20)  //bigger 20sec for ETA calc (is 1C)
        {
            lcdPrintTime(Monitor::getETATime()); //TODO_NJ (not accurate for balancing time)
        } else {
            lcdPrint_P(PSTR(" --:--"));
        }
    } else {
        lcdPrintUnsigned(procent, 4);
        lcdPrint_P(PSTR("%"));
        lcdPrintSpaces();
    }
}

