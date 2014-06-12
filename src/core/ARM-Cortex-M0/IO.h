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
extern "C" {
#include "DrvGPIO.h"
}

#define OUTPUT E_IO_OUTPUT
#define INPUT E_IO_QUASI
#define ANALOG_INPUT 200
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
        //TODO: Rewrite!
		inline E_DRVGPIO_PORT getPort(uint8_t pinNumber) {
			switch(pinNumber) {
			case 32:
			case 33:
			case 34:
			case 35:
			case 37:
			case 38:
			case 39:
			case 40:
				return E_PORT0;

			case 5:
			case 7:
			case 8:
			case 9:
			case 10:
			case 11:
			case 13:
			case 14:
				return E_PORT3;

			case 31:
			case 30:
			case 29:
			case 28:
			case 12:
			case 48:
			case 36:
			case 24:
				return E_PORT4;

			case 19:
			case 20:
			case 21:
			case 22:
			case 23:
			case 25:
			case 26:
			case 27:
				return E_PORT2;

			case 43:
			case 44:
			case 45:
			case 46:
			case 47:
			case 1:
			case 2:
			case 3:
				return E_PORT1;

			}
			return E_PORT4;
		}
		//TODO: Rewrite!
		inline uint8_t getPin(uint8_t pinNumber) {
			switch(pinNumber) {
			case 32: return E_PIN7;
			case 33: return E_PIN6;
			case 34: return E_PIN5;
			case 35: return E_PIN4;
			case 37: return E_PIN3;
			case 38: return E_PIN2;
			case 39: return E_PIN1;
			case 40: return E_PIN0;

			case 5: return E_PIN0;
			case 7: return E_PIN1;
			case 8: return E_PIN2;
			case 9: return E_PIN3;
			case 10: return E_PIN4;
			case 11: return E_PIN5;
			case 13: return E_PIN6;
			case 14: return E_PIN7;

			case 31: return E_PIN7;
			case 30: return E_PIN6;
			case 29: return E_PIN5;
			case 28: return E_PIN4;
			case 12: return E_PIN3;
			case 48: return E_PIN2;
			case 36: return E_PIN1;
			case 24: return E_PIN0;

			case 19: return E_PIN0;
			case 20: return E_PIN1;
			case 21: return E_PIN2;
			case 22: return E_PIN3;
			case 23: return E_PIN4;
			case 25: return E_PIN5;
			case 26: return E_PIN6;
			case 27: return E_PIN7;

			case 43: return E_PIN0;
			case 44: return E_PIN1;
			case 45: return E_PIN2;
			case 46: return E_PIN3;
			case 47: return E_PIN4;
			case 1: return E_PIN5;
			case 2: return E_PIN6;
			case 3: return E_PIN7;
			}
			return E_PIN6;
		}
        inline uint8_t getADCChannel(uint8_t pinNumber) {
        	switch(pinNumber) {
        		case 43: return 0;
        		case 44: return 1;
        		case 45: return 2;
        		case 46: return 3;
        		case 47: return 4;
        		case 1: return 5;
        		case 2: return 6;
        		case 3: return 7;
        	}
        	return 7;
        }


		// Core IO functions to be implemented in proper target folder
        inline void digitalWrite(uint8_t pinNumber, uint8_t value) {
        	E_DRVGPIO_PORT port = getPort(pinNumber);
        	uint8_t pin = getPin(pinNumber);

        	if(value) 	DrvGPIO_SetBit(port, pin);
        	else 		DrvGPIO_ClrBit(port, pin);
        };

        inline uint8_t digitalRead(uint8_t pinNumber) {
        	E_DRVGPIO_PORT port = getPort(pinNumber);
        	uint8_t pin = getPin(pinNumber);
        	return DrvGPIO_GetBit(port, pin);
        }

        inline void pinMode(uint8_t pinNumber, uint8_t mode) {
        	E_DRVGPIO_PORT port = getPort(pinNumber);
        	uint8_t pin = getPin(pinNumber);
        	if(mode == ANALOG_INPUT) {
            	DrvGPIO_DisableDigitalInputBit(port, pin);
            	DrvGPIO_InitFunction((E_DRVGPIO_FUNC) (E_FUNC_ADC0 + getADCChannel(pinNumber)));
        	} else {
        		DrvGPIO_Open(port, pin, (E_DRVGPIO_IO)mode);
        		if(mode == INPUT)
        			DrvGPIO_SetBit(port, pin);
        	}
        };
}
#endif /* IO_H_ */

