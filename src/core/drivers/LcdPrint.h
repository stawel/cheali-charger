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
#ifndef LCDPRINT_H_
#define LCDPRINT_H_

#include <stddef.h>
#include "Hardware.h"
#include "AnalogInputs.h"
#include "Utils.h"

char* printLong(int32_t value, char * buf);

#ifdef ENABLE_LCD_RAM_CG
void lcdCreateCGRam();
#endif

void lcdSetCursor(uint8_t x, uint8_t y);
void lcdSetCursor0_0();
void lcdSetCursor0_1();
void lcdClear();


void lcdPrintUInt(uint16_t x);
void lcdPrintLong(int32_t t, uint8_t dig);


int8_t lcdPrint_P(const char * const str[], uint8_t index);
int8_t lcdPrint_P(const char *str);
int8_t lcdPrint  (const char *str, int8_t n);
void lcdPrintR(const char *str, int8_t n);
int8_t lcdPrintSpace1();
int8_t lcdPrintSpaces();
int8_t lcdPrintSpaces(int8_t n);

void lcdPrintUnsigned(uint16_t x, int8_t dig, const char prefix);
void lcdPrintUnsigned(uint16_t x, int8_t dig = 5);
void lcdPrintSigned(int16_t x, int8_t dig = 5);
void lcdPrintChar(char c);
int8_t lcdPrintChar(char c, int8_t dig);
void lcdPrintDigit(uint8_t d);
void lcdPrintYesNo(uint8_t yes, int8_t dig);

void lcdPrintTemperature(AnalogInputs::ValueType c, int8_t dig);
void lcdPrintCharge(AnalogInputs::ValueType c, int8_t dig);
void lcdPrintCurrent(AnalogInputs::ValueType i, int8_t dig);
void lcdPrintVoltage(AnalogInputs::ValueType v, int8_t dig);
void lcdPrintResistance(AnalogInputs::ValueType r, int8_t dig);
void lcdPrintPercentage(AnalogInputs::ValueType p, int8_t dig);
void lcdPrint_mV(int16_t p, int8_t dig);
void lcdPrintAnalog(AnalogInputs::ValueType x, int8_t dig, AnalogInputs::Type type);

void lcdPrintTime(uint16_t timeSec);

#endif /* LCDPRINT_H_ */
