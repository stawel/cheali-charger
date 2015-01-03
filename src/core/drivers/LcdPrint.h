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

void print_P(char * &buf, uint8_t &maxSize, const char *str);
void print(char * &buf, uint8_t &maxSize, const char *str);
void printChar(char * &buf, uint8_t &maxSize, char chr);
void printUInt(char * &buf, uint8_t &maxSize, uint16_t value);
void printULong(char * &buf, uint8_t &maxSize, uint32_t value);

#ifdef ENABLE_LCD_RAM_CG
void lcdCreateCGRam();
#endif

void lcdSetCursor(uint8_t x, uint8_t y);
void lcdSetCursor0_0();
void lcdSetCursor0_1();
void lcdClear();

uint8_t lcdPrint_P(const char * const str[], uint8_t index);
uint8_t lcdPrint_P(const char *str);
uint8_t lcdPrint_E(const char *str, uint8_t n);
uint8_t lcdPrint  (const char *str, uint8_t n);
uint8_t lcdPrintSpace1();
uint8_t lcdPrintSpaces();
uint8_t lcdPrintSpaces(uint8_t n);

void lcdPrintUnsigned(uint16_t x, int8_t dig, const char prefix);
void lcdPrintUnsigned(uint16_t x, int8_t dig = 5);
void lcdPrintSigned(int16_t x, int8_t dig = 5);
void lcdPrintChar(char c);
void lcdPrintDigit(uint8_t d);
void lcdPrintUInt(uint16_t x);
void lcdPrintYesNo(uint8_t yes);

void lcdPrintValue1000(uint16_t x, int8_t dig, bool dot);

void lcdPrintTemperature(AnalogInputs::ValueType c, int8_t dig);
void lcdPrintCharge(AnalogInputs::ValueType c, int8_t dig);
void lcdPrintCurrent(AnalogInputs::ValueType i, int8_t dig);
void lcdPrintVoltage(AnalogInputs::ValueType v, int8_t dig);
void lcdPrintResistance(AnalogInputs::ValueType r, int8_t dig);
void lcdPrintPercentage(AnalogInputs::ValueType p, int8_t dig);
void lcdPrint_mV(int16_t p, int8_t dig);
void lcdPrintAnalog(AnalogInputs::ValueType x, AnalogInputs::Type type, int8_t dig);

void lcdPrintTime(uint16_t timeSec);

#endif /* LCDPRINT_H_ */
