/*
    cheali-charger - open source firmware for a variety of LiPo chargers
    Copyright (C) 2013  Pawel Stawicki. All right reserved.

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
#include "memory.h"
#include "Screen.h"
#include "LcdPrint.h"
#include "ProgramData.h"
#include "TheveninMethod.h"
#include "Settings.h"
#include "Hardware.h"
#include "Program.h"
#include "DelayStrategy.h"
#include "Version.h"
#include "ProgramDCcycle.h"
#include "Monitor.h"
#include "PolarityCheck.h"
#include "ScreenBalancer.h"
#include "Balancer.h"

namespace Screen { namespace Balancer {

#ifdef  ENABLE_SCREEN_KNIGHTRIDEREFFECT
    int8_t knightRiderCounter = 0;
    int8_t knightRiderDir = 1;
#endif
   
    AnalogInputs::ValueType getBalanceValue(uint8_t cell, AnalogInputs::Type type)
    {
        if(type == AnalogInputs::Voltage)
            return ::Balancer::getPresumedV(cell);
        return TheveninMethod::getReadableRthCell(cell);
    }

    void printBalancer(uint8_t cell, AnalogInputs::Type type) {
        if(AnalogInputs::isConnected(AnalogInputs::Name(AnalogInputs::Vb1+cell))) {
            lcdPrintAnalog(getBalanceValue(cell, type), type, 6);
        } else {
            lcdPrint_P(PSTR("  --  "));
        }
    }

    void displayBalanceInfo(uint8_t from, AnalogInputs::Type type)
    {
        lcdSetCursor0_0();

#ifdef ENABLE_SCREEN_KNIGHTRIDEREFFECT
        knightRiderCounter += knightRiderDir;
        if (knightRiderCounter==0 || knightRiderCounter>4) knightRiderDir=-knightRiderDir;
#endif
        char c = ' ';
        if(!::Balancer::isWorking()) {
            if(!::Balancer::isStable())
                c = 'm';
        } else {
            if(::Balancer::savedVon)
                c = 'B';
            else
                c = 'b';
        }

        if (::Balancer::balance == 0) {
            lcdPrintChar(c);
        }

        if (::Balancer::balance != 0) {
            uint8_t  j = 1;
            for(uint8_t i = 0; i < ::Balancer::getCells(); i++) {
                if(i == ::Balancer::minCell) {
                    c = SCREEN_EMPTY_CELL_CHAR; //lowest cell
                } else {
                    if(::Balancer::balance&j) {
                        if (blink.blinkTime_ & 1) {
                            c = SCREEN_FULL_CELL_CHAR; //flash full/empty cells
                        } else {
                            c = SCREEN_EMPTY_CELL_CHAR; //flash full/empty cells
                        }
                    } else {
                     c = SCREEN_AVR_CELL_CHAR; //average cells
                    }
                }
                lcdPrintChar(c);
                if(j!=128)  j <<= 1;
                else        j = 0;
            }
            lcdPrintSpaces(8 - ::Balancer::getCells());

        } else {
#ifdef ENABLE_SCREEN_KNIGHTRIDEREFFECT
            char knightRiderArrow;
            if (knightRiderDir > 0) knightRiderArrow='>'; else knightRiderArrow='<';
            if ((c == 'm') || (c == ' ')) {
                for (uint8_t i=0; i<6; i++ ) {
                    if (knightRiderCounter==i) lcdPrintChar(knightRiderArrow);
                    else lcdPrintChar(' ');
                }
                lcdPrintChar(' ');
            } else {
                lcdPrintSpaces(7);
            }
#else
        lcdPrintSpaces(7);
#endif
        }
        lcdPrintDigit(from+1);
        lcdPrintChar(':');

        printBalancer(from++, type);
        lcdPrintSpaces();

        lcdSetCursor0_1();
        if(from < MAX_BANANCE_CELLS) {
            lcdPrintDigit(from+1);
            lcdPrintChar(':');
            printBalancer(from++, type);
        }
        if(from < MAX_BANANCE_CELLS) {
            lcdPrintDigit(from+1);
            lcdPrintChar(':');
            printBalancer(from, type);
        }
        lcdPrintSpaces();
    }

}// namespcae Balancer
}// namespace Screen


void Screen::Balancer::displayVoltage1_3() {
    displayBalanceInfo(0, AnalogInputs::Voltage);
}
void Screen::Balancer::displayVoltage4_6() {
    displayBalanceInfo(3, AnalogInputs::Voltage);
}
void Screen::Balancer::displayVoltage7_9() {
    displayBalanceInfo(6, AnalogInputs::Voltage);
}

void Screen::Balancer::displayResistance1_3() {
    displayBalanceInfo(0, AnalogInputs::Resistance);
}
void Screen::Balancer::displayResistance4_6() {
    displayBalanceInfo(3, AnalogInputs::Resistance);
}
void Screen::Balancer::displayResistance7_9() {
    displayBalanceInfo(6, AnalogInputs::Resistance);
}

