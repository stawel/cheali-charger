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

#define PROGMEM_PTR     PROGMEM
#define EEMEM_PTR       EEMEM

typedef const PROGMEM char *        const_char_ptr;
typedef const PROGMEM void *        const_void_ptr;
typedef const PROGMEM uint16_t *    const_uint16_ptr;
typedef const PROGMEM uint32_t *    const_uint32_ptr;
typedef DATAMEM uint16_t * uint16_ptr;
typedef DATAMEM int16_t  * int16_ptr;
typedef DATAMEM uint32_t * uint32_ptr;

#define eeprom_write(adr, x) eeprom_write_(adr, &(x), sizeof(x))
#define eeprom_read(x, adr) eeprom_read_(&(x), adr, sizeof(x))

#define pgm_read(x, adr) pgm_read_(&(x), adr, sizeof(x))

inline void eeprom_write_(void EEMEM * adr, const void * const x, uint8_t size)   { memcpy(adr,x,size);}
inline void eeprom_read_(void *x, const EEMEM void * const adr, uint8_t size)     { memcpy(x,adr,size);}

inline void pgm_read_(void *x, const PROGMEM void * const adr, uint8_t size)
{
    if(size == 1) {
        const_char_ptr ptr = (const_char_ptr)adr;
        char * xp = (char *)x;
        *xp = *ptr;
    } else if(size == 2) {
        const_uint16_ptr ptr = (const_uint16_ptr)adr;
        uint16_t * xp = (uint16_t *) x;
        *xp = *ptr;
    } else {
        memcpy(x,adr,size);
    }
}
inline uint8_t pgm_strlen(const PROGMEM char * str)                                 { return strlen(str);}


#endif /* MEMORY_H_ */
