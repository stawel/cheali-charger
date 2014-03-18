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
#ifndef IO_H_
#define IO_H_

#include <stdint.h>

namespace IO
{
	// Core IO functions to be implemented in proper target folder
	void digitalWrite(uint8_t pinNumber, uint8_t value);
	uint8_t digitalRead(uint8_t pinNumber);
	void pinMode(uint8_t pinNumber, uint8_t mode);

	// Auxillery functions utilized for pin to port mapping
	volatile uint8_t* pinToPort(uint8_t pinNumber);
	uint8_t pinBitmask(uint8_t pinNumber);
	volatile uint8_t* pinToInputPort(uint8_t pinNumber);
	volatile uint8_t* pinToDDR(uint8_t pinNumber);

}


#endif /* IO_H_ */
