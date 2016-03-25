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
#ifndef MEMORY_H_
#define MEMORY_H_

//#define CHEALI_EEPROM_PACKED

#ifndef SDCC_COMPILER
#define __reentrant
#define __xdata
#define __data
#define __idata
#define __pdata
#define __code
#define __bit bool
#define PSTR(x) x
#define PROGMEM
#define EEMEM
#define DATAMEM
#else
#define PSTR(x) x
#define PROGMEM     __code
#define EEMEM       __code
#define DATAMEM     __xdata
#endif

#include <stdint.h>
#include <string.h>

#define eeprom_write(adr, x) eeprom_write_(adr, &(x), sizeof(x))
#define eeprom_read(x, adr) eeprom_read_(&(x), adr, sizeof(x))

#define pgm_read(x, adr) pgm_read_(&(x), adr, sizeof(x))

inline void eeprom_write_(void * adr, const void * const x, uint8_t size){}
inline void eeprom_read_(void *x, const void * const adr, uint8_t size){}

inline void pgm_read_(void *x, const void * const adr, uint8_t size){}
inline uint8_t pgm_strlen(const char * str) {return strlen(str);}


typedef const PROGMEM char * const_char_ptr;
typedef const PROGMEM void * const_void_ptr;
typedef DATAMEM uint16_t * uint16_ptr;
typedef DATAMEM int16_t  * int16_ptr;
typedef DATAMEM uint32_t * uint32_ptr;
/*uint16_t uint16;
int16_t  * int16Ptr;
uint32_t * uint32Ptr;
*/

#endif /* MEMORY_H_ */
