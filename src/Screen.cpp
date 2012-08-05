#include <avr/pgmspace.h>
#include "Screen.h"
#include "LcdPrint.h"
#include "ProgramData.h"



uint8_t Screen::display(ScreenType screen, uint8_t blink )
{
	switch(screen) {
	case Screen1: 			return displayScreen1(blink);
	case ScreenCIVlimits:	return displayScreenCIVlimits(blink);
	case ScreenTime:		return displayScreenTime(blink);
	case ScreenTemperature:	return displayScreenTemperature(blink);
	case ScreenBalancer0_2:	return displayScreenBalancer0_2(blink);
	case ScreenBalancer3_5:	return displayScreenBalancer3_5(blink);
	case ScreenRthVth:		return displayRthVth(blink);
	}
	return -1;
}

uint8_t Screen::displayScreen1(uint8_t blink)
{
	BLINK_START;
	lcd.setCursor(0,0);

	analogInputs.print(smps.getCharge(), AnalogInputs::Charge, 8);
	lcd.print(' ');
	analogInputs.printRealValue(smps.Im_,  7);
	lcdPrintSpaces();

	lcd.setCursor(0,1);
	lcd.print(smps.isPowerOn() ? 'C' : 'N');
	lcd.print('=');
	lcdPrintEValueI(smps.getValue(), 	4, PSTR("   "));
	analogInputs.printRealValue(smps.Vm_, 	7);
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


uint8_t Screen::displayScreenBalancer0_2(uint8_t blink)
{
	lcd.setCursor(0,0);
//	lcdPrint_P(PSTR("balance "));

	lcd.print(balancer.minCell_);
	lcd.print((balancer.balance_&1)&&1);
	lcd.print((balancer.balance_&2)&&1);
	lcd.print((balancer.balance_&4)&&1);
	lcd.print((balancer.balance_&8)&&1);
	lcd.print((balancer.balance_&16)&&1);
	lcd.print((balancer.balance_&32)&&1);
	lcd.print(' ');

	lcdPrint_P(PSTR("0:"));
	analogInputs.printRealValue(AnalogInputs::Vb0,	6);
	lcdPrintSpaces();

	lcd.setCursor(0,1);
	lcdPrint_P(PSTR("1:"));
	analogInputs.printRealValue(AnalogInputs::Vb1,	6);
	lcdPrint_P(PSTR("2:"));
	analogInputs.printRealValue(AnalogInputs::Vb2,	6);
	lcdPrintSpaces();
	BLINK_NON;
}

uint8_t Screen::displayScreenBalancer3_5(uint8_t blink)
{
	lcd.setCursor(0,0);
	lcdPrint_P(PSTR("balance "));
	lcdPrint_P(PSTR("3:"));
	analogInputs.printRealValue(AnalogInputs::Vb3,	6);
	lcdPrintSpaces();

	lcd.setCursor(0,1);
	lcdPrint_P(PSTR("4:"));
	analogInputs.printRealValue(AnalogInputs::Vb4,	6);
	lcdPrint_P(PSTR("5:"));
	analogInputs.printRealValue(AnalogInputs::Vb5,	6);
	lcdPrintSpaces();
	BLINK_NON;
}



uint8_t Screen::displayChargeEnded(uint8_t blink)
{
	lcd.setCursor(0,0); lcdPrint_P(smps.getError1());
	lcd.setCursor(0,1); lcdPrint_P(smps.getError2());
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

