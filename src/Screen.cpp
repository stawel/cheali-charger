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
		analogInputs.print(getBalanceValue(cell, mesured), AnalogInputs::Voltage , 6);
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
			if(balancer.balance_ > 0 && i == balancer.minCell_)
				lcd.print('_');
			else
				lcd.print((balancer.balance_&j)&&1);
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

	analogInputs.print(charge_, AnalogInputs::Charge, 8);
	lcd.print(' ');
	analogInputs.printRealValue(I,  7);
	lcdPrintSpaces();

	lcd.setCursor(0,1);
	lcd.print(c);
	lcd.print('=');
	lcdPrintEValueI(value, 	4, PSTR("   "));
	analogInputs.printRealValue(V, 	7);
	lcdPrintSpaces();
}

void Screen::displayScreenCIVlimits()
{
	lcd.setCursor(0,0);
	analogInputs.print(ProgramData::currentProgramData.C, AnalogInputs::Charge, 8);
	lcd.print(' ');
	analogInputs.print(ProgramData::currentProgramData.I, AnalogInputs::Current, 7);
	lcdPrintSpaces();

	lcd.setCursor(0,1);
	lcdPrint_P(PSTR("Limits: "));
	analogInputs.print(ProgramData::currentProgramData.getVoltage(ProgramData::VCharge), AnalogInputs::Voltage, 7);
	lcdPrintSpaces();
}

void Screen::displayScreenTime()
{
	long t_up = timer.getMiliseconds();
	t_up /= 1000;

	lcd.setCursor(0,0);
	lcdPrint_P(PSTR("C="));
	lcdPrintEValueI(smps.getCharge(),		5, PSTR("s t="));
	lcdPrintEValueI(t_up,					5, PSTR("s     "));

	lcd.setCursor(0,1);
	lcdPrint_P(PSTR("c="));
	lcdPrintEValueI(smps.getOnTimeSec(), 	5, PSTR("/"));
	lcdPrintEValueI(14567,	 				5, PSTR("s     "));
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

	lcdPrint_P(PSTR("Rth="));
	analogInputs.print(Rth_, AnalogInputs::Resistance, 8);
	lcdPrintSpaces();

	lcd.setCursor(0,1);
	lcdPrint_P(PSTR(""));
	analogInputs.print(Vth_, AnalogInputs::Voltage, 7);
	lcdPrint_P(PSTR(" "));
	analogInputs.print(valueTh_, AnalogInputs::Unknown, 8);
	lcdPrintSpaces();
}

void Screen::notImplemented()
{
	lcd.clear();
	screens.displayStrings(PSTR("Function not"), PSTR("implemented yet"));
	do { } while(keyboard.getPressedWithSpeed() == BUTTON_NONE);
}



