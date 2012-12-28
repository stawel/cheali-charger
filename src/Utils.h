#ifndef UTILS_H_
#define UTILS_H_

#include "Hardware.h"

#define sizeOfArray(x) (sizeof(x)/sizeof((x)[0]))

uint8_t selectIndexWithKeyboard(uint8_t &x, const uint8_t tab_size);

bool testTinern(bool &more, AnalogInputs::ValueType off, AnalogInputs::ValueType on);
uint16_t pow10(uint8_t n);
uint8_t digits(uint16_t x);
int8_t sign(int16_t x);

#define ADD_MAX(x,v,max) (x) = (x)<(max)-(v)? (x)+(v) : (max)
#define SUB_MIN(x,v,min) (x) = (x)>(min)+(v)? (x)-(v) : (min)

void changeMax(uint16_t &v, int direc, uint16_t max);

#endif /* UTILS_H_ */
