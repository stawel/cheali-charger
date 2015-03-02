/*
    cheali-charger - open source firmware for a variety of LiPo chargers
    Copyright (C) 2015  Paweł Stawicki. All right reserved.

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
#ifndef CPRINTF_H_
#define CPRINTF_H_

#include <stddef.h>
#include <inttypes.h>
#include <Utils.h>

#define CP_TYPE_END             0
#define CP_TYPE_TEMPERATURE     1
#define CP_TYPE_UNSIGNED        2
#define CP_TYPE_STRING_ARRAY    3
#define CP_TYPE_SIGNED_mV       4
#define CP_TYPE_V               5
#define CP_TYPE_MINUTES         6
#define CP_TYPE_PROCENTAGE      7
#define CP_TYPE_A               8
#define CP_TYPE_V2              9
#define CP_TYPE_ON_OFF         10
#define CP_TYPE_TEMP_MINUT     11
#define CP_TYPE_UINT32_ARRAY   12
#define CP_TYPE_METHOD         13
#define CP_TYPE_CHARGE		   14
#define CP_TYPE_CHARGE_TIME    15
#define CP_TYPE_STRING         16
#define CP_TYPE_NEWLINE        17

#define CPRINTF_METHOD(method)      {CP_TYPE_METHOD, 0, {.methodPtr = method}}
#define CPRINTF_STRING(string)      {CP_TYPE_STRING, 0, {.charPtr = string_ ## string}}
#define CPRINTF_END                 {CP_TYPE_END, 0, 0}
#define CPRINTF_NEWLINE             {CP_TYPE_NEWLINE, 0, 0}
#define CPRINTF_VOLT(value, size)   {CP_TYPE_V, size, &value}

namespace cprintf {

    struct ArrayData;

    union Data {
        const void * voidPtr;
        uint16_t * uint16Ptr;
        uint16_t uint16;
        int16_t  * int16Ptr;
        uint32_t * uint32Ptr;
        const char * charPtr;
        const char * const* stringArrayPtr;
        VoidMethod methodPtr;
        ArrayData * arrayPtr;
    };

    struct ArrayData {
        Data ArrayPtr;
        uint16_t * indexPtr;
    };

    struct PrintData {
        uint8_t type;
        uint8_t size;
        Data data;
    };

    uint8_t cprintf(const PrintData * p, uint8_t dig);
    void cprintf(const PrintData * p);
}


//TODO: move to cprintfAnalogInputs.h ??

#include "AnalogInputs.h"
#include "AnalogInputsPrivate.h"
#define CPRINTF_ANALOG_REAL_V(name, size) {CP_TYPE_V, size, &AnalogInputs::real_[name]}


#endif /* CPRINTF_H_ */
