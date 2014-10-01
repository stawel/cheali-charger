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

#ifdef ENABLE_LCD_RAM_CG
   #define BALANCE_FULL_CELL_CHAR   2
   #define BALANCE_AVR_CELL_CHAR    1
   #define BALANCE_EMPTY_CELL_CHAR  0
#else
   #define BALANCE_FULL_CELL_CHAR   255    //'!'-34 (fulfill char)
   #define BALANCE_AVR_CELL_CHAR    '-'
   #define BALANCE_EMPTY_CELL_CHAR  '_'
#endif


namespace Screen {

    Blink blink;
    bool blinkIcon;
#ifdef  ENABLE_SCREEN_KNIGHTRIDEREFFECT
    int8_t knightRiderCounter = 0;
    bool knightRiderDir;
#endif

    uint8_t toggleTextCounter = 0;
    uint8_t toggleTextCycleCounter_ = 1;
    
    //TODO_NJ for cyclehistory  
    uint16_t cyclesHistoryChCapacity[5]   = {0,0,0,0,0};
    uint16_t cyclesHistoryDcCapacity[5]   = {0,0,0,0,0};
    uint16_t cyclesHistoryChTime[5]       = {0,0,0,0,0};
    uint16_t cyclesHistoryDcTime[5]       = {0,0,0,0,0};
    char     cyclesHistoryMode[5]         = {'-','-','-','-','-'}; //C=charge   D=discharge '-' = none

    const char programString[] PROGMEM = "ChCBBlDiFCStSBChDiCYcyChDiFPCPEB";
    void printProgram2chars(Program::ProgramType prog)
    {
        STATIC_ASSERT(sizeOfArray(programString)-1 == Program::LAST_PROGRAM_TYPE*2);
        for(uint8_t i = 0; i < 2; i++) {
            lcdPrintChar(pgm::read(&programString[prog*2+i]));
        }
    }
   
    AnalogInputs::ValueType getBalanceValue(uint8_t cell, AnalogInputs::Type type)
    {
        if(type == AnalogInputs::Voltage)
            return Balancer::getPresumedV(cell);
        return TheveninMethod::getReadableRthCell(cell);
    }

    void printBalancer(uint8_t cell, AnalogInputs::Type type) {
        if(AnalogInputs::isConnected(AnalogInputs::Name(AnalogInputs::Vb1+cell))) {
            lcdPrintAnalog(getBalanceValue(cell, type), type, 6);
        } else {
            lcdPrint_P(PSTR("  --  "));
        }
    }

    void displayBalanceInfo(uint8_t from, AnalogInputs::Type type)
    {
        lcdSetCursor0_0();
        blinkIcon = !blinkIcon;

#ifdef ENABLE_SCREEN_KNIGHTRIDEREFFECT
        if ( knightRiderDir==true) knightRiderCounter++; else knightRiderCounter--;
        if (knightRiderCounter>5) knightRiderDir=false;
        if (knightRiderCounter<=1) knightRiderDir=true;
#endif
        char c = ' ';
        if(!Balancer::isWorking()) {
            if(!Balancer::isStable())
                c = 'm';
        } else {
            if(Balancer::savedVon_)
                c = 'B';
            else
                c = 'b';
        }

        if (Balancer::balance_ == 0) {
            lcdPrintChar(c);
        }

        if (Balancer::balance_ != 0) {
            uint8_t  j = 1;
            for(uint8_t i = 0; i < Balancer::getCells(); i++) {
                if(i == Balancer::minCell_) {
                    c = BALANCE_EMPTY_CELL_CHAR; //lowest cell
                } else {
                    if(Balancer::balance_&j) {
                        if (blinkIcon) {
                            c = BALANCE_FULL_CELL_CHAR; //flash full/empty cells
                        } else {
                            c = BALANCE_EMPTY_CELL_CHAR; //flash full/empty cells
                        }
                    } else {
                     c = BALANCE_AVR_CELL_CHAR; //average cells
                    }
                }
                lcdPrintChar(c);
                if(j!=128)  j <<= 1;
                else        j = 0;
            }
            lcdPrintSpaces(8 - Balancer::getCells());

        } else {
#ifdef ENABLE_SCREEN_KNIGHTRIDEREFFECT
            char knightRiderArrow;
            if (knightRiderDir==true) knightRiderArrow='>'; else knightRiderArrow='<';
            if ((c == 'm') || (c == ' ')) {
                for (uint8_t i=1; i<7; i++ ) {
                    if (knightRiderCounter==i) lcdPrintChar(knightRiderArrow); else lcdPrintChar(' ');
                }
                lcdPrintChar(' ');
            } else {
                lcdPrintSpaces(7);
            }
#else
        lcdPrintSpaces(7);
#endif
        }
        lcdPrintDigit(from+1);
#ifdef ENABLE_B0_DISCHARGE_VOLTAGE_CORRECTION
        if(from == 0 && Discharger::isPowerOn() && Discharger::getValue()>0) {
            lcdPrintChar('?');
        } else {
            lcdPrintChar(':');
        }
#else
        lcdPrintChar(':');
#endif
        printBalancer(from++, type);
        lcdPrintSpaces();

        lcdSetCursor0_1();
        if(from < MAX_BANANCE_CELLS) {
            lcdPrintDigit(from+1);
            lcdPrintChar(':');
            printBalancer(from++, type);
        }
        if(from < MAX_BANANCE_CELLS) {
            lcdPrintDigit(from+1);
            lcdPrintChar(':');
            printBalancer(from, type);
        }
        lcdPrintSpaces();
    }

    void display_(ScreenType screen);

} // namespace Screen

void Screen::initialize() {
#ifdef SCREEN_START_DELAY_MS
    Time::delay(SCREEN_START_DELAY_MS); //waiting common display charger for display relase
#endif
#ifdef ENABLE_LCD_RAM_CG
    lcdCreateCGRam();
#endif
}

void Screen::printCharge() {
    lcdPrintCharge(AnalogInputs::getRealValue(AnalogInputs::Cout), 8);
    lcdPrintChar(' ');
}

void Screen::printChar_Time() {
    char c = 'N';
    if(SMPS::isPowerOn()) {
        c = 'C';
    } else if(Discharger::isPowerOn()) {
        c = 'D';
        if(SMPS::isPowerOn()) c = 'E';
    } else if(Balancer::isWorking()) {
        c = 'B';
    }

    if(DelayStrategy::isDelay() == true) {
        c = 'W';
    }
    lcdPrintChar(c);
    lcdPrintChar(' ');
    lcdPrintTime(Monitor::getTimeSec());
    lcdPrintChar(' ');
}

void Screen::displayScreenFirst()
{
    lcdSetCursor0_0();
    printCharge();
    AnalogInputs::printRealValue(AnalogInputs::Iout,     7);
    lcdPrintSpaces();

    lcdSetCursor0_1();
    printChar_Time();
    AnalogInputs::printRealValue(AnalogInputs::VoutBalancer,     7);
    lcdPrintSpaces();
}

void Screen::displayScreenCIVlimits()
{
    lcdSetCursor0_0();
    lcdPrintCharge(ProgramData::currentProgramData.getCapacityLimit(), 8);
    lcdPrintChar(' ');
    lcdPrintCurrent(TheveninMethod::getImax(), 7);  //failed value on Nixx
    //ProgramData::currentProgramData.printIcString();
    lcdPrintSpaces();

    lcdSetCursor0_1();
    lcdPrint_P(PSTR("Limits: "));
    lcdPrintVoltage(TheveninMethod::Vend_, 7);
    lcdPrintSpaces();
}


void Screen::doSlowInterrupt()
{
      //TODO_NJ
    //for multiscreen
    toggleTextCounter++; if (toggleTextCounter>40) toggleTextCounter=0;
}

void Screen::displayScreenTime()
{
    lcdSetCursor0_0();
    lcdPrint_P(PSTR("time:     ")); lcdPrintTime(Monitor::getTimeSec());
    lcdSetCursor0_1();
    lcdPrint_P(PSTR("b "));
    lcdPrintTime(Monitor::getTotalBalanceTimeSec());
    lcdPrint_P(PSTR("  "));
    lcdPrintTime(Monitor::getTotalChargeDischargeTimeSec());
}


void Screen::displayScreenR()
{
    lcdSetCursor0_0();
    lcdPrint_P(PSTR("batt. R="));
    lcdPrintResistance(TheveninMethod::getReadableBattRth(), 8);
    lcdPrintSpaces();
    lcdSetCursor0_1();
    if(AnalogInputs::isConnected(AnalogInputs::Vbalancer)) {
        lcdPrint_P(PSTR("wires R="));
        lcdPrintResistance(TheveninMethod::getReadableWiresRth(),8);
    }
    lcdPrintSpaces();
}

void Screen::displayScreenVinput()
{
    lcdSetCursor0_0();
    lcdPrint_P(PSTR("Vinput="));
    AnalogInputs::printRealValue(AnalogInputs::Vin, 7);
    lcdPrintSpaces();
    lcdSetCursor0_1();
    lcdPrint_P(PSTR(" limit="));
    lcdPrintAnalog(settings.inputVoltageLow_, AnalogInputs::Voltage, 7);
    lcdPrintSpaces();
}

void Screen::displayScreenVout()
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

void Screen::displayScreenTemperature()
{
    lcdSetCursor0_0();
    lcdPrint_P(PSTR("Text="));
    if(settings.externT_)
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

void Screen::displayStrings(const char *s)
{
    lcdClear();
    lcdSetCursor0_0();
    lcdPrint_P(s);
}


namespace {
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

namespace {
    void deltaV() {
        int16_t x = AnalogInputs::getRealValue(AnalogInputs::deltaVout);
        lcdPrintSigned(x, 5);
        lcdPrintChar('m');
        lcdPrintChar('V');
        lcdPrintSpaces();

    }
    void deltaT() {
        if(settings.externT_) {
        	int16_t x = AnalogInputs::getRealValue(AnalogInputs::deltaTextern);
            lcdPrintSigned(x*10, 5);
            lcdPrintChar('m');
            lcdPrintChar('C');
        } else {
            lcdPrint_P(PSTR("no dT/t"));
        }
        lcdPrintSpaces();
    }
}

void Screen::displayDeltaFirst()
{
    lcdSetCursor0_0();
    printCharge();
    deltaT();

    lcdSetCursor0_1();
    printChar_Time();
    deltaV();
}


void Screen::displayDeltaVout()
{
    lcdSetCursor0_0();
    lcdPrint_P(PSTR("maxVout="));
    AnalogInputs::printRealValue(AnalogInputs::deltaVoutMax, 7);
    lcdPrintSpaces();

    lcdSetCursor0_1();
    lcdPrint_P(PSTR("delta V= "));
    deltaV();
}

void Screen::displayDeltaTextern()
{
    lcdSetCursor0_0();
    lcdPrint_P(PSTR("Text="));
    if(settings.externT_) {
        lcdPrintTemperature(AnalogInputs::getDeltaLastT(), 9);
    } else {
        lcdPrint_P(PSTR("N/A"));
    }
    lcdPrintSpaces();

    lcdSetCursor0_1();
    lcdPrint_P(PSTR("delta T= "));
    deltaT();
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

void Screen::displayStartInfo()
{
    resetCycleHistory();
    lcdSetCursor0_0();
    ProgramData::currentProgramData.printBatteryString();
    lcdPrintChar(' ');
    ProgramData::currentProgramData.printVoltageString();
    lcdPrintChar(' ');
    printProgram2chars(Program::programType_);

    lcdSetCursor0_1();
    lcdPrintUnsigned(Monitor::getChargeProcent(), 2);
    lcdPrint_P(PSTR("% "));

    int bindex = blink.getBlinkIndex();
    if(bindex & 1) AnalogInputs::printRealValue(AnalogInputs::Vout, 5);
    else lcdPrintSpaces(5);

    lcdPrintChar(' ');
    if(ProgramData::currentProgramData.isLiXX()) {
        //display balance port
        if(bindex & 2) AnalogInputs::printRealValue(AnalogInputs::Vbalancer, 5);
        else lcdPrintSpaces(5);

        if(bindex & 4) lcdPrintDigit(AnalogInputs::getConnectedBalancePorts());
        else lcdPrintChar(' ');
    } else {

        lcdPrintCharge(ProgramData::currentProgramData.battery.C, 6);
        lcdPrintSpaces();
    }
}



void Screen::display(ScreenType screen)
{
	Screen::storeCycleHistoryInfo();     //actualisation values
    blink.incBlinkTime();

    display_(screen);
}
void Screen::display_(ScreenType screen)
{
    switch(screen) {
    case ScreenFirst:                   return displayScreenFirst();
    case ScreenCIVlimits:               return displayScreenCIVlimits();
    case ScreenTime:                    return displayScreenTime();
    case ScreenTemperature:             return displayScreenTemperature();
    case ScreenBalancer1_3:             return displayBalanceInfo(0, AnalogInputs::Voltage);
    case ScreenBalancer4_6:             return displayBalanceInfo(3, AnalogInputs::Voltage);
    case ScreenBalancer7_9:             return displayBalanceInfo(6, AnalogInputs::Voltage);
    case ScreenBalancer1_3Rth:          return displayBalanceInfo(0, AnalogInputs::Resistance);
    case ScreenBalancer4_6Rth:          return displayBalanceInfo(3, AnalogInputs::Resistance);
    case ScreenBalancer7_9Rth:          return displayBalanceInfo(6, AnalogInputs::Resistance);
    case ScreenStartInfo:               return displayStartInfo();
    case ScreenR:                       return displayScreenR();
    case ScreenVout:                    return displayScreenVout();
    case ScreenVinput:                  return displayScreenVinput();
    case ScreenEnergy:                  return displayScreenEnergy();
    case ScreenDeltaVout:               return displayDeltaVout();
    case ScreenDeltaTextern:            return displayDeltaTextern();
    case ScreenDeltaFirst:              return displayDeltaFirst();
    case ScreenCycles:                  return displayScreenCycles();
    default:
        return;
    }
}

void Screen::displayScreenCycles()
{
    uint8_t c;
    //multiscreen (5x2 cyclenumber, D/C, timeD/timeC, mAhDC/mAhC)
    if (++toggleTextCounter>3) toggleTextCounter=0;

    if (toggleTextCounter == 3) {
        toggleTextCycleCounter_++;
        if (toggleTextCycleCounter_ >  ProgramDCcycle::currentCycle) toggleTextCycleCounter_ = 1;
    }

    c = toggleTextCycleCounter_-1;
    lcdSetCursor0_0();
    lcdPrintUnsigned(toggleTextCycleCounter_, 1);
    lcdPrintChar(BALANCE_EMPTY_CELL_CHAR);
    lcdPrintTime(cyclesHistoryDcTime[c]);
    lcdPrintSpaces(1);
    lcdPrintChar(BALANCE_FULL_CELL_CHAR);
    lcdPrintTime(cyclesHistoryChTime[c]);
    lcdPrintSpaces();

    lcdSetCursor0_1();
    lcdPrintCharge(cyclesHistoryDcCapacity[c],8);
    lcdPrintCharge(cyclesHistoryChCapacity[c],8);
    lcdPrintSpaces();
}

void Screen::displayScreenEnergy()
{
    bool displayBlink_ = displayBlink();

    if (displayBlink_ == true) {
        lcdSetCursor0_0();
        printCharge();
        AnalogInputs::printRealValue(AnalogInputs::Iout, 7);

        lcdPrintSpaces();
        lcdSetCursor0_1();
        printChar_Time();
        lcdPrint_P(PSTR(" "));
    } else {
        lcdSetCursor0_0();
        AnalogInputs::printRealValue(AnalogInputs::Pout, 8);
        lcdPrint_P(PSTR(" "));
        AnalogInputs::printRealValue(AnalogInputs::Iout, 7);
        lcdPrintSpaces();
        lcdSetCursor0_1();
        AnalogInputs::printRealValue(AnalogInputs::Eout, 8);
        lcdPrint_P(PSTR("  "));
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

#ifdef ENABLE_SCREEN_ANIMATION
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
#endif

void Screen::displayCalibrationErrorScreen(uint8_t errNo)
{
    lcdClear();
    lcdSetCursor0_0();
    lcdPrint_P(PSTR("Cal.err.  F:"));
    lcdPrintUnsigned(errNo, 2);
    Time::delay(8000);
}

void Screen::resetCycleHistory()
{
    for (uint8_t i = 0; i < 5; i++) {
        cyclesHistoryMode[i] = '-';
        cyclesHistoryChTime[i] = 0;
        cyclesHistoryDcTime[i] = 0;
        cyclesHistoryChCapacity[i] = 0;
        cyclesHistoryDcCapacity[i] = 0;
    }
}

void Screen::storeCycleHistoryInfo()
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

bool Screen::displayBlink()
{
    return toggleTextCounter<=20;
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
