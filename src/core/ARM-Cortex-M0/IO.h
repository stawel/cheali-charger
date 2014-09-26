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
#ifndef IO_H_
#define IO_H_

#include <stdint.h>

#define OUTPUT 1
#define INPUT 0
#define HIGH 1
#define LOW 0

// A0-7 definitions
#define A0 0
#define A1 1
#define A2 2
#define A3 3
#define A4 4
#define A5 5
#define A6 6
#define A7 7


//#define INLINE_ATTR __attribute__((always_inline))
//#define INLINE_ATTR

namespace IO
{
        //TODO: implement
        // Core IO functions to be implemented in proper target folder
        inline void digitalWrite(uint8_t pinNumber, uint8_t value) {};
        inline uint8_t digitalRead(uint8_t pinNumber) {};
        inline void pinMode(uint8_t pinNumber, uint8_t mode) {};

}
#endif /* IO_H_ */
