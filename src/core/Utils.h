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
#ifndef UTILS_H_
#define UTILS_H_

#include "AnalogInputsTypes.h"

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

#define sizeOfArray(x) (sizeof(x)/sizeof((x)[0]))

#define ADD_MAX(x,v,max) (x) = (x)<(max)-(v)? (x)+(v) : (max)
#define SUB_MIN(x,v,min) (x) = (x)>(min)+(v)? (x)-(v) : (min)

//info: I use __ COUNTER__ which is not supported by some compilers
#define START_CASE_COUNTER const uint8_t _case_counter = __COUNTER__
#define START_CASE_COUNTER_FROM(x) const uint8_t _case_counter = __COUNTER__ - (x)
#define NEXT_CASE (__COUNTER__ - _case_counter - 1)

//assert
#if __cplusplus <= 199711L
#define STATIC_ASSERT(x) typedef char __STATIC_ASSERT__[( x )?1:-1] __attribute__((unused))
#define STATIC_ASSERT_MSG(x, msg) STATIC_ASSERT(x)
#else
#define STATIC_ASSERT(x) static_assert(x, #x)
#define STATIC_ASSERT_MSG(x, msg) static_assert(x, msg)
#endif
//Preprocessor: concatenate int to string
#define CHEALI_CHARGER_STRING2(x)   #x
#define CHEALI_CHARGER_STRING(x)    CHEALI_CHARGER_STRING2(x)

typedef void(*VoidMethod)();
void callVoidMethod_P(const VoidMethod * method);
void callVoidMethod(const VoidMethod method);


bool testTintern(bool &more, AnalogInputs::ValueType off, AnalogInputs::ValueType on);
uint16_t pow10(uint8_t n);
uint8_t digits(int32_t x);
uint8_t digits(uint16_t x);
int8_t sign(int16_t x);
uint8_t countBits(uint16_t v);

void change0ToInfSmart(uint16_t *v, int dir);
void changeMinToMaxSmart(uint16_t *v, int dir, uint16_t min, uint16_t max);
void changeMinToMaxStep(uint16_t *v, int dir, uint16_t min, uint16_t max, uint16_t step);
void change0ToMax(uint16_t *v, int dir, uint8_t max);
void change1ToMax(uint16_t *v, int dir, uint8_t max);

uint8_t waitButtonPressed();

uint8_t countElements(const void * const array[]);
template<typename T>
uint8_t countElements(const T array[]) {return countElements((const void * const *)array); }

// Platform specific delays. Implemented in Utils.cpp located in platform folder
namespace Utils
{
    void delayTenMicroseconds(uint16_t value);
    void delayMicroseconds(uint16_t value);
    void delayMilliseconds(uint16_t value);
}

#endif /* UTILS_H_ */
