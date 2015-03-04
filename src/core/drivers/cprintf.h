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
#define CP_TYPE_VOID           13
#define CP_TYPE_CHARGE		   14
#define CP_TYPE_CHARGE_TIME    15
#define CP_TYPE_STRING         16
#define CP_TYPE_NEWLINE        17
#define CP_TYPE_CHAR           18
#define CP_TYPE_CHAIN          19
#define CP_TYPE_SPACES_END     20
#define CP_TYPE_TIME           21
#define CP_TYPE_R              22
#define CP_TYPE_P              23
#define CP_TYPE_E              24

#define CP_TYPE_METHOD        128


namespace cprintf {

    struct ArrayData;
    struct PrintData;

    union Data {
        const void * voidPtr;
        uint16_t * uint16Ptr;
        uint16_t uint16;
        int16_t  * int16Ptr;
        uint32_t * uint32Ptr;
        const char * charPtr;
        const char * const* stringArrayPtr;
        VoidMethod voidMethodPtr;
        Uint16Method uint16MethodPtr;
        PrintData * printDataPtr;
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


#endif /* CPRINTF_H_ */
