#include <avr/eeprom.h>

#include "LcdPrint.h"
#include "Hardware.h"


void lcdPrintSpaces()
{
	lcdPrint_P(PSTR("                "));
}


void lcdPrint (const char *str, uint8_t n)
{
	if(str) {
		char buffer[LCD_COLUMNS + 1];
		if (n > LCD_COLUMNS) n = LCD_COLUMNS;
		strncpy(buffer, str, n);
		buffer[n] = 0;
		lcd.print(buffer);
	}
}


void lcdPrint_P(const char *str, uint8_t n)
{
	if(str) {
		char buffer[LCD_COLUMNS + 1];
		if (n > LCD_COLUMNS) n = LCD_COLUMNS;
		strncpy_P(buffer, str, n);
		buffer[n] = 0;
		lcd.print(buffer);
	}
}

void lcdPrint_E(const char *str, uint8_t n)
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
		lcd.print(buffer);
	}
}


uint8_t digits(unsigned long x)
{
	uint8_t retu = 0;
	if(x == 0)
		x = 1;
	for(;x!=0; x/=10)
		retu++;
	return retu;
}

void doBlink(int8_t &dig, uint8_t blink)
{
	if(blink && dig > 0) {
		uint16_t mili = timer.getMiliseconds();
		mili/=BLINK_SPEED_MS/4;
		if(!(blink & BLINK_SPEED2))
			mili/=4;
		if(mili%2) {
			do{
				lcd.print(' ');
			} while(--dig);
		}
	}
}

void lcdPrintInf(int8_t dig, uint8_t blink = 0)
{
	doBlink(dig, blink);
	for(; dig > 3; dig--)
		lcd.print(' ');

	lcdPrint_P(PSTR("Inf"), dig);
}

void lcdPrintEValue(uint16_t x, int8_t dig, bool dot, char prefix)
{

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

void lcdPrintEValueEnd(uint16_t x, int8_t dig, bool dot, const char *end)
{
	lcdPrintEValue(x, dig, dot);
	lcdPrint_P(end);
}



void lcdPrintEValueU(uint16_t x, int8_t dig, char prefix)
{
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


void lcdPrintEValueI(uint16_t x, int8_t dig, char prefix)
{
	if(dig<=0)
		return;
	if(x<0) {
		lcd.print('-');
		x=-x;
		dig--;
	}
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

void lcdPrintEValueI(uint16_t x, int8_t dig, const char *end, uint8_t blink)
{
	doBlink(dig, blink);
	lcdPrintEValueI(x, dig, ' ');
	lcdPrint_P(end);
}



