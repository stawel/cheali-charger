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

void printChar(char * &buf, uint8_t &maxSize, char chr)
{
	if(maxSize>0) {
		*buf = chr; buf++; maxSize--;
		if(maxSize>0)
			*buf = 0;
	}
}


void print_P(char * &buf, uint8_t &maxSize, const char *str)
{
	if(str) {
		uint8_t s = strlen_P(str);
		if(s>maxSize) s = maxSize;
		strncpy_P(buf, str, s);
		buf+=s;
		maxSize-=s;
		if(maxSize>0)
			*buf = 0;
	}
}

void print(char * &buf, uint8_t &maxSize, const char *str)
{
	if(str) {
		uint8_t s = strlen(str);
		if(s>maxSize) s = maxSize;
		strncpy(buf, str, s);
		buf+=s;
		maxSize-=s;
		if(maxSize>0)
			*buf = 0;
	}
}


void printInt (char * &buf, uint8_t &maxSize, uint16_t value)
{
	uint8_t dig = digits(value);
	if(dig > maxSize) {
		printChar(buf, maxSize,'?');
	} else {
		buf+=dig;
		char * str = buf;
		maxSize-=dig;
		if(maxSize > 0)
			*buf = 0;
		while(dig) {
			uint16_t m = value;
			value /= 10;
			char c = m - 10 * value;
			*--str = c + '0';
			dig--;
		}
	}
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

void lcdPrintSigned(int16_t x, int8_t dig)
{
	const char prefix = ' ';
	if(dig<=0)
		return;

	int16_t y = x;
	uint8_t xdig = digits(abs(x));
	if(y<0) {
		xdig++;
	}

	if(dig < xdig) {
		if(y<0) {
			lcd.print('-');
			dig--;
		}
		lcdPrintInf(dig);
	} else {
		for(;xdig<dig;dig--)
			lcd.print(prefix);
		lcd.print(y);
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
	bool dot = true;
	char unit = 'U';
	dig--;
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
		lcdPrintSigned(x, dig);
		lcd.print(unit);
		return;
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


