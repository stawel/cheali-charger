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

#include "IO.h"
#include <avr/io.h>

namespace IO
{
	void digitalWrite(uint8_t pinNumber, uint8_t value)
	{
		// Grab the port and proper bitmask for the pin
		volatile uint8_t *port = pinToPort(pinNumber);
		uint8_t bitmask = pinBitmask(pinNumber);

		// Set the output
		if (value)
		{
			*port |= bitmask;
		}
		else
		{
			*port &= ~bitmask;
		}
		return;
	}

	uint8_t digitalRead(uint8_t pinNumber)
	{
		// Grab the port and proper bitmask for the pin
		volatile uint8_t *port = pinToInputPort(pinNumber);
		uint8_t bitmask = pinBitmask(pinNumber);
		
		return (*port & bitmask);
	}

	void pinMode(uint8_t pinNumber, uint8_t mode)
	{
		volatile uint8_t *port = pinToDDR(pinNumber);
		uint8_t bitmask = pinBitmask(pinNumber);

		if (mode)
		{
			// Mode is not 0, Set mode to output
			*port |= bitmask;
		}
		else
		{
			// Set pin to input
			*port &= ~bitmask;
		}
	}

	// Convert physical pin number to logical port address
	volatile uint8_t* pinToPort(uint8_t pinNumber)
	{
		// Better way to do this?
		switch (pinNumber)
		{
			// Port A
			case 30:
			case 31:
			case 32:
			case 33:
			case 34:
			case 35:
			case 36:
			case 37:
			return &PORTA;
			
			// Port B
			case 1:
			case 2:
			case 3:
			case 40:
			case 41:
			case 42:
			case 43:
			case 44:
			return &PORTB;
			
			// Port C
			case 19:
			case 20:
			case 21:
			case 22:
			case 23:
			case 24:
			case 25:
			case 26:
			return &PORTC;
			
			// Port D
			case 9:
			case 10:
			case 11:
			case 12:
			case 13:
			case 14:
			case 15:
			case 16:
			return &PORTD;
			
			default:
			return 0;
		}		
	}
	
	// Pin number to bitmask mapping
	uint8_t pinBitmask(uint8_t pinNumber)
	{
		switch (pinNumber)
		{
			// Px0
			case 37:
			case 40:
			case 19:
			case 9:
			return 1<<0;
			
			// Px1
			case 36:
			case 41:
			case 20:
			case 10:
			return 1<<1;
			
			// Px2
			case 35:
			case 42:
			case 21:
			case 11:
			return 1<<2;
			
			// Px3
			case 34:
			case 43:
			case 22:
			case 12:
			return 1<<3;
			
			// Px4
			case 33:
			case 44:
			case 23:
			case 13:
			return 1<<4;
			
			// Px5
			case 32:
			case 1:
			case 24:
			case 14:
			return 1<<5;
			
			// Px6
			case 31:
			case 2:
			case 25:
			case 15:
			return 1<<6;
			
			// Px7
			case 30:
			case 3:
			case 26:
			case 16:
			return 1<<7;
			
			default:
			return 0;
		}

	}

	// Pin number to input register map
	volatile uint8_t* pinToInputPort(uint8_t pinNumber)
	{
		switch (pinNumber)
		{
			// Port A
			case 30:
			case 31:
			case 32:
			case 33:
			case 34:
			case 35:
			case 36:
			case 37:
			return &PINA;

			// Port B
			case 1:
			case 2:
			case 3:
			case 40:
			case 41:
			case 42:
			case 43:
			case 44:
			return &PINB;

			// Port C
			case 19:
			case 20:
			case 21:
			case 22:
			case 23:
			case 24:
			case 25:
			case 26:
			return &PINC;

			// Port D
			case 9:
			case 10:
			case 11:
			case 12:
			case 13:
			case 14:
			case 15:
			case 16:
			return &PIND;

			default:
			return 0;
		}		

	}

	// Pin number to data direction register
	volatile uint8_t* pinToDDR(uint8_t pinNumber)
	{
		switch (pinNumber)
		{
			// Port A
			case 30:
			case 31:
			case 32:
			case 33:
			case 34:
			case 35:
			case 36:
			case 37:
			return &DDRA;
			
			// Port B
			case 1:
			case 2:
			case 3:
			case 40:
			case 41:
			case 42:
			case 43:
			case 44:
			return &DDRB;
			
			// Port C
			case 19:
			case 20:
			case 21:
			case 22:
			case 23:
			case 24:
			case 25:
			case 26:
			return &DDRC;
			
			// Port D
			case 9:
			case 10:
			case 11:
			case 12:
			case 13:
			case 14:
			case 15:
			case 16:
			return &DDRD;
			
			default:
			return 0;
		}		
	}
}
