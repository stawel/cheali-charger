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

using namespace programData;

namespace cprintf {




uint8_t cprintf_size(const PrintData printDataPtr[], uint8_t size) {
    uint8_t type;
    do {
        type = pgm::read(&printDataPtr->type);
        size -= cprintf(printDataPtr++, size);
        if(type == CP_TYPE_NEWLINE) {
            size = LCD_COLUMNS;
            lcdPrintChar('\n');
        }
    } while(type != CP_TYPE_END && type != CP_TYPE_SPACES_END);

    return size;
}

void cprintf(const PrintData printDataPtr[]) {
    cprintf_size(printDataPtr, LCD_COLUMNS);
}


uint8_t cprintf(const PrintData * printDataPtr, uint8_t dig)
{
    PrintData p = pgm::read(printDataPtr);
    if(p.type == CP_TYPE_METHOD + CP_TYPE_VOID) {
        //Info: this must be before: uvalue = *p.data.uint16Ptr
        p.data.voidMethodPtr();
        return 0;
    } else if (p.type == CP_TYPE_STRING) {
        return lcdPrint_P(p.data.charPtr)-1;
    } else if (p.type == CP_TYPE_CHAIN) {
        return cprintf_size(p.data.printDataPtr, dig);
    } else if (p.type == CP_TYPE_NEWLINE) {
        p.type = CP_TYPE_SPACES_END;
    }

    uint16_t uvalue;
    bool isMethod = false;

    if(p.type & CP_TYPE_METHOD) {
        p.type ^= CP_TYPE_METHOD;
        uvalue = p.data.uint16MethodPtr();
        isMethod = true;
    } else {
        uvalue = *p.data.uint16Ptr;
    }


    if(p.size) {
        dig = p.size;
    }

    uint32_t v;
    uint8_t i;
    ArrayData array;
    const char * strPtr;
    switch(p.type) {
    case CP_TYPE_TEMPERATURE:
        lcdPrintUnsigned(uvalue/100, dig-1);
        lcdPrintChar('C');
        break;

    case CP_TYPE_TEMP_MINUT:
        lcdPrintUnsigned(uvalue/100, dig-3-2);
        lcdPrintChar('.');
        lcdPrintDigit((uvalue%100)/10);
        lcdPrint_P(settingsMenu::string_unitDeltaT);
        break;
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

    case CP_TYPE_UNSIGNED:      lcdPrintUnsigned(uvalue, dig); break;
    case CP_TYPE_SIGNED_mV:     lcdPrint_mV((int16_t)uvalue, dig); break;
    case CP_TYPE_V:             lcdPrintVoltage(uvalue, dig); break;
    case CP_TYPE_A:             lcdPrintCurrent(uvalue, dig); break;
    case CP_TYPE_P:             lcdPrintCurrent(uvalue, dig); break; //TODO:??
    case CP_TYPE_E:             lcdPrintCurrent(uvalue, dig); break;//TODO:??
    case CP_TYPE_R:             lcdPrintResistance(uvalue, dig); break;
    case CP_TYPE_PROCENTAGE:    lcdPrintPercentage(uvalue, dig); break;
    case CP_TYPE_SPACES_END:    lcdPrintSpaces(dig); break;
    case CP_TYPE_END:           break;
    case CP_TYPE_MINUTES:
        lcdPrintUnsigned(uvalue, dig-1);
        lcdPrintChar('m');
        break;
    case CP_TYPE_V2:
        lcdPrintUnsigned(uvalue/1000, dig-1);
        lcdPrintChar('V');
        break;
    case CP_TYPE_ON_OFF:        lcdPrintYesNo(uvalue, dig); break;
    case CP_TYPE_CHARGE:
        if(uvalue == PROGRAM_DATA_MAX_CHARGE) {
            lcdPrint_P(string_unlimited);
        } else {
            lcdPrintCharge(uvalue, dig);
        }
        break;
#ifdef ENABLE_TIME_LIMIT
    case CP_TYPE_CHARGE_TIME:
        if(uvalue == PROGRAM_DATA_MAX_TIME) {
            lcdPrint_P(string_unlimited);
        } else {
            lcdPrintUnsigned(uvalue, dig - string_size_minutes + 1);
            lcdPrint_P(string_minutes);
        }
        break;
#endif
    case CP_TYPE_CHAR:
        if(isMethod) p.data.uint16 = uvalue;
        for(i=0;i<dig;i++) {
            lcdPrintChar(p.data.uint16);
        }
        break;
    case CP_TYPE_TIME:
        lcdPrintTime(uvalue);  //TODO: dig??
        break;

    }
    return dig;
}




} //cprintf
