#include <avr/pgmspace.h>
#include "Screen.h"
#include "LcdPrint.h"
#include "ProgramData.h"
#include "TheveninMethod.h"

Screen screens;

namespace {
	AnalogInputs::ValueType getBalanceValue(uint8_t cell, uint8_t mesured)
	{
		switch(mesured) {
		case 0: return balancer.getPresumedV(cell);
		case 1:	return balancer.getV(cell);
		case 2:	return theveninMethod.tBal_[cell].Rth_V_;
		case 3:	return theveninMethod.tBal_[cell].Rth_I_;
		case 4:	return abs(theveninMethod.tBal_[cell].Rth_V_*1000/theveninMethod.tBal_[cell].Rth_I_);
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

		char c;
		if(!mesured) {
			if(balancer.balance_ == 0) {
				if(!balancer.isStable())
					c = 'm';
				else
					c = ' ';
			} else {
				if(balancer.savedVon_)
					c = 'B';
				else
					c = 'b';
			}
		} else {
			c='M';
		}
		lcd.print(c);

		uint8_t  j = 1;
		for(uint8_t i = 0; i < 6; i++) {
			if(balancer.balance_ > 0 && i == balancer.minCell_) {
				lcd.print('_');
			} else {
				if(balancer.isPowerOn()) {
					lcd.print((balancer.balance_&j)&&1);
				} else {
					lcd.print(' ');
				}
			}
			j<<=1;
		}
		lcd.print(' ');

		char nr = '0' + from;
		lcd.print(nr++);
		lcd.print(':');
		printBalancer(from+0, mesured, type);
		lcdPrintSpaces();

		lcd.setCursor(0,1);
		lcd.print(nr++);
		lcd.print(':');
		printBalancer(from+1, mesured, type);
		lcd.print(nr++);
		lcd.print(':');
		printBalancer(from+2, mesured, type);
		lcdPrintSpaces();
	}

}

void Screen::display(ScreenType screen)
{
	switch(screen) {
	case Screen1: 				return displayScreen1();
	case ScreenCIVlimits:		return displayScreenCIVlimits();
	case ScreenTime:			return displayScreenTime();
	case ScreenTemperature:		return displayScreenTemperature();
	case ScreenBalancer0_2:		return displayBalanceInfo(0, 0, AnalogInputs::Voltage);
	case ScreenBalancer3_5:		return displayBalanceInfo(3, 0, AnalogInputs::Voltage);
	case ScreenBalancer0_2M:	return displayBalanceInfo(0, 1, AnalogInputs::Voltage);
	case ScreenBalancer3_5M:	return displayBalanceInfo(3, 1, AnalogInputs::Voltage);
	case ScreenBalancer0_2RthV:	return displayBalanceInfo(0, 2, AnalogInputs::Unknown);
	case ScreenBalancer3_5RthV:	return displayBalanceInfo(3, 2, AnalogInputs::Unknown);
	case ScreenBalancer0_2RthI:	return displayBalanceInfo(0, 3, AnalogInputs::Unknown);
	case ScreenBalancer3_5RthI:	return displayBalanceInfo(3, 3, AnalogInputs::Unknown);
	case ScreenBalancer0_2Rth:	return displayBalanceInfo(0, 4, AnalogInputs::Resistance);
	case ScreenBalancer3_5Rth:	return displayBalanceInfo(3, 4, AnalogInputs::Resistance);
	case ScreenRthVth:			return displayRthVth();
	case ScreenStartInfo: 		return displayStartInfo();
	}
}

void Screen::displayScreen1()
{
	char c = 'N';
	uint16_t value = 0;
	AnalogInputs::Name I = smps.IName;
	AnalogInputs::Name V = smps.VName;

	lcd.setCursor(0,0);

	c= 'N';
	if(smps.isPowerOn()) {
		c = 'C';
		charge_ = smps.getCharge();
		value = smps.getValue();
	}
	if(discharger.isPowerOn()) {
		c = 'D';
		charge_ = discharger.getDischarge();
		value = discharger.getValue();
		I = discharger.IName;
		V = discharger.VName;

		if(smps.isPowerOn()) c = 'E';
	}

	lcdPrintCharge(charge_, 8);
	lcd.print(' ');
	analogInputs.printRealValue(I,  7);
	lcdPrintSpaces();

	lcd.setCursor(0,1);
	lcd.print(c);
	lcd.print('=');
	lcdPrintUnsigned(value, 	4);
	lcdPrintSpaces(3);
	analogInputs.printRealValue(V, 	7);
	lcdPrintSpaces();
}

void Screen::displayScreenCIVlimits()
{
	lcd.setCursor(0,0);
	lcdPrintCharge(ProgramData::currentProgramData.battery.C, 8);
	lcd.print(' ');
	lcdPrintCurrent(ProgramData::currentProgramData.battery.Ic, 7);
	lcdPrintSpaces();

	lcd.setCursor(0,1);
	lcdPrint_P(PSTR("Limits: "));
	lcdPrintVoltage(ProgramData::currentProgramData.getVoltage(ProgramData::VCharge), 7);
	lcdPrintSpaces();
}

void Screen::displayScreenTime()
{

	lcd.setCursor(0,0);
	lcdPrint_P(PSTR("C=0")); lcdPrint_P(PSTR("s t="));
	lcdPrintUnsigned(timer.getMiliseconds()/1000,	5); lcdPrint_P(PSTR("s     "));

	lcd.setCursor(0,1);
	lcdPrint_P(PSTR("c="));
	lcdPrintUnsigned(smps.getOnTimeSec(), 	5); lcdPrint_P(PSTR("/"));
	lcdPrintUnsigned(14567,	 				5); lcdPrint_P(PSTR("s     "));
}

void Screen::displayScreenTemperature()
{
	lcd.setCursor(0,0);
	lcdPrint_P(PSTR("Text="));
	analogInputs.printRealValue(AnalogInputs::Textern,	4);
	lcdPrintSpaces();

	lcd.setCursor(0,1);
	lcdPrint_P(PSTR("Tint="));
	analogInputs.printRealValue(AnalogInputs::Tintern,	4);
	lcdPrintSpaces();
}

void Screen::displayStrings(const char *s1, const char *s2) const
{
	lcd.setCursor(0,0); lcdPrint_P(s1);
	lcd.setCursor(0,1); lcdPrint_P(s2);
}

void Screen::displayChargeEnded()
{
	displayStrings(smps.getError1(), smps.getError2());
}

void Screen::displayMonitorError()
{
	displayStrings(PSTR("Monitor"), PSTR("error"));
}


void Screen::displayRthVth()
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
	lcdPrint_P(PSTR(""));
	//lcdPrintVoltage(Vth_, 7);
	lcd.print(Vth_);
	lcd.print('V');

	lcd.print(' ');
	lcd.print(valueTh_);
	lcdPrintSpaces();
}

void Screen::displayNotImplemented()
{
	lcd.clear();
	screens.displayStrings(PSTR("Function not"), PSTR("implemented yet"));
}

void Screen::runNotImplemented()
{
	displayNotImplemented();
	do { } while(keyboard.getPressedWithSpeed() != BUTTON_NONE);
	do { } while(keyboard.getPressedWithSpeed() == BUTTON_NONE);
}

void Screen::reversedPolarity()
{
	lcd.clear();
	lcd.setCursor(0,0); lcdPrint_P(PSTR("REVERSE POLARITY"));
	lcd.setCursor(0,1);
	lcdPrint_P(PSTR("Vrev:"));

	lcdPrintUnsigned(analogInputs.getValue(AnalogInputs::VreversePolarity), 8);
}


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
}

void Screen::displayStartInfo()
{
	lcd.setCursor(0,0);
	ProgramData::currentProgramData.printBatteryString(4);
	lcd.print(' ');
	ProgramData::currentProgramData.printVoltageString();
	lcd.print(' ');
	printProgram2chars(programType_);

	lcd.setCursor(0,1);
	uint16_t procent = getChargeProcent();
	if(procent == 100) {
		if(getBlink())
			lcdPrintSpaces(4);
		else
			lcdPrint_P(PSTR("FUL "));
	} else {
		if(procent < 10)
			lcd.print(' ');
		lcd.print(procent);
		lcdPrint_P(PSTR("% "));
	}

	int bindex = getBlinkIndex();
	if(bindex & 1) analogInputs.printRealValue(AnalogInputs::Vout, 5);
	else lcdPrintSpaces(5);

	lcd.print(' ');
	if(ProgramData::currentProgramData.isLiXX()) {
		//display balance port
		if(bindex & 2) analogInputs.printRealValue(AnalogInputs::Vbalacer, 5);
		else lcdPrintSpaces(5);

		if(bindex & 4) lcd.print(analogInputs.getConnectedBalancePorts());
		else lcd.print(' ');
	} else {
		lcdPrintSpaces(8);
	}


}
