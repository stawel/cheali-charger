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


void lcdSetCursor(uint8_t x, uint8_t y);
void lcdSetCursor0_0();
void lcdSetCursor0_1();
void lcdClear();

uint8_t lcdPrint_P(const char *str);
uint8_t lcdPrint_P(const char *str, uint8_t n);
uint8_t lcdPrint_E(const char *str, uint8_t n);
uint8_t lcdPrint  (const char *str, uint8_t n);
uint8_t lcdPrintSpaces();
uint8_t lcdPrintSpaces(uint8_t n);

void lcdPrintUnsigned(uint16_t x, int8_t dig, const char prefix);
void lcdPrintUnsigned(uint16_t x, int8_t dig = 5);
void lcdPrintSigned(int16_t x, int8_t dig = 5);
void lcdPrintChar(char c);
void lcdPrintDigit(uint8_t d);
void lcdPrintUInt(uint16_t x);

void lcdPrintTemperature(AnalogInputs::ValueType c, int8_t dig);
void lcdPrintCharge(AnalogInputs::ValueType c, int8_t dig);
void lcdPrintCurrent(AnalogInputs::ValueType i, int8_t dig);
void lcdPrintVoltage(AnalogInputs::ValueType v, int8_t dig);
void lcdPrintResistance(AnalogInputs::ValueType r, int8_t dig);
void lcdPrintAnalog(AnalogInputs::ValueType x, AnalogInputs::Type type, int8_t dig);

void lcdPrintTime(uint16_t timeSec);

#endif /* LCDPRINT_H_ */
