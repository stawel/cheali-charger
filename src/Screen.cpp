#include <avr/pgmspace.h>
#include "Screen.h"
#include "LcdPrint.h"
#include "ProgramData.h"

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

	uint8_t displayBalanceInfo(int from, bool mesured)
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
		BLINK_NON;
	}
}

uint8_t Screen::display(ScreenType screen, uint8_t blink )
{
	switch(screen) {
	case Screen1: 			return displayScreen1(blink);
	case ScreenCIVlimits:	return displayScreenCIVlimits(blink);
	case ScreenTime:		return displayScreenTime(blink);
	case ScreenTemperature:	return displayScreenTemperature(blink);
	case ScreenBalancer0_2:	return displayBalanceInfo(0, false);
	case ScreenBalancer3_5:	return displayBalanceInfo(3, false);
	case ScreenBalancer0_2M:return displayBalanceInfo(0, true);
	case ScreenBalancer3_5M:return displayBalanceInfo(3, true);
	case ScreenRthVth:		return displayRthVth(blink);
	}
	return -1;
}

uint8_t Screen::displayScreen1(uint8_t blink)
{
	double charge = 0;
	char c = 'N';
	uint16_t value = 0;
	AnalogInputs::Name I;
	AnalogInputs::Name V;

	BLINK_START;
	lcd.setCursor(0,0);

	c= 'N';
	if(smps.isPowerOn()) {
		c = 'C';
		charge = smps.getCharge();
		value = smps.getValue();
		I = smps.IName;
		V = smps.VName;
	}
	if(discharger.isPowerOn()) {
		c = 'D';
		charge = discharger.getDischarge();
		value = discharger.getValue();
		I = discharger.IName;
		V = discharger.VName;

		if(smps.isPowerOn()) c = 'E';
	}

	analogInputs.print(charge, AnalogInputs::Charge, 8);
	lcd.print(' ');
	analogInputs.printRealValue(I,  7);
	lcdPrintSpaces();

	lcd.setCursor(0,1);
	lcd.print(c);
	lcd.print('=');
	lcdPrintEValueI(value, 	4, PSTR("   "));
	analogInputs.printRealValue(V, 	7);
	lcdPrintSpaces();
	BLINK_END;
}

uint8_t Screen::displayScreenCIVlimits(uint8_t blink)
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
	BLINK_NON;
}

uint8_t Screen::displayScreenTime(uint8_t blink)
{
	BLINK_START;
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
	BLINK_END;
}

uint8_t Screen::displayScreenTemperature(uint8_t blink)
{
	BLINK_START;
	lcd.setCursor(0,0);
	lcdPrint_P(PSTR("Text="));
	analogInputs.printRealValue(AnalogInputs::Textern,	4);
	lcdPrintSpaces();

	lcd.setCursor(0,1);
	lcdPrint_P(PSTR("Tint="));
	analogInputs.printRealValue(AnalogInputs::Tintern,	4);
	lcdPrintSpaces();
	BLINK_END;
}

void Screen::displayStrings(const char *s1, const char *s2) const
{
	lcd.setCursor(0,0); lcdPrint_P(s1);
	lcd.setCursor(0,1); lcdPrint_P(s2);
}

uint8_t Screen::displayChargeEnded(uint8_t blink)
{
	displayStrings(smps.getError1(), smps.getError2());
	BLINK_NON;
}

uint8_t Screen::displayRthVth(uint8_t blink)
{
	lcd.setCursor(0,0);

	lcdPrint_P(PSTR("Rth="));
	analogInputs.print(Rth_, AnalogInputs::Resistance, 8);
	lcdPrintSpaces();

	lcd.setCursor(0,1);
	lcdPrint_P(PSTR(""));
	analogInputs.print(Vth_, AnalogInputs::Voltage, 7);
	lcdPrint_P(PSTR(" "));
	analogInputs.print(I_, AnalogInputs::Current, 8);
	lcdPrintSpaces();
	BLINK_NON;
}


Screen screens;

