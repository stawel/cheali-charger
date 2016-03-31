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
#define __STDC_LIMIT_MACROS
#include "Utils.h"
#include "Hardware.h"
#include "AnalogInputs.h"
#include "memory.h"

void callVoidMethod_P(const PROGMEM_PTR VoidMethod * method)
{
    VoidMethod voidMethod;
    pgm_read(voidMethod, method);
    callVoidMethod(voidMethod);
}
void callVoidMethod(const VoidMethod method)
{
    method();
}


uint16_t pow10(uint8_t n)
{
    uint16_t retu = 1;
    while(n--) {
        retu*=10;
    }
    return retu;
}

int8_t sign(int16_t x)
{
    if(x == 0) return 0;
    if(x >  0) return 1;
    return -1;
}

uint8_t countBits(uint16_t v)
{
    uint8_t i, bits = 0;
    for(i = 0; i < 16; i++) {
        if(v&1) bits++;
        v>>=1;
    }
    return bits;
}

uint8_t digits(uint16_t x)
{
    return digits((int32_t)x);
}

uint8_t digits(int32_t x)
{
    uint8_t retu = 0;
    if(x < 0) {
        retu++;
        x = -x;
    }
    if(x == 0)
        retu=1;
    for(;x!=0; x/=10)
        retu++;
    return retu;
}

void change0ToMax(uint16_ptr v, int dir, uint8_t max)
{
    changeMinToMaxStep(v, dir, 0, max, 1);
}

void change1ToMax(uint16_ptr v, int dir, uint8_t max)
{
    changeMinToMaxStep(v, dir, 1, max, 1);
}

void change0ToInfSmart(uint16_ptr v, int dir)
{
    changeMinToMaxSmart(v, dir, 0, UINT16_MAX);
}

void changeMinToMaxSmart(uint16_ptr v, int dir, uint16_t min, uint16_t max)
{
    uint8_t dv = digits(*v);
    uint16_t step = 1;
    if(dv>1) step = pow10(dv-2);
    changeMinToMaxStep(v, dir, min, max, step);
}

void changeMinToMaxStep(uint16_ptr v, int dir, uint16_t min, uint16_t max, uint16_t step)
{
    uint16_t r = (*v)%step;

    if(r) {
        if(dir > 0) step -=r;
        else step = r;
    }
    if(dir > 0) ADD_MAX(*v, step, max);
    else SUB_MIN(*v, step, min);
}

uint8_t waitButtonPressed()
{
    uint8_t key;

    while(Keyboard::getPressedWithDelay() != BUTTON_NONE);

    do {
        key = Keyboard::getPressedWithDelay();
    } while(key == BUTTON_NONE);

    return key;
}
