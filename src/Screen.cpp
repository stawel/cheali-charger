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

    AnalogInputs::ValueType calculateRth(int16_t V, int16_t I) {
        AnalogInputs::Name iName = screen.iName_;
        AnalogInputs::ValueType  I2 = analogInputs.calibrateValue(iName, abs(I));
        uint32_t R = abs(V);
        R*=1000;
        R/=I2;
        return R;
    }
    AnalogInputs::ValueType calculateRthCell(uint8_t cell)
    {
        return calculateRth(theveninMethod.tBal_[cell].Rth_V_, theveninMethod.tBal_[cell].Rth_I_);
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
        if(analogInputs.isConnected(AnalogInputs::Name(AnalogInputs::Vb0+cell))) {
            lcdPrintAnalog(getBalanceValue(cell, mesured), type, 6);
        } else {
            lcdPrint_P(PSTR("  --  "));
        }
    }

    void displayBalanceInfo(uint8_t from, uint8_t mesured, AnalogInputs::Type type)
    {
        lcd.setCursor(0,0);

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

        char nr = '0' + from;
        lcd.print(nr++);
        lcdPrintChar(':');
        printBalancer(from+0, mesured, type);
        lcdPrintSpaces();

        lcd.setCursor(0,1);
        lcd.print(nr++);
        lcdPrintChar(':');
        printBalancer(from+1, mesured, type);
        lcd.print(nr++);
        lcdPrintChar(':');
        printBalancer(from+2, mesured, type);
        lcdPrintSpaces();
    }

} // namespace {

void Screen::displayScreenFirst()
{
    char c = 'N';
    AnalogInputs::Name V = smps.VName;
    AnalogInputs::ValueType I = 0;

    lcd.setCursor(0,0);

    c= 'N';
    if(smps.isPowerOn()) {
        c = 'C';
        charge_ = smps.getCharge();
        I = analogInputs.getRealValue(smps.IName);
    }
    if(discharger.isPowerOn()) {
        c = 'D';
        charge_ = discharger.getDischarge();
        I = analogInputs.getRealValue(discharger.IName);
        V = discharger.VName;

        if(smps.isPowerOn()) c = 'E';
    }

    lcdPrintCharge(charge_, 8);
    lcdPrintChar(' ');
    lcdPrintCurrent(I,  7);
    lcdPrintSpaces();

    lcd.setCursor(0,1);
    lcdPrintChar(c);
    lcdPrintChar(' ');
    lcdPrintTime(getTimeSec());
    lcdPrintChar(' ');
    analogInputs.printRealValue(V,     7);
    lcdPrintSpaces();
}

void Screen::displayScreenCIVlimits()
{
    lcd.setCursor(0,0);
    lcdPrintCharge(ProgramData::currentProgramData.battery.C, 8);
    lcdPrintChar(' ');
    lcdPrintCurrent(ProgramData::currentProgramData.battery.Ic, 7);
    lcdPrintSpaces();

    lcd.setCursor(0,1);
    lcdPrint_P(PSTR("Limits: "));
    lcdPrintVoltage(ProgramData::currentProgramData.getVoltage(ProgramData::VCharge), 7);
    lcdPrintSpaces();
}

uint16_t Screen::getTimeSec() const
{
    uint32_t t = startTime_totalTime_;
    if(on_) t = timer.getMiliseconds() - startTime_totalTime_;
    return t/1000;
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
    lcd.setCursor(0,0);
    lcdPrint_P(PSTR("time:     ")); lcdPrintTime(getTimeSec());
    lcd.setCursor(0,1);
    lcdPrint_P(PSTR("b "));
    lcdPrintTime(totalBalanceTime_/1000);
    lcdPrint_P(PSTR("  "));
    lcdPrintTime(totalChargDischargeTime_/1000);
}

void Screen::displayScreenR()
{
    lcd.setCursor(0,0);
    lcdPrint_P(PSTR("batt. R="));
    lcdPrintResistance(calculateRth(theveninMethod.tVout_.Rth_V_, theveninMethod.tVout_.Rth_I_),8);
    lcdPrintSpaces();
    lcd.setCursor(0,1);
    lcdPrint_P(PSTR("wires R="));
    int16_t Vwires =  analogInputs.getRealValue(AnalogInputs::Vout);
    Vwires -= analogInputs.getRealValue(AnalogInputs::Vbalacer);
    lcdPrintResistance(calculateRth(Vwires, theveninMethod.tVout_.Rth_I_),8);
    lcdPrintSpaces();
}

void Screen::displayScreenVinput()
{
    lcd.setCursor(0,0);
    lcdPrint_P(PSTR("Vinput="));
    analogInputs.printRealValue(AnalogInputs::Vin, 7);
    lcdPrintSpaces();
    lcd.setCursor(0,1);
    lcdPrint_P(PSTR(" limit="));
    lcdPrintAnalog(settings.inputVoltageLow_, AnalogInputs::Voltage, 7);
    lcdPrintSpaces();
}

void Screen::displayScreenVout()
{
    lcd.setCursor(0,0);
    lcdPrint_P(PSTR("Vout ="));
    analogInputs.printRealValue(AnalogInputs::Vout, 7);
    lcdPrintSpaces();
    lcd.setCursor(0,1);
    lcdPrint_P(PSTR("Vbal.="));
    analogInputs.printRealValue(AnalogInputs::Vbalacer, 7);
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
    lcd.setCursor(0,0);
    lcdPrint_P(PSTR("Iset="));
    analogInputs.printRealValue(Iset, 7);
    lcdPrintSpaces();
    lcd.setCursor(0,1);
    lcdPrint_P(PSTR("Iget="));
    analogInputs.printRealValue(Iget, 7);
    lcdPrintSpaces();
}


void Screen::displayScreenTemperature()
{
    lcd.setCursor(0,0);
    lcdPrint_P(PSTR("Text="));
    analogInputs.printRealValue(AnalogInputs::Textern,    5);
    lcdPrintSpaces();

    lcd.setCursor(0,1);
    lcdPrint_P(PSTR("Tint="));
    analogInputs.printRealValue(AnalogInputs::Tintern,    5);
    lcdPrintSpaces();
}

void Screen::displayStrings(const char *s1, const char *s2)
{
    lcd.setCursor(0,0); lcdPrint_P(s1);
    lcd.setCursor(0,1); lcdPrint_P(s2);
}

void Screen::displayScreenProgramCompleted()
{
    displayStrings(PSTR("program"), PSTR("completed"));
}

void Screen::displayMonitorError()
{
    displayStrings(PSTR("Monitor"), PSTR("error"));
}


void Screen::displayDebugRthVth()
{
    lcd.setCursor(0,0);

    lcdPrint_P(PSTR("V="));
    //lcdPrintResistance(Rth_, 8);
    lcd.print(Rth_V_);
    lcdPrint_P(PSTR(" I="));
    //lcdPrintResistance(Rth_, 8);
    lcd.print(Rth_I_);
    lcdPrintSpaces();

    lcd.setCursor(0,1);
    lcd.print(Vth_);
    lcdPrintChar(' ');
    lcd.print(valueTh_);
    lcdPrintChar(' ');
    uint16_t v;
    if(smps.isPowerOn())    v = smps.getValue();
    else                    v = discharger.getValue();
    lcd.print(v);
    lcdPrintSpaces();
}

void Screen::displayNotImplemented()
{
    lcd.clear();
    screen.displayStrings(PSTR("Function not"), PSTR("implemented yet"));
}

void Screen::runNotImplemented()
{
    displayNotImplemented();
    waitButtonPressed();
}

void Screen::displayScreenReversedPolarity()
{
    lcd.clear();
    lcd.setCursor(0,0);
    lcdPrint_P(PSTR("REVERSE POLARITY"));

    if(settings.isDebug()) {
        lcd.setCursor(0,1);
        lcdPrint_P(PSTR("Vrev:"));
        lcdPrintUnsigned(analogInputs.getValue(AnalogInputs::VreversePolarity), 8);
    }
}

void Screen::displayStartInfo()
{
    lcd.setCursor(0,0);
    ProgramData::currentProgramData.printBatteryString(4);
    lcdPrintChar(' ');
    ProgramData::currentProgramData.printVoltageString();
    lcdPrintChar(' ');
    printProgram2chars(programType_);

    lcd.setCursor(0,1);
    uint16_t procent = getChargeProcent();
    if(procent == 100) {
        if(getBlinkOff())
            lcdPrintSpaces(4);
        else
            lcdPrint_P(PSTR("FUL "));
    } else {
        if(procent < 10)
            lcdPrintChar(' ');
        lcd.print(procent);
        lcdPrint_P(PSTR("% "));
    }

    int bindex = getBlinkIndex();
    if(bindex & 1) analogInputs.printRealValue(AnalogInputs::Vout, 5);
    else lcdPrintSpaces(5);

    lcdPrintChar(' ');
    if(ProgramData::currentProgramData.isLiXX()) {
        //display balance port
        if(bindex & 2) analogInputs.printRealValue(AnalogInputs::Vbalacer, 5);
        else lcdPrintSpaces(5);

        if(bindex & 4) lcd.print(analogInputs.getConnectedBalancePorts());
        else lcdPrintChar(' ');
    } else {
        lcdPrintSpaces(8);
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
    case ScreenBalancer0_2:             return displayBalanceInfo(0, 0, AnalogInputs::Voltage);
    case ScreenBalancer3_5:             return displayBalanceInfo(3, 0, AnalogInputs::Voltage);
    case ScreenDebugBalancer0_2M:       return displayBalanceInfo(0, 1, AnalogInputs::Voltage);
    case ScreenDebugBalancer3_5M:       return displayBalanceInfo(3, 1, AnalogInputs::Voltage);
    case ScreenDebugBalancer0_2RthV:    return displayBalanceInfo(0, 2, AnalogInputs::Unknown);
    case ScreenDebugBalancer3_5RthV:    return displayBalanceInfo(3, 2, AnalogInputs::Unknown);
    case ScreenDebugBalancer0_2RthI:    return displayBalanceInfo(0, 3, AnalogInputs::Unknown);
    case ScreenDebugBalancer3_5RthI:    return displayBalanceInfo(3, 3, AnalogInputs::Unknown);
    case ScreenBalancer0_2Rth:          return displayBalanceInfo(0, 4, AnalogInputs::Resistance);
    case ScreenBalancer3_5Rth:          return displayBalanceInfo(3, 4, AnalogInputs::Resistance);
    case ScreenDebugRthVth:             return displayDebugRthVth();
    case ScreenStartInfo:               return displayStartInfo();
    case ScreenR:                       return displayScreenR();
    case ScreenVout:                    return displayScreenVout();
    case ScreenVinput:                  return displayScreenVinput();
    case ScreenDebugI:                  return displayDebugI();
    }
}
