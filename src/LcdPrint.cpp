#include <avr/eeprom.h>

#include "LcdPrint.h"
#include "Hardware.h"


uint8_t lcdPrintSpaces()
{
	return lcdPrintSpaces(16);
}

uint8_t lcdPrintSpaces(uint8_t n)
{
	for(uint8_t i=0;i<n;i++)
		lcd.print(' ');
	return n;
}


uint8_t lcdPrint(const char *str, uint8_t n)
{
	if(str) {
		char buffer[LCD_COLUMNS + 1];
		if (n > LCD_COLUMNS) n = LCD_COLUMNS;
		strncpy(buffer, str, n);
		buffer[n] = 0;
		return lcd.print(buffer);
	}
	return 0;
}


uint8_t lcdPrint_P(const char *str)
{
	return lcdPrint_P (str, LCD_COLUMNS);
}

uint8_t lcdPrint_P(const char *str, uint8_t n)
{
	if(str) {
		char buffer[LCD_COLUMNS + 1];
		if (n > LCD_COLUMNS) n = LCD_COLUMNS;
		strncpy_P(buffer, str, n);
		buffer[n] = 0;
		return lcd.print(buffer);
	}
	return 0;
}

uint8_t lcdPrint_E(const char *str, uint8_t n)
{
	if(str) {
		char buffer[LCD_COLUMNS + 1];
		if (n > LCD_COLUMNS) n = LCD_COLUMNS;
		buffer[n--]=0;
		for (uint8_t i = 0; i < n; i++) {
			buffer[i] = eeprom_read_byte((uint8_t*)str + i);
			if(!buffer[i])
				break;
		}
		buffer[n] = 0;
		return lcd.print(buffer);
	}
	return 0;
}


uint8_t digits(uint16_t x)
{
	uint8_t retu = 0;
	if(x == 0)
		x=1;
	for(;x!=0; x/=10)
		retu++;
	return retu;
}

namespace {


void lcdPrintInf(int8_t dig)
{
	for(; dig > 3; dig--)
		lcd.print(' ');

	lcdPrint_P(PSTR("Inf"), dig);
}

void lcdPrintEValue(uint16_t x, int8_t dig, bool dot)
{
	const char prefix = ' ';
	if(dig<=0)
		return;

	if(dig<=0)
		return;

	int8_t xdig = digits(x/1000);
	xdig+=1+3; // m or .

	for(;xdig<dig;dig--)
		lcd.print(prefix);

	if(dig >= xdig && !dot) {
		xdig = digits(x);
		dig--; //m
		for(;xdig<dig;dig--)
			lcd.print(prefix);
		lcd.print(x);
		lcd.print('m');
	} else if(dig < xdig - 4) {
		lcdPrintInf(dig);
	} else {
		lcd.print(x/1000);
		x%=1000;
		dig -= xdig - 4;
		if(dig-- >0) lcd.print('.');
		if(dig-- >0) lcd.print(x/100);
		x%=100;
		if(dig-- >0) lcd.print(x/10);
		x%=10;
		if(dig-- >0) lcd.print(x);
	}
}
}

void lcdPrintUnsigned(uint16_t x, int8_t dig)
{
	const char prefix = ' ';
	if(dig<=0)
		return;
	uint8_t xdig = digits(x);

	if(dig < xdig) {
		lcdPrintInf(dig);
	} else {
		for(;xdig<dig;dig--)
			lcd.print(prefix);
		lcd.print(x);
	}
}



void lcdPrintCharge(AnalogInputs::ValueType c, int8_t dig)
{
	lcdPrintAnalog(c, AnalogInputs::Charge, dig);
}
void lcdPrintCurrent(AnalogInputs::ValueType i, int8_t dig)
{
	lcdPrintAnalog(i, AnalogInputs::Current, dig);
}

void lcdPrintVoltage(AnalogInputs::ValueType v, int8_t dig)
{
	lcdPrintAnalog(v, AnalogInputs::Voltage, dig);
}

void lcdPrintResistance(AnalogInputs::ValueType r, int8_t dig)
{
	lcdPrintAnalog(r, AnalogInputs::Resistance, dig);
}



void lcdPrintAnalog(AnalogInputs::ValueType x, AnalogInputs::Type type, int8_t dig)
{
	if(dig <= 0)
		return;
	dig--;
	bool dot = true;
	char unit = 'U';
	switch (type) {
	case AnalogInputs::Current:
		dot = false;
		unit ='A';
		break;
	case AnalogInputs::Voltage:
		unit ='V';
		break;
	case AnalogInputs::Temperature:
		unit ='C';
		x*=10;
		break;
	case AnalogInputs::Resistance:
		dot = false;
		//TODO: ??
		unit ='W';
		break;
	case AnalogInputs::Unknown:
		break;
	case AnalogInputs::Charge:
		dot = false;
		dig--;
		unit ='h';
		break;
	}
	lcdPrintEValue(x, (int8_t) dig, dot);
	lcd.print(unit);

	if(type == AnalogInputs::Charge) 	lcd.print('A');
}


