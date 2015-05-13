/*
    cheali-charger - open source firmware for a variety of LiPo chargers
    Copyright (C) 2014 Paweł Stawicki. All right reserved.

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

#include "IO.h"

#define PORT_OFFSET   0x40

namespace IO {

inline uint32_t getPortNr(volatile uint32_t * pinAddress) {
    return (((uint32_t) pinAddress) / 0x20) & 7;
}

inline GPIO_T * getPort(volatile uint32_t * pinAddress) {
    return (GPIO_T *) (((uint32_t)P0) + (getPortNr(pinAddress) * PORT_OFFSET));
}


void pinMode_(volatile uint32_t * pinAddress, uint8_t mode) {
    GPIO_T * port = getPort(pinAddress);
    uint32_t pinBit = getPinBit_(pinAddress);
    if(mode == ANALOG_INPUT) {
        GPIO_SetMode(port, 1 << pinBit, GPIO_PMD_INPUT);
        GPIO_DISABLE_DIGITAL_PATH(port, 1 << pinBit);
        enableFuncADC(pinBit);
    } else if(mode == ANALOG_INPUT_DISCHARGE) {
        GPIO_SetMode(port, 1 << pinBit, GPIO_PMD_OPEN_DRAIN);
        GPIO_DISABLE_DIGITAL_PATH(port, 1 << pinBit);
        enableFuncADC(pinBit);
    } else {
        GPIO_SetMode(port, 1 << pinBit, mode);
        GPIO_ENABLE_DIGITAL_PATH(port, 1 << pinBit);
    }
}

}
