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
#include <stdint.h>
#include "Utils.h"
#include "AnalogInputsTypes.h"
#include "memory.h"

#define CP_TYPE_ANALOG_FLAG    64
#define CP_TYPE_ANALOG_MASK    63

#define CP_TYPE_ANALOG(x) (x | CP_TYPE_ANALOG_FLAG)

#ifdef SDCC_COMPILER
//TODO: sdcc remove
#define CP_TYPE_TEMPERATURE     CP_TYPE_ANALOG(Temperature)
#define CP_TYPE_UNSIGNED        CP_TYPE_ANALOG(Unsigned)
#define CP_TYPE_SIGNED_mV       CP_TYPE_ANALOG(SignedVoltage)
#define CP_TYPE_V               CP_TYPE_ANALOG(Voltage)
#define CP_TYPE_MINUTES         CP_TYPE_ANALOG(Minutes)
#define CP_TYPE_PROCENTAGE      CP_TYPE_ANALOG(Procent)
#define CP_TYPE_A               CP_TYPE_ANALOG(Current)
#define CP_TYPE_TEMP_MINUT      CP_TYPE_ANALOG(TemperatureMinutes)
#define CP_TYPE_CHARGE          CP_TYPE_ANALOG(ChargeUnit)
#define CP_TYPE_CHARGE_TIME     CP_TYPE_ANALOG(TimeLimitMinutes)
#define CP_TYPE_ON_OFF          CP_TYPE_ANALOG(YesNo)
#else
#define CP_TYPE_TEMPERATURE     CP_TYPE_ANALOG(AnalogInputs::Temperature)
#define CP_TYPE_UNSIGNED        CP_TYPE_ANALOG(AnalogInputs::Unsigned)
#define CP_TYPE_SIGNED_mV       CP_TYPE_ANALOG(AnalogInputs::SignedVoltage)
#define CP_TYPE_V               CP_TYPE_ANALOG(AnalogInputs::Voltage)
#define CP_TYPE_MINUTES         CP_TYPE_ANALOG(AnalogInputs::Minutes)
#define CP_TYPE_PROCENTAGE      CP_TYPE_ANALOG(AnalogInputs::Procent)
#define CP_TYPE_A               CP_TYPE_ANALOG(AnalogInputs::Current)
#define CP_TYPE_TEMP_MINUT      CP_TYPE_ANALOG(AnalogInputs::TemperatureMinutes)
#define CP_TYPE_CHARGE          CP_TYPE_ANALOG(AnalogInputs::ChargeUnit)
#define CP_TYPE_CHARGE_TIME     CP_TYPE_ANALOG(AnalogInputs::TimeLimitMinutes)
#define CP_TYPE_ON_OFF          CP_TYPE_ANALOG(AnalogInputs::YesNo)
#endif
#define CP_TYPE_UINT32_ARRAY    1
#define CP_TYPE_STRING_ARRAY    2
//TODO:??
#define CP_TYPE_METHOD          3

#define CPRINTF_METHOD(method) {CP_TYPE_METHOD, 0, {.methodPtr = method}}
//TODO: rename
#define EDIT_STRING_ARRAY(x)                {CP_TYPE_STRING_ARRAY,0, {.arrayPtr = &x}}
#define EDIT_UINT32_ARRAY(x)                {CP_TYPE_UINT32_ARRAY,0, {.arrayPtr = &x}}
#define CPRINTF_STRING_ARRAY_DATA(array, index)    {{.stringArrayPtr    = array}, &index}
#define CPRINTF_UINT32_ARRAY_DATA(array, index)    {{.uint32Ptr         = array}, &index}

namespace cprintf {

    struct ArrayData;
    typedef void(*CPrintfMethod)(int8_t);


    union Data {
        uint16_ptr uint16Ptr;
        uint16_t uint16;
        int16_ptr int16Ptr;
        const_uint32_ptr uint32Ptr;
        const_char_ptr const PROGMEM_PTR * stringArrayPtr;
        CPrintfMethod methodPtr;
        const PROGMEM_PTR struct ArrayData * arrayPtr;
    };

    struct ArrayData {
        union Data ArrayPtr;
        uint16_ptr indexPtr;
    };

    struct PrintData {
        uint8_t type;
        uint8_t size;
        union Data data;
    };

    void cprintf(const PROGMEM_PTR struct PrintData * p, uint8_t dig);

}

#endif /* CPRINTF_H_ */
