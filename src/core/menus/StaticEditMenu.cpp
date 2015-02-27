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
#include "StaticEditMenu.h"
#include "Keyboard.h"
#include "memory.h"
#include "LcdPrint.h"
#include STRINGS_HEADER

void StaticEditMenu::printItem(uint8_t item)
{
    uint8_t dig = lcdPrint_P(staticMenu_, item);
    dig = LCD_COLUMNS - dig;

    const PrintData p = pgm::read(&(staticEditData[item].print));
    const uint16_t value = *p.valuePtr;
    switch(p.type) {
    case CP_TYPE_TEMPERATURE:
        lcdPrintUnsigned(value/100, dig-3-2);
        lcdPrintChar('.');
        lcdPrintDigit((value%100)/10);
        lcdPrint_P(settingsMenu::string_unitDeltaT);

    	break;
//    case CP_TYPE_STRING_ARRAY:	lcdPrint_P(p.stringArrayPtr, item); break;
    case CP_TYPE_UNSIGNED:		lcdPrintUnsigned(value, dig); break;
    case CP_TYPE_SIGNED_mV:		lcdPrint_mV(value, dig); break;
    case CP_TYPE_V:				lcdPrintVoltage(value, dig); break;
    case CP_TYPE_A:				lcdPrintCurrent(value, dig); break;
    case CP_TYPE_PROCENTAGE:	lcdPrintPercentage(value, dig); break;
    case CP_TYPE_MINUTES:
		lcdPrintUnsigned(value, dig-1);
		lcdPrintChar('m');
		break;
    case CP_TYPE_V2:
	    lcdPrintUnsigned(value/1000, dig-1);
	    lcdPrintChar('V');
	    break;
    }
}

void StaticEditMenu::editItem(uint8_t item, uint8_t key) {
    uint16_t * const valuePtr = pgm::read(&staticEditData[item].print.valuePtr);
    EditData d = pgm::read(&staticEditData[item].edit);
    *valuePtr += d.step;
    if(*valuePtr < d.minValue) *valuePtr = d.minValue;
    if(*valuePtr > d.maxValue) *valuePtr = d.maxValue;

}
