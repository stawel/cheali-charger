#ifndef LCDPRINT_H_
#define LCDPRINT_H_

#include <stddef.h>
#include "Hardware.h"
#include "AnalogInputs.h"

uint8_t lcdPrint_P(const char *str);
uint8_t lcdPrint_P(const char *str, uint8_t n);
uint8_t lcdPrint_E(const char *str, uint8_t n);
uint8_t lcdPrint  (const char *str, uint8_t n);
uint8_t lcdPrintSpaces();
uint8_t lcdPrintSpaces(uint8_t n);

void lcdPrintUnsigned(uint16_t x, int8_t dig = 5);
void lcdPrintSigned(int16_t x, int8_t dig = 5);

void lcdPrintCharge(AnalogInputs::ValueType c, int8_t dig);
void lcdPrintCurrent(AnalogInputs::ValueType i, int8_t dig);
void lcdPrintVoltage(AnalogInputs::ValueType v, int8_t dig);
void lcdPrintResistance(AnalogInputs::ValueType r, int8_t dig);
void lcdPrintAnalog(AnalogInputs::ValueType x, AnalogInputs::Type type, int8_t dig);

#endif /* LCDPRINT_H_ */
