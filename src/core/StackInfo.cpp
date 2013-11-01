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

#include "StackInfo.h"

#ifdef ENABLE_STACK_INFO

extern int __heap_start, *__brkval;

void StackInfo::initialize()
{
    volatile uint8_t * v = (uint8_t *)(__brkval == 0 ?  &__heap_start : __brkval);
    volatile uint8_t data;
    for( ; v <= &data; v++ ) {
        *v = STACK_INFO_MAGIC_NUMBER;
    }
}


uint16_t StackInfo::getFreeStackSize()
{
    int v;
    return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

uint16_t StackInfo::getNeverUsedStackSize()
{
    volatile uint8_t * v = (uint8_t*)(__brkval == 0 ?  &__heap_start : __brkval);
    volatile uint8_t data;
    uint16_t size = 0;
    for(; v <= &data && *v == STACK_INFO_MAGIC_NUMBER; v++ ) {
        size++;
    }
    return size;
}

#endif
