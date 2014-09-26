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

#include "Utils.h"
#include "Hardware.h"
#include "memory.h"

uint8_t countElements(const char * const* staticMenu)
{
    uint8_t retu=0;
    while(pgm::read(staticMenu++))
        retu++;

    return retu;
}


bool testTintern(bool &more, AnalogInputs::ValueType off, AnalogInputs::ValueType on)
{
    AnalogInputs::ValueType t = AnalogInputs::getRealValue(AnalogInputs::Tintern);

    bool retu = false;
    if(t < off) {
        retu = true;
        more =  false;
    } else if(t > on) {
        retu = true;
        more = true;
    }
    return retu;
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
    if(x>=0) return 1;
    return -1;
}

uint8_t digits(uint32_t x)
{
    uint8_t retu = 0;
    if(x == 0)
        x=1;
    for(;x!=0; x/=10)
        retu++;
    return retu;
}

void change0ToMax(uint16_t &v, int dir, uint8_t max)
{
    if( ((int)v) + dir< 0) v = 0;
    else if(((int)v)+dir > max) v = max;
    else v+=dir;
}

void change1ToMax(uint16_t &v, int dir, uint8_t max)
{
    if( ((int)v) + dir< 1) v = 1;
    else if(((int)v)+dir > max) v = max;
    else v+=dir;
}


void change0ToMaxSmart(uint16_t &v, int dir, uint16_t max)
{
    return change0ToMaxSmart(v, dir, max, 0, 0);
}



void change100ToMaxSmart(uint16_t &v, int dir, uint16_t max)
{
    return change0ToMaxSmart(v, dir, max, 0, 100);
}


void change0ToMaxSmart(uint16_t &v, int dir, uint16_t max, int16_t step, uint8_t starting)
{
    uint16_t r;

    uint8_t dv = digits(v);
    if(step == 0) {
        step = 1;
        if(dv>1) step = pow10(dv-2);
    }

    r = v%step;

    if(r) {
        if(dir > 0) step -=r;
        else step = r;
    }
    if(dir > 0) ADD_MAX(v, step, max);
    else SUB_MIN(v, step ,starting);
}

void waitButtonPressed()
{
    while(Keyboard::getPressedWithSpeed() != BUTTON_NONE);
    while(Keyboard::getPressedWithSpeed() == BUTTON_NONE);
}
