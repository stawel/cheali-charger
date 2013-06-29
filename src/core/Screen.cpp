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
#include <avr/pgmspace.h>
#include "Screen.h"
#include "LcdPrint.h"
#include "ProgramData.h"
#include "TheveninMethod.h"
#include "Settings.h"

Screen screen;

namespace {

    const char programString[] PROGMEM = "ChCBBlDiFCStSBChDiCyChDiEBLP";
    void printProgram2chars(Program::ProgramType prog)
    {
        //TODO: ??
        lcdPrint_P(programString+prog*2, 2);
    }

    uint8_t getChargeProcent(){
        uint16_t v1,v2, v;
        v2 = ProgramData::currentProgramData.getVoltage(ProgramData::VCharge);
        v1 = ProgramData::currentProgramData.getVoltage(ProgramData::VDischarge);
        v = analogInputs.getRealValue(AnalogInputs::VoutBalancer);

        if(v >= v2) return 100;
        if(v <= v1) return 0;
        v-=v1;
        v2-=v1;
        v2/=100;
        v=  v/v2;
        return v;
    }

    AnalogInputs::ValueType calculateRth2(int16_t V, uint16_t I) {
        uint32_t R = abs(V);
        R*=1000;
        R/=I;
        return R;
    }

    AnalogInputs::ValueType calculateRth_calibrated(int16_t V, int16_t I) {
        AnalogInputs::Name iName = Program::iName_;
        AnalogInputs::ValueType  I2 = analogInputs.calibrateValue(iName, abs(I));
        return calculateRth2(V,I2);
    }
    AnalogInputs::ValueType calculateRthCell(uint8_t cell)
    {
        return calculateRth_calibrated(theveninMethod.tBal_[cell].Rth_V_, theveninMethod.tBal_[cell].Rth_I_);
    }
    AnalogInputs::ValueType getBalanceValue(uint8_t cell, uint8_t mesured)
    {
        switch(mesured) {
        case 0: return balancer.getPresumedV(cell);
        case 1: return balancer.getV(cell);
        case 2: return theveninMethod.tBal_[cell].Rth_V_;
        case 3: return theveninMethod.tBal_[cell].Rth_I_;
        case 4: return calculateRthCell(cell);
        }
    }

    void printBalancer(uint8_t cell, uint8_t mesured, AnalogInputs::Type type) {
        if(analogInputs.isConnected(AnalogInputs::Name(AnalogInputs::Vb1+cell))) {
            lcdPrintAnalog(getBalanceValue(cell, mesured), type, 6);
        } else {
            lcdPrint_P(PSTR("  --  "));
        }
    }

    void displayBalanceInfo(uint8_t from, uint8_t mesured, AnalogInputs::Type type)
    {
        lcdSetCursor0_0();

        char c = ' ';
        if(mesured == 0) {
            if(!balancer.isWorking()) {
                if(!balancer.isStable())
                    c = 'm';
            } else {
                if(balancer.savedVon_)
                    c = 'B';
                else
                    c = 'b';
            }
        }
        if(mesured == 1) c = 'M';
        if(mesured == 2) c = 'V';
        if(mesured == 3) c = 'I';
        if(mesured == 4) c = 'R';

        lcdPrintChar(c);

        if(balancer.balance_ != 0) {
            uint8_t  j = 1;
            for(uint8_t i = 0; i < 6; i++) {
                if(i == balancer.minCell_) {
                    c = '_';
                } else {
                    if(balancer.balance_&j) c = '1';
                    else c = '0';
                }
                lcdPrintChar(c);
                j<<=1;
            }
            lcdPrintChar(' ');
        } else lcdPrintSpaces(7);

        lcdPrintDigit(from+1);
        lcdPrintChar(':');
#ifdef ENABLE_B0_DISCHARGE_VOLTAGE_CORRECTION
        if(from == 0 && !settings.isDebug() && discharger.isPowerOn()) {
            lcdPrint_P(PSTR("unknown"));
            from++;
        } else {
            printBalancer(from++, mesured, type);
        }
#else
        printBalancer(from++, mesured, type);
#endif
        lcdPrintSpaces();

        lcdSetCursor0_1();
        lcdPrintDigit(from+1);
        lcdPrintChar(':');
        printBalancer(from++, mesured, type);
        lcdPrintDigit(from+1);
        lcdPrintChar(':');
        printBalancer(from, mesured, type);
        lcdPrintSpaces();
    }

} // namespace {

AnalogInputs::ValueType Screen::getI()
{
    if(smps.isPowerOn())
        return  analogInputs.getRealValue(smps.IName);
    if(discharger.isPowerOn())
        return  analogInputs.getRealValue(discharger.IName);
    return 0;
}

void Screen::printCharge() {
    getCharge(charge_);
    lcdPrintCharge(charge_, 8);
    lcdPrintChar(' ');
}

void Screen::printChar_Time() {
    char c = 'N';
    if(smps.isPowerOn()) {
        c = 'C';
    } else if(discharger.isPowerOn()) {
        c = 'D';
        if(smps.isPowerOn()) c = 'E';
    } else if(balancer.isWorking()) {
        c = 'B';
    }

    lcdPrintChar(c);
    lcdPrintChar(' ');
    lcdPrintTime(getTimeSec());
    lcdPrintChar(' ');
}

void Screen::displayScreenFirst()
{
    lcdSetCursor0_0();
    printCharge();
    lcdPrintCurrent(getI(),  7);
    lcdPrintSpaces();

    lcdSetCursor0_1();
    printChar_Time();
    analogInputs.printRealValue(AnalogInputs::VoutBalancer,     7);
    lcdPrintSpaces();
}

void Screen::displayScreenCIVlimits()
{
    lcdSetCursor0_0();
    lcdPrintCharge(ProgramData::currentProgramData.getCapacityLimit(), 8);
    lcdPrintChar(' ');
    lcdPrintCurrent(ProgramData::currentProgramData.battery.Ic, 7);
    lcdPrintSpaces();

    lcdSetCursor0_1();
    lcdPrint_P(PSTR("Limits: "));
    lcdPrintVoltage(ProgramData::currentProgramData.getVoltage(ProgramData::VCharge), 7);
    lcdPrintSpaces();
}

uint16_t Screen::getTimeSec() const
{
    uint32_t t = startTime_totalTime_;
    if(on_) t = timer.getMiliseconds() - startTime_totalTime_;
    t/=1000;
    return t;
}

void Screen::powerOn()
{
    startTime_totalTime_ = timer.getMiliseconds();
    totalBalanceTime_ = 0;
    totalChargDischargeTime_ = 0;
    on_ = true;
}

void Screen::powerOff()
{
    startTime_totalTime_ = timer.getMiliseconds() - startTime_totalTime_;
    on_ = false;
}

void Screen::doSlowInterrupt()
{
   if(smps.isWorking() || discharger.isWorking())
       totalChargDischargeTime_ += SLOW_INTERRUPT_PERIOD_MILISECONDS;

   if(balancer.isWorking())
       totalBalanceTime_ += SLOW_INTERRUPT_PERIOD_MILISECONDS;

}

void Screen::displayScreenTime()
{
    lcdSetCursor0_0();
    lcdPrint_P(PSTR("time:     ")); lcdPrintTime(getTimeSec());
    lcdSetCursor0_1();
    lcdPrint_P(PSTR("b "));
    lcdPrintTime(totalBalanceTime_/1000);
    lcdPrint_P(PSTR("  "));
    lcdPrintTime(totalChargDischargeTime_/1000);
}

void Screen::displayScreenR()
{
    lcdSetCursor0_0();
    lcdPrint_P(PSTR("batt. R="));
    lcdPrintResistance(calculateRth_calibrated(theveninMethod.tVout_.Rth_V_, theveninMethod.tVout_.Rth_I_),8);
    lcdPrintSpaces();
    lcdSetCursor0_1();
    if(analogInputs.isConnected(AnalogInputs::Vbalancer)) {
        lcdPrint_P(PSTR("wires R="));
        int16_t Vwires =  analogInputs.getRealValue(AnalogInputs::Vout);
        Vwires -= analogInputs.getRealValue(AnalogInputs::Vbalancer);
        lcdPrintResistance(calculateRth2(Vwires, getI()+1),8);
    }
    lcdPrintSpaces();
}

void Screen::displayScreenVinput()
{
    lcdSetCursor0_0();
    lcdPrint_P(PSTR("Vinput="));
    analogInputs.printRealValue(AnalogInputs::Vin, 7);
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
    analogInputs.printRealValue(AnalogInputs::Vout, 7);
    lcdPrintSpaces();
    lcdSetCursor0_1();
    lcdPrint_P(PSTR("Vbal.="));
    analogInputs.printRealValue(AnalogInputs::Vbalancer, 7);
    lcdPrintSpaces();
}

void Screen::displayDebugI()
{
    AnalogInputs::Name Iset;
    AnalogInputs::Name Iget;
    if(smps.isPowerOn()) {
        Iset = AnalogInputs::IsmpsValue;
        Iget = AnalogInputs::Ismps;
    } else {
        Iset = AnalogInputs::IdischargeValue;
        Iget = AnalogInputs::Idischarge;
    }
    lcdSetCursor0_0();
    lcdPrint_P(PSTR("Iset="));
    analogInputs.printRealValue(Iset, 7);
    lcdPrintSpaces();
    lcdSetCursor0_1();
    lcdPrint_P(PSTR("Iget="));
    analogInputs.printRealValue(Iget, 7);
    lcdPrintSpaces();
}


void Screen::displayScreenTemperature()
{
    lcdSetCursor0_0();
    lcdPrint_P(PSTR("Text="));
    if(settings.externT_ || settings.isDebug())
        analogInputs.printRealValue(AnalogInputs::Textern,    5);
    else
        lcdPrint_P(PSTR("not used"));
    lcdPrintSpaces();

    lcdSetCursor0_1();
#ifdef ENABLE_T_INTERNAL
    lcdPrint_P(PSTR("Tint="));
    analogInputs.printRealValue(AnalogInputs::Tintern,    5);
#endif
    lcdPrintSpaces();
}

void Screen::displayStrings(const char *s1, const char *s2)
{
    lcdSetCursor0_0(); lcdPrint_P(s1);
    lcdSetCursor0_1(); lcdPrint_P(s2);
}

namespace {
    void screenEnd(const char * firstLine) {
        lcdSetCursor0_0();
        lcdPrint_P(firstLine);
        lcdSetCursor0_1();
        if(strlen_P(Program::stopReason_)>0) {
            lcdPrint_P(PSTR("reason: "));
            lcdPrint_P(Program::stopReason_);
        }
        lcdPrintSpaces();
    }
}

void Screen::displayScreenProgramCompleted()
{
    screenEnd(PSTR("program complete"));
}

void Screen::displayMonitorError()
{
    screenEnd(PSTR("monitor error"));
}

namespace {
    void deltaV() {
        int x = analogInputs.getRealValue(AnalogInputs::deltaVout);
        lcdPrintSigned(x, 5);
        lcdPrintChar('m');
        lcdPrintChar('V');
        lcdPrintSpaces();

    }
    void deltaT() {
        if(settings.externT_) {
            int x = analogInputs.getRealValue(AnalogInputs::deltaTextern);
            lcdPrintSigned(x*10, 5);
            lcdPrintChar('m');
            lcdPrintChar('C');
        } else {
            lcdPrint_P(PSTR("not used"));
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
    analogInputs.printRealValue(AnalogInputs::deltaVoutMax, 7);
    lcdPrintSpaces();

    lcdSetCursor0_1();
    lcdPrint_P(PSTR("delta V= "));
    deltaV();
}

void Screen::displayDeltaTextern()
{
    lcdSetCursor0_0();
    lcdPrint_P(PSTR("Text="));
    if(settings.externT_ || settings.isDebug()) {
        lcdPrintTemperature(analogInputs.deltaLastT_, 9);
    } else {
        lcdPrint_P(PSTR("not used"));
    }
    lcdPrintSpaces();

    lcdSetCursor0_1();
    lcdPrint_P(PSTR("delta T= "));
    deltaT();
}

void Screen::displayDebugDelta()
{
    lcdSetCursor0_0();

    lcdPrint_P(PSTR("C="));
    lcdPrintUnsigned(analogInputs.getRealValue(AnalogInputs::deltaLastCount),5);
    lcdPrint_P(PSTR(" L="));
    int x = analogInputs.deltaLastT_;
    lcdPrintSigned(x, 6);
    lcdPrintSpaces();

    lcdSetCursor0_1();
    lcdPrint_P(PSTR("N="));
    lcdPrintUnsigned(analogInputs.deltaCount_,5);
    lcdPrint_P(PSTR(" L="));
    x = ProgramData::currentProgramData.getDeltaVLimit();
    lcdPrintSigned(x, 6);
    lcdPrintSpaces();
}

void Screen::displayDebugBalancerReal0_2()
{
    lcdSetCursor0_0();
    lcdPrint_P(PSTR("0:"));
    analogInputs.printRealValue(AnalogInputs::Vb0_real, 6);
    lcdPrint_P(PSTR("1:"));
    analogInputs.printRealValue(AnalogInputs::Vb1_real, 6);
    lcdPrintSpaces();

    lcdSetCursor0_1();
    lcdPrint_P(PSTR("2:"));
    analogInputs.printRealValue(AnalogInputs::Vb2_real, 6);
    lcdPrint_P(PSTR(" real"));
    lcdPrintSpaces();
}

void Screen::displayDebugRthVth()
{
    lcdSetCursor0_0();

    lcdPrint_P(PSTR("V="));
    //lcdPrintResistance(Rth_, 8);
    lcdPrintSigned(theveninMethod.tVout_.Rth_V_);
    lcdPrint_P(PSTR(" I="));
    //lcdPrintResistance(Rth_, 8);
    lcdPrintSigned(theveninMethod.tVout_.Rth_I_);
    lcdPrintSpaces();

    lcdSetCursor0_1();
    lcdPrintSigned(theveninMethod.tVout_.Vth_);
    lcdPrintChar(' ');
    lcdPrintSigned(theveninMethod.valueTh_, 4);
    lcdPrintChar(' ');
    uint16_t v;
    if(smps.isPowerOn())    v = smps.getValue();
    else                    v = discharger.getValue();
    lcdPrintSigned(v,4);
    lcdPrintSpaces();
}

void Screen::displayNotImplemented()
{
    screen.displayStrings(PSTR("Function not"), PSTR("implemented yet"));
}

void Screen::runNotImplemented()
{
    displayNotImplemented();
    waitButtonPressed();
}

void Screen::displayScreenReversedPolarity()
{
    lcdSetCursor0_0();
    lcdPrint_P(PSTR("REVERSE POLARITY"));

    if(settings.isDebug()) {
        lcdSetCursor0_1();
        lcdPrint_P(PSTR("Vrev:"));
        lcdPrintUnsigned(analogInputs.getRealValue(AnalogInputs::VreversePolarity), 8);
    }
}

void Screen::displayStartInfo()
{
    lcdSetCursor0_0();
    ProgramData::currentProgramData.printBatteryString(4);
    lcdPrintChar(' ');
    ProgramData::currentProgramData.printVoltageString();
    lcdPrintChar(' ');
    printProgram2chars(Program::programType_);

    lcdSetCursor0_1();
    uint16_t procent = getChargeProcent();
    if(procent == 100) {
        if(getBlinkOff())
            lcdPrintSpaces(4);
        else
            lcdPrint_P(PSTR("FUL "));
    } else {
        lcdPrintUnsigned(procent, 2);
        lcdPrint_P(PSTR("% "));
    }

    int bindex = getBlinkIndex();
    if(bindex & 1) analogInputs.printRealValue(AnalogInputs::Vout, 5);
    else lcdPrintSpaces(5);

    lcdPrintChar(' ');
    if(ProgramData::currentProgramData.isLiXX()) {
        //display balance port
        if(bindex & 2) analogInputs.printRealValue(AnalogInputs::Vbalancer, 5);
        else lcdPrintSpaces(5);

        if(bindex & 4) lcdPrintDigit(analogInputs.getConnectedBalancePorts());
        else lcdPrintChar(' ');
    } else {

        lcdPrintCharge(ProgramData::currentProgramData.battery.C, 6);
        lcdPrintSpaces();
    }
}



void Screen::display(ScreenType screen)
{
    incBlinkTime();
    switch(screen) {
    case ScreenFirst:                   return displayScreenFirst();
    case ScreenCIVlimits:               return displayScreenCIVlimits();
    case ScreenTime:                    return displayScreenTime();
    case ScreenTemperature:             return displayScreenTemperature();
    case ScreenDebugBalancerReal0_2:    return displayDebugBalancerReal0_2();
    case ScreenBalancer1_3:             return displayBalanceInfo(0, 0, AnalogInputs::Voltage);
    case ScreenBalancer4_6:             return displayBalanceInfo(3, 0, AnalogInputs::Voltage);
    case ScreenDebugBalancer1_3M:       return displayBalanceInfo(0, 1, AnalogInputs::Voltage);
    case ScreenDebugBalancer4_6M:       return displayBalanceInfo(3, 1, AnalogInputs::Voltage);
    case ScreenDebugBalancer1_3RthV:    return displayBalanceInfo(0, 2, AnalogInputs::Unknown);
    case ScreenDebugBalancer4_6RthV:    return displayBalanceInfo(3, 2, AnalogInputs::Unknown);
    case ScreenDebugBalancer1_3RthI:    return displayBalanceInfo(0, 3, AnalogInputs::Unknown);
    case ScreenDebugBalancer4_6RthI:    return displayBalanceInfo(3, 3, AnalogInputs::Unknown);
    case ScreenBalancer1_3Rth:          return displayBalanceInfo(0, 4, AnalogInputs::Resistance);
    case ScreenBalancer4_6Rth:          return displayBalanceInfo(3, 4, AnalogInputs::Resistance);
    case ScreenDebugRthVth:             return displayDebugRthVth();
    case ScreenStartInfo:               return displayStartInfo();
    case ScreenR:                       return displayScreenR();
    case ScreenVout:                    return displayScreenVout();
    case ScreenVinput:                  return displayScreenVinput();
    case ScreenDebugReversedPolarity:   return displayScreenReversedPolarity();
    case ScreenDebugI:                  return displayDebugI();
    case ScreenDebugDelta:              return displayDebugDelta();
    case ScreenDeltaVout:               return displayDeltaVout();
    case ScreenDeltaTextern:            return displayDeltaTextern();
    case ScreenDeltaFirst:              return displayDeltaFirst();
    }
}
