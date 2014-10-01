/*
    cheali-charger - open source firmware for a variety of LiPo chargers
    Copyright (C) 2013  Paweł Stawicki. All right reserved.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "LcdPrint.h"
#include "Hardware.h"
#include "memory.h"

using namespace lcd_print;

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
        uint8_t s = pgm::strlen(str);
        if(s>maxSize) s = maxSize;
        pgm::strncpy(buf, str, s);
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

void printUInt(char * &buf, uint8_t &maxSize, uint16_t value)
{
    //TODO: use printULong - for some reason the program size is smaller now!?
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


void printULong(char * &buf, uint8_t &maxSize, uint32_t value)
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
            uint32_t m = value;
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
    lcdPrint(buf, 8);
}


uint8_t lcdPrint(const char *str, uint8_t size)
{
    uint8_t n = 0;
    if(str) {
        while(n < size) {
            n++;
            char c = *str;
            str++;
            if (c == 0) {
                break;
            }
            lcdPrintChar(c);
        }
    }
    return n;
}


uint8_t lcdPrint_P(const char *str)
{
    uint8_t n = 0;
    if(str) {
        while(true) {
            n++;
            char c = pgm::read(str);
            str++;
            if (c == 0) {
                break;
            }
            lcdPrintChar(c);
        }
    }
    return n;
}

uint8_t lcdPrint_E(const char *str, uint8_t size)
{
    uint8_t n = 0;
    if(str) {
        while(n < size) {
            n++;
            char c = eeprom::read((const uint8_t*)str);
            str++;
            if (c == 0) {
                break;
            } else {
                lcdPrintChar(c);
            }
        }
    }
    return n;
}

namespace {
void lcdPrintInf(int8_t dig)
{
    for(; dig > 3; dig--)
        lcdPrintChar(' ');

    lcdPrint_P(string_infinity + 3 - dig);
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
    const char * str = string_no;
    if(yes) str = string_yes;

    lcdPrint_P(str);
}

void lcdPrintUnsigned(uint16_t x, int8_t dig)
{
    lcdPrintUnsigned(x,dig, ' ');
}

void lcdPrintChar(char c)
{
    if(c == '\n') {
        lcdSetCursor0_1();
    } else {
        lcd.print(c);
    }
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
void lcdPrint_mV(AnalogInputs::ValueType p, int8_t dig)
{
    if(dig > 2) {
        lcdPrintUnsigned(p,dig-2);
        lcdPrint_P(string_unit_mV);
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
    case AnalogInputs::Power:
        return lcdPrintPower(x,'W',' ');
        break;
    case AnalogInputs::Work:
        return lcdPrintPower(x,'W','h');
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
        unit ='A';
        break;
    }
    lcdPrintEValue(x, (int8_t) dig, dot);
    lcdPrintChar(unit);

    if(type == AnalogInputs::Charge)     lcdPrintChar('h');
}

#ifdef ENABLE_LCD_RAM_CG
void lcdCreateCGRam()
{
   uint8_t CGRAM[8];
   CGRAM[0] = 0b01110;
   CGRAM[1] = 0b11111;
   CGRAM[2] = 0b10001;
   CGRAM[3] = 0b10001;
   CGRAM[4] = 0b10001;
   CGRAM[5] = 0b10001;
   CGRAM[6] = 0b10001;
   CGRAM[7] = 0b11111;
   lcd.createChar(0, CGRAM); //empty
   
   CGRAM[0] = 0b01110;
   CGRAM[1] = 0b11111;
   CGRAM[2] = 0b10001;
   CGRAM[3] = 0b10001;
   CGRAM[4] = 0b11111;
   CGRAM[5] = 0b11111;
   CGRAM[6] = 0b11111;
   CGRAM[7] = 0b11111;           
   lcd.createChar(1, CGRAM); //half
   
   CGRAM[0] = 0b01110;
   CGRAM[1] = 0b11111;
   CGRAM[2] = 0b11111;
   CGRAM[3] = 0b11111;
   CGRAM[4] = 0b11111;
   CGRAM[5] = 0b11111;
   CGRAM[6] = 0b11111;
   CGRAM[7] = 0b11111;           
   lcd.createChar(2, CGRAM); //full      
}
#endif


void lcdPrintPower(AnalogInputs::ValueType x, char u1, char u2)
{
    const char prefix = ' ';
    x /=100;
    if(x<10000) lcdPrintChar(prefix);
    if(x<1000) lcdPrintChar(prefix);
    if(x<100) lcdPrintChar(prefix);
    if(x<10) lcdPrintChar(prefix);
    
    lcdPrintUInt(x);
     lcdPrintChar(u1);
     lcdPrintChar(u2);
     lcdPrintChar(prefix);
    
}
