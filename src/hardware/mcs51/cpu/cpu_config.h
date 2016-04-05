/*
    cheali-charger - open source firmware for a variety of LiPo chargers
    Copyright (C) 2016 Paweł Stawicki. All right reserved.

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
#ifndef CPU_CONFIG_H_
#define CPU_CONFIG_H_

#define CHEALI_CHARGER_ARCHITECTURE_CPU         0x1000
#define CHEALI_CHARGER_ARCHITECTURE_CPU_STRING  "atmega32"

//#ifndef CHEALI_EEPROM_PACKED
//TODO:??
#define CHEALI_EEPROM_PACKED
//#endif

#define REDUNDANT_CHECK(x)
//#include <stdbool.h>

#ifdef SDCC_COMPILER
typedef uint8_t bool2;
#else
typedef bool bool2;
#endif


#ifdef SDCC_COMPILER
#include <stdint.h>

#define bool __bit
#define bool2 uint8_t
#define true 1
#define false 0

#endif

#endif /* CPU_CONFIG_H_ */
