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
#include "AnalogInputsTypes.h"

#define CP_TYPE_ANALOG_FLAG    64
#define CP_TYPE_ANALOG_MASK    63

#define CP_TYPE_ANALOG(x) (x | CP_TYPE_ANALOG_FLAG)

#define CP_TYPE_TEMPERATURE     CP_TYPE_ANALOG(AnalogInputs::Temperature)
#define CP_TYPE_UNSIGNED        CP_TYPE_ANALOG(AnalogInputs::Unsigned)
#define CP_TYPE_SIGNED_mV       CP_TYPE_ANALOG(AnalogInputs::SignedVoltage)
#define CP_TYPE_V               CP_TYPE_ANALOG(AnalogInputs::Voltage)
#define CP_TYPE_MINUTES         CP_TYPE_ANALOG(AnalogInputs::Minutes)
#define CP_TYPE_PROCENTAGE      CP_TYPE_ANALOG(AnalogInputs::Procent)
#define CP_TYPE_A               CP_TYPE_ANALOG(AnalogInputs::Current)
#define CP_TYPE_TEMP_MINUT      CP_TYPE_ANALOG(AnalogInputs::TemperatureMinutes)
#define CP_TYPE_CHARGE		    CP_TYPE_ANALOG(AnalogInputs::Charge)
#define CP_TYPE_CHARGE_TIME     CP_TYPE_ANALOG(AnalogInputs::TimeLimitMinutes)
#define CP_TYPE_ON_OFF          CP_TYPE_ANALOG(AnalogInputs::YesNo)
#define CP_TYPE_UINT32_ARRAY    1
#define CP_TYPE_STRING_ARRAY    2
//TODO:??
#define CP_TYPE_METHOD          3


#define CPRINTF_METHOD(method) {CP_TYPE_METHOD, 0, {.methodPtr = method}}
namespace cprintf {

    struct ArrayData;
    typedef void(*PrintMethod)(int8_t);


    union Data {
        const void * voidPtr;
        uint16_t * uint16Ptr;
        uint16_t uint16;
        int16_t  * int16Ptr;
        uint32_t * uint32Ptr;
        const char * const* stringArrayPtr;
        PrintMethod methodPtr;
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

    void cprintf(const PrintData * p, uint8_t dig);

}

#endif /* CPRINTF_H_ */
