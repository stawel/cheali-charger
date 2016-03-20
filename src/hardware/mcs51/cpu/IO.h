/*
    cheali-charger - open source firmware for a variety of LiPo chargers
    Copyright (C) 2013  Tucker Kern. All right reserved.

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
//#include <avr/io.h>

// Hopefully these aren't invalid for other platforms
#define OUTPUT 1
#define INPUT 0
#define HIGH 1
#define LOW 0
#define EXTERNAL 0

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
#define INLINE_ATTR

namespace IO {

        // Core IO functions to be implemented in proper target folder
        inline void digitalWrite(uint8_t pinNumber, uint8_t value) INLINE_ATTR;
        inline uint8_t digitalRead(uint8_t pinNumber) INLINE_ATTR;
        inline void pinMode(uint8_t pinNumber, uint8_t mode) INLINE_ATTR;
        inline void analogReference(uint8_t mode) INLINE_ATTR;

        // Auxillery functions utilized for pin to port mapping
        inline volatile uint8_t* pinToPort(uint8_t pinNumber) INLINE_ATTR;
        inline uint8_t pinBitmask(uint8_t pinNumber) INLINE_ATTR;
        inline volatile uint8_t* pinToInputPort(uint8_t pinNumber) INLINE_ATTR;
        inline volatile uint8_t* pinToDDR(uint8_t pinNumber) INLINE_ATTR;

        inline void digitalWrite(uint8_t pinNumber, uint8_t value)
        {
        }

        inline uint8_t digitalRead(uint8_t pinNumber)
        {
        }

        inline void pinMode(uint8_t pinNumber, uint8_t mode)
        {
        }

        inline void analogReference(uint8_t mode)
        {
        }

        // Convert physical pin number to logical port address
        inline volatile uint8_t* pinToPort(uint8_t pinNumber)
        {
        }

        // Pin number to bitmask mapping
        inline uint8_t pinBitmask(uint8_t pinNumber)
        {
        }

        // Pin number to input register map
        inline volatile uint8_t* pinToInputPort(uint8_t pinNumber)
        {

        }

        // Pin number to data direction register
        inline volatile uint8_t* pinToDDR(uint8_t pinNumber)
        {
        }


}
#endif /* IO_H_ */
