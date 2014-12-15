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
#include "M051Series.h"

//#define OUTPUT GPIO_PMD_OUTPUT
#define OUTPUT GPIO_PMD_QUASI
//#define OUTPUT_QUASI GPIO_PMD_QUASI
#define INPUT GPIO_PMD_QUASI
#define ANALOG_INPUT 200
#define ANALOG_INPUT_DISCHARGE 201
#define HIGH 1
#define LOW 0

#define PORT_OFFSET   0x40

//#define INLINE_ATTR __attribute__((always_inline))
//#define INLINE_ATTR

namespace IO
{

        inline volatile uint32_t * getPinAddress(uint8_t pinNumber) {
            switch(pinNumber) {
            case 40: return &P00;
            case 39: return &P01;
            case 38: return &P02;
            case 37: return &P03;
            case 35: return &P04;
            case 34: return &P05;
            case 33: return &P06;
            case 32: return &P07;

            case 43: return &P10;
            case 44: return &P11;
            case 45: return &P12;
            case 46: return &P13;
            case 47: return &P14;
            case 1: return &P15;
            case 2: return &P16;
            case 3: return &P17;

            case 19: return &P20;
            case 20: return &P21;
            case 21: return &P22;
            case 22: return &P23;
            case 23: return &P24;
            case 25: return &P25;
            case 26: return &P26;
            case 27: return &P27;

            case 5: return &P30;
            case 7: return &P31;
            case 8: return &P32;
            case 9: return &P33;
            case 10: return &P34;
            case 11: return &P35;
            case 13: return &P36;
            case 14: return &P37;

            case 24: return &P40;
            case 36: return &P41;
            case 48: return &P42;
            case 12: return &P43;
            case 28: return &P44;
            case 29: return &P45;
            case 30: return &P46;
            case 31: return &P47;

            //virtual pins
            case 3+128: return &P17;
            case 3+64:  return &P17;
            }
            return &P46; //not used
        }

//#define GPIO_PIN_ADDR(port, pin)    (*((volatile uint32_t *)((GPIO_PIN_DATA_BASE+(0x20*(port))) + ((pin)<<2))))

        inline uint32_t getPinBit(uint8_t pinNumber) {
            uint32_t adr = (uint32_t) getPinAddress(pinNumber);
            return (adr >> 2) & 7;
        }

        inline uint32_t getPortNr(uint8_t pinNumber) {
            uint32_t adr = (uint32_t) getPinAddress(pinNumber);
            return (adr / 0x20) & 7;
        }

        inline GPIO_T * getPort(uint8_t pinNumber) {
            return (GPIO_T *) (((uint32_t)P0) + (getPortNr(pinNumber) * PORT_OFFSET));
        }

        inline uint8_t getADCChannel(uint8_t pinNumber) {
            return getPinBit(pinNumber);
        }




        // Core IO functions to be implemented in proper target folder
        inline void digitalWrite(uint8_t pinNumber, uint8_t value) {
            if(value)   value = 1;
            *(getPinAddress(pinNumber)) = value;
        };

        inline uint8_t digitalRead(uint8_t pinNumber) {
            return *(getPinAddress(pinNumber));
        }

        //based on coocox DrvGPIO
        inline void enableFuncADC(uint8_t adc) {
            outpw(&SYS->P1_MFP, (inpw(&SYS->P1_MFP) & ~(0x1<<((adc) +8))) | (0x1<<(adc)));
        }
        inline void disableFuncADC(uint8_t adc) {
            outpw(&SYS->P1_MFP, inpw(&SYS->P1_MFP) & ~( (0x1<<(adc+8)) | (0x1<<adc)) );
        }

        inline void pinMode(uint8_t pinNumber, uint8_t mode) {
            GPIO_T * port = getPort(pinNumber);
            uint8_t pin = getPinBit(pinNumber);

            if(mode == ANALOG_INPUT) {
                GPIO_SetMode(port, 1 << pin, GPIO_PMD_INPUT);
                GPIO_DISABLE_DIGITAL_PATH(port, 1 << pin);
                enableFuncADC(pin);
            } else if(mode == ANALOG_INPUT_DISCHARGE) {
                GPIO_SetMode(port, 1 << pin, GPIO_PMD_OPEN_DRAIN);
                GPIO_DISABLE_DIGITAL_PATH(port, 1 << pin);
                enableFuncADC(pin);
            } else {
                GPIO_SetMode(port, 1 << pin, mode);
                GPIO_ENABLE_DIGITAL_PATH(port, 1 << pin);
            }
        };
}
#endif /* IO_H_ */

