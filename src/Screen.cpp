#include <avr/pgmspace.h>
#include "Screen.h"
#include "LcdPrint.h"
#include "ProgramData.h"


Screen screens;

namespace {
	AnalogInputs::ValueType getBalanceValue(int cell, bool mesured)
	{
		if(mesured)
			return balancer.getV(cell);
		else
			return balancer.getPresumedV(cell);
	}

	void printBalancer(int cell, bool mesured) {
		if(analogInputs.isConnected(AnalogInputs::Name(AnalogInputs::Vb0+cell))) {
			lcdPrintVoltage(getBalanceValue(cell, mesured), 6);
		} else {
			lcdPrint_P(PSTR("  --  "));
		}
	}

	void displayBalanceInfo(int from, bool mesured)
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
		printBalancer(from+0, mesured);
		lcdPrintSpaces();

		lcd.setCursor(0,1);
		lcd.print(nr++);
		lcd.print(':');
		printBalancer(from+1, mesured);
		lcd.print(nr++);
		lcd.print(':');
		printBalancer(from+2, mesured);
		lcdPrintSpaces();
	}
}

void Screen::display(ScreenType screen)
{
	switch(screen) {
	case Screen1: 			return displayScreen1();
	case ScreenCIVlimits:	return displayScreenCIVlimits();
	case ScreenTime:		return displayScreenTime();
	case ScreenTemperature:	return displayScreenTemperature();
	case ScreenBalancer0_2:	return displayBalanceInfo(0, false);
	case ScreenBalancer3_5:	return displayBalanceInfo(3, false);
	case ScreenBalancer0_2M:return displayBalanceInfo(0, true);
	case ScreenBalancer3_5M:return displayBalanceInfo(3, true);
	case ScreenRthVth:		return displayRthVth();
	case ScreenStartInfo: 	return displayStartInfo();
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
	lcdPrintCharge(ProgramData::currentProgramData.C, 8);
	lcd.print(' ');
	lcdPrintCurrent(ProgramData::currentProgramData.I, 7);
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

void Screen::notImplemented()
{
	lcd.clear();
	screens.displayStrings(PSTR("Function not"), PSTR("implemented yet"));
	do { } while(keyboard.getPressedWithSpeed() == BUTTON_NONE);
}

namespace {
const char programString[] PROGMEM = "ChCBBlDiFCStSBENEB";
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

	if(v >= v2) return 99;
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
	if(procent < 10)
		lcd.print(' ');
	lcd.print(procent);
	lcdPrint_P(PSTR("% "));

	int bindex = getBlinkIndex();
	if(bindex & 1) analogInputs.printRealValue(AnalogInputs::Vout, 5);
	else lcdPrintSpaces(5);

	lcd.print(' ');

	if(bindex & 2) analogInputs.printRealValue(AnalogInputs::Vbalacer, 5);
	else lcdPrintSpaces(5);

	if(bindex & 4) lcd.print(analogInputs.getConnectedBalancePorts());
	else lcd.print(' ');

}
