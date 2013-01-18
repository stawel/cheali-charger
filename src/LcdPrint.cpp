#include "LcdPrint.h"
#include "Hardware.h"
#include "memory.h"



void lcdSetCursor(uint8_t x, uint8_t y)
{
    lcd.setCursor(x, y);
}
void lcdSetCursor0_0()
{
    lcdSetCursor(0,0);
}
void lcdSetCursor0_1()
{
    lcdSetCursor(0,1);
}

void lcdClear()
{
    lcd.clear();
}


uint8_t lcdPrintSpaces()
{
    return lcdPrintSpaces(16);
}

uint8_t lcdPrintSpaces(uint8_t n)
{
    for(uint8_t i=0;i<n;i++)
        lcdPrintChar(' ');
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


void printUInt (char * &buf, uint8_t &maxSize, uint16_t value)
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

void lcdPrintUInt(uint16_t x)
{
    char buf[8];
    char *str = buf;
    uint8_t maxSize = 7;
    printUInt(str, maxSize, x);
    lcd.print(buf);
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
            buffer[i] = eeprom::read((uint8_t*)str + i);
            if(!buffer[i])
                break;
        }
        buffer[n] = 0;
        return lcd.print(buffer);
    }
    return 0;
}

namespace {
void lcdPrintInf(int8_t dig)
{
    for(; dig > 3; dig--)
        lcdPrintChar(' ');

    lcdPrint_P(PSTR("Inf"), dig);
}
}

void lcdPrintEValue(uint16_t x, int8_t dig, bool dot)
{
    const char prefix = ' ';
    if(dig<=0)
        return;

    int8_t xdig = digits(x/1000);
    xdig+=1+3; // m or .

    for(;xdig<dig;dig--)
        lcdPrintChar(prefix);

    if(dig >= xdig && !dot) {
        xdig = digits(x);
        dig--; //m
        for(;xdig<dig;dig--)
            lcdPrintChar(prefix);
        lcdPrintUInt(x);
        lcdPrintChar('m');
    } else if(dig < xdig - 4) {
        lcdPrintInf(dig);
    } else {
        lcdPrintUInt(x/1000);
        x%=1000;
        dig -= xdig - 4;
        if(dig-- >0) lcdPrintChar('.');
        if(dig-- >0) lcdPrintDigit(x/100);
        x%=100;
        if(dig-- >0) lcdPrintDigit(x/10);
        x%=10;
        if(dig-- >0) lcdPrintDigit(x);
    }
}

void lcdPrintTemperature_(AnalogInputs::ValueType x, int8_t dig)
{
    const char prefix = ' ';
    if(dig<=0)
        return;

    int8_t xdig = digits(x/100);
    xdig+=1+2; // .

    for(;xdig<dig;dig--)
        lcdPrintChar(prefix);

    if(dig < xdig - 3) {
        lcdPrintInf(dig);
    } else {
        lcdPrintUInt(x/100);
        x%=100;
        dig -= xdig - 3;
        if(dig-- >0) lcdPrintChar('.');
        if(dig-- >0) lcdPrintDigit(x/10);
        x%=10;
        if(dig-- >0) lcdPrintDigit(x);
    }
}

void lcdPrintTime(uint16_t timeSec)
{
    lcdPrintUnsigned(timeSec/60, 3, '0');
    lcdPrintChar(':');
    lcdPrintUnsigned(timeSec%60, 2, '0');
}


void lcdPrintYesNo(uint8_t yes)
{
    if(yes)
        lcdPrint_P(PSTR("yes"));
    else
        lcdPrint_P(PSTR(" no"));
}

void lcdPrintUnsigned(uint16_t x, int8_t dig)
{
    lcdPrintUnsigned(x,dig, ' ');
}

void lcdPrintChar(char c)
{
    lcd.print(c);
}


void lcdPrintDigit(uint8_t d)
{
    d%=10;
    lcdPrintChar('0'+ d);
}

void lcdPrintUnsigned_sign(uint16_t x, int8_t dig, const char prefix, bool minus)
{
    if(dig<=0)
        return;

    if(minus)
        dig--;

    uint8_t xdig = digits(x);

    for(;xdig<dig;dig--)
        lcdPrintChar(prefix);

    if(minus) lcdPrintChar('-');
    if(dig < xdig) {
        lcdPrintInf(dig);
    } else {
        lcdPrintUInt(x);
    }
}

void lcdPrintUnsigned(uint16_t x, int8_t dig, const char prefix)
{
    lcdPrintUnsigned_sign(x,dig,prefix, false);
}

void lcdPrintSigned(int16_t x, int8_t dig)
{
    bool minus = x < 0;
    if(minus) x=-x;
    lcdPrintUnsigned_sign(x,dig, ' ', minus);
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

void lcdPrintTemperature(AnalogInputs::ValueType t, int8_t dig)
{
    if(dig > 0) {
        lcdPrintTemperature_(t,--dig);
        lcdPrintChar('C');
    }
}
void lcdPrintPercentage(AnalogInputs::ValueType p, int8_t dig)
{
    if(dig > 0) {
        lcdPrintUnsigned(p,--dig);
        lcdPrintChar('%');
    }
}
void lcdPrintDeltaV(AnalogInputs::ValueType p, int8_t dig)
{
    if(dig > 2) {
        lcdPrintUnsigned(p,dig-2);
        lcdPrint_P(PSTR("mV"));
    }
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
        return lcdPrintTemperature(x, dig+1);
    case AnalogInputs::Resistance:
        dot = false;
        //TODO: ??Ohm
        unit ='!'-45;
        break;
    case AnalogInputs::Unknown:
        lcdPrintSigned(x, dig);
        lcdPrintChar(unit);
        return;
    case AnalogInputs::Charge:
        dot = false;
        dig--;
        unit ='h';
        break;
    }
    lcdPrintEValue(x, (int8_t) dig, dot);
    lcdPrintChar(unit);

    if(type == AnalogInputs::Charge)     lcdPrintChar('A');
}


