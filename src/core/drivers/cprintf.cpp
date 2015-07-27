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


#include "cprintf.h"
#include "memory.h"
#include "LcdPrint.h"
#include "ProgramData.h"

namespace cprintf {


void cprintf(const PrintData * printDataPtr, uint8_t dig)
{
    const PrintData p = pgm::read(printDataPtr);
    if(p.type == CP_TYPE_METHOD) {
        //Info: this must be before: uvalue = *p.data.uint16Ptr
        return p.data.methodPtr(dig);
    }
    if(p.type & CP_TYPE_ANALOG_FLAG) {
        lcdPrintAnalog(*p.data.uint16Ptr, dig, AnalogInputs::Type(p.type & CP_TYPE_ANALOG_MASK));
    } else {
        uint32_t v;
        uint8_t i;
        ArrayData array;
        const char * strPtr;
        switch(p.type) {
            case CP_TYPE_UINT32_ARRAY:
                pgm::read(array, p.data.arrayPtr);
                v = pgm::read(&array.ArrayPtr.uint32Ptr[*array.indexPtr]);
                v/=100;
                lcdPrintUnsigned(v, dig-2);
                lcdPrintChar('0');
                lcdPrintChar('0');
                break;

            case CP_TYPE_STRING_ARRAY:
                pgm::read(array, p.data.arrayPtr);
                strPtr = pgm::read(&array.ArrayPtr.stringArrayPtr[*array.indexPtr]);
                i = pgm::strlen(strPtr);
                lcdPrintSpaces(dig-i);
                lcdPrint_P(array.ArrayPtr.stringArrayPtr, *array.indexPtr);
                break;
        }
    }
}




} //cprintf
