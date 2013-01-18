#ifndef UTILS_H_
#define UTILS_H_

#include "Hardware.h"

#define sizeOfArray(x) (sizeof(x)/sizeof((x)[0]))

#define ADD_MAX(x,v,max) (x) = (x)<(max)-(v)? (x)+(v) : (max)
#define SUB_MIN(x,v,min) (x) = (x)>(min)+(v)? (x)-(v) : (min)

bool testTintern(bool &more, AnalogInputs::ValueType off, AnalogInputs::ValueType on);
uint16_t pow10(uint8_t n);
uint8_t digits(uint16_t x);
int8_t sign(int16_t x);

void changeMaxSmart(uint16_t &v, int direc, uint16_t max);
void changeMax(uint16_t &v, int direc, uint8_t max);
void change1Max(uint16_t &v, int direc, uint8_t max);

void waitButtonPressed();

#endif /* UTILS_H_ */
