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

using namespace AnalogInputs;

char* printLong(int32_t value, char * buf) {
    char * end;
    if (value < 0) {
        *(buf++)='-';
        value = -value;
    }
    buf += digits((int32_t)value);
    *buf = 0;
    end = buf;
    buf--;
    do {
        *buf = (value%10) + '0';
        value /= 10;
        buf--;
    } while(value);
    return end;
}

void lcdSetCursor(uint8_t x, uint8_t y) { lcd.setCursor(x, y); }
void lcdSetCursor0_0() { lcdSetCursor(0,0); }
void lcdSetCursor0_1() { lcdSetCursor(0,1); }
void lcdClear() { lcd.clear(); }

int8_t lcdPrintSpace1() {   return lcdPrintSpaces(1); }
int8_t lcdPrintSpaces() {   return lcdPrintSpaces(16);}
int8_t lcdPrintSpaces(int8_t n)
{
    return lcdPrintChar(' ', n);
}

void lcdPrintUInt(uint16_t x)
{
    char buf[11];
    printLong(x, buf);
    lcdPrint(buf, 11);
}

void lcdPrintLong(int32_t x, int8_t dig)
{
    char buf[11];
    printLong(x, buf);
    lcdPrintR(buf, dig);
}


void lcdPrintR(const char *str, int8_t size)
{
    uint8_t str_size = strlen(str);
    lcdPrintSpaces(size - str_size);
    lcdPrint(str, size);
}


int8_t lcdPrint(const char *str, int8_t size)
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

int8_t lcdPrint_P(const char * const str[], uint8_t index) {
    return lcdPrint_P(pgm::read(&str[index]));
}


int8_t lcdPrint_P(const char *str)
{
    int8_t n = 0;
    char c;
    if(str) {
        while((c = pgm::read(str++)) != 0) {
            lcdPrintChar(c);
            n++;
        }
    }
    return n;
}

static uint16_t div10(uint16_t div) {
    if(div <= 100)
        return 100;
    return 1000;
}

void lcdPrintValue_(uint16_t x, int8_t dig, uint16_t div, bool mili, bool minus)
{
    char buf[12];
    char *end, *dot_char;
    int32_t t;

    uint8_t size;

    if(mili) {
        t = x;
        t *= 1000;
        t /= div;
        if(minus) {
            t = -t;
        }
        size = digits(t) + 1;
        if(size <= dig) {
            dig--;
            lcdPrintLong(t, dig);
            lcdPrintChar('m');
            return;
        }
    }

    end = buf;

    t = x / div;
    if(minus) {
        *(end++) = '-';
    }
    end = printLong(t, end);
    if(div > 1 && end - buf < dig -1) {
        dot_char = end;
        t = x % div;
        t += div;
        t *= div10(div);
        t /= div;
        printLong(t, end);
        *dot_char = '.';
    }

    lcdPrintR(buf, dig);

}

void lcdPrintTime(uint16_t timeSec)
{
    lcdPrintUnsigned(timeSec/60, 3, '0');
    lcdPrintChar(':');
    lcdPrintUnsigned(timeSec%60, 2, '0');
}


void lcdPrintYesNo(uint8_t yes, int8_t dig)
{
    lcdPrintSpaces(dig - 3);

    const char * str = string_no;
    if(yes) str = string_yes;
    lcdPrint_P(str);
}


void lcdPrintChar(char c)
{
    if(c == '\n') {
        lcdSetCursor0_1();
    } else {
        lcd.print(c);
    }
}

int8_t lcdPrintChar(char c, int8_t dig)
{
    int8_t i = 0;
    for (; i < dig; i++)
        lcdPrintChar(c);
    return i;
}


void lcdPrintDigit(uint8_t d)
{
    lcdPrintChar('0'+ d);
}


void lcdPrintUnsigned(uint16_t x, int8_t dig, const char prefix)
{

    uint8_t s = digits(x);
    lcdPrintChar(prefix, dig - s);
    lcdPrintUInt(x);
}

void lcdPrintUnsigned(uint16_t x, int8_t dig)
{
    lcdPrintLong(x, dig);
}

void lcdPrintSigned(int16_t x, int8_t dig)
{
    lcdPrintLong(x,dig);
}

void lcdPrintTemperature(AnalogInputs::ValueType t, int8_t dig)
{
    lcdPrintAnalog(t, dig, AnalogInputs::Temperature);
}

void lcdPrintCharge(AnalogInputs::ValueType c, int8_t dig)
{
    lcdPrintAnalog(c, dig, AnalogInputs::Charge);
}
void lcdPrintCurrent(AnalogInputs::ValueType i, int8_t dig)
{
    lcdPrintAnalog(i, dig, AnalogInputs::Current);
}

void lcdPrintVoltage(AnalogInputs::ValueType v, int8_t dig)
{
    lcdPrintAnalog(v, dig, AnalogInputs::Voltage);
}

void lcdPrintResistance(AnalogInputs::ValueType r, int8_t dig)
{
    lcdPrintAnalog(r, dig, AnalogInputs::Resistance);
}

void lcdPrintPercentage(AnalogInputs::ValueType p, int8_t dig)
{
    lcdPrintAnalog(p, dig, AnalogInputs::Procent);
}

void lcdPrint_mV(int16_t p, int8_t dig)
{
    lcdPrintAnalog(p, dig, AnalogInputs::SignedVoltage);
}

struct Info {
    uint16_t div;
    bool mili;
    const char * symbol;
};
static const Info info[] PROGMEM = {
        // Current
        {ANALOG_AMP(1.000), true, string_A},
        //Voltage,
        {ANALOG_VOLT(1.000), false, string_V},
        //Power,
        {ANALOG_WATT(1.00), false,string_W},
        //Work,
        {ANALOG_WATTH(1.00), false,string_Wh},
        //Temperature,
        {ANALOG_CELCIUS(1.00), false, string_C},
        //Charge,
        {ANALOG_CHARGE(1.000), true,string_Ah},
        //Resistance,
        {ANALOG_OHM(1.000), true, string_Ohm},
        //Procent,
        {1, false, string_procent},
        //SignedVoltage,
        {ANALOG_VOLT(1.000), true, string_V},
        //Unsigned
        {1, false, string_unsigned},
        //TemperatureMinutes,
        {ANALOG_CELCIUS(1.00), false, string_C_m},
        //Minutes
        {1, false, string_minutes},
        //TimeLimitMinutes,
        {1, false, string_minutes},
        //YesNo
        {1, false, NULL},
        //Unknown
        {1, false, string_unknown},
};


void lcdPrintAnalog(AnalogInputs::ValueType x, int8_t dig, AnalogInputs::Type type)
{
    STATIC_ASSERT(sizeOfArray(info) -1 == AnalogInputs::Unknown);

    if(type == AnalogInputs::YesNo) {
        lcdPrintYesNo(x, dig);
    } else if(  (type == AnalogInputs::TimeLimitMinutes && x >= ANALOG_MAX_TIME_LIMIT)
       ||(type == AnalogInputs::Charge && x >= ANALOG_MAX_CHARGE)) {
            lcdPrintSpaces(dig - string_size_unlimited + 1);
            //TODO: programData::
            lcdPrint_P(string_unlimited);
    } else {
        const char * symbol = pgm::read(&info[type].symbol);
        uint8_t symbol_size = pgm::strlen(symbol);

        dig -= symbol_size;
        if(dig <= 0)
            return;

        bool sign = false;
        if(type == AnalogInputs::SignedVoltage) {
            int16_t y = x;
            if(y < 0) {
                sign = true;
                x = -y;
            }
        }

        lcdPrintValue_(x, (int8_t) dig, pgm::read(&info[type].div), pgm::read(&info[type].mili), sign);
        lcdPrint_P(symbol);
    }
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

