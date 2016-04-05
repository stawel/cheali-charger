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

#include <cstring>
#include <stdint.h>

#define PSTR(x) x
#define PROGMEM
#define DATAMEM
#define PROGMEM_PTR
#define EEMEM_PTR
#define EEMEM __attribute__((section(".data_flash")))

// *_ptr - needed for mcs51 compatibility
typedef const char *        const_char_ptr;
typedef const void *        const_void_ptr;
typedef const uint16_t *    const_uint16_ptr;
typedef const uint32_t *    const_uint32_ptr;
typedef uint16_t * uint16_ptr;
typedef int16_t  * int16_ptr;
typedef uint32_t * uint32_ptr;


inline char *pgm_strncpy(char * buf, const char *str, size_t s) {
    return std::strncpy(buf, str, s);
}

inline size_t pgm_strlen(const char *s) {
    return std::strlen(s);
}

template<class Type>
static void pgm_read(Type &t, const Type * addressP) {
    std::memcpy(&t, addressP, sizeof(Type));
}



void eeprom_write_impl(uint8_t * addressE, const uint8_t * data, int size);

template<class Type>
inline void eeprom_read(Type &t, const Type * addressE) {
    t = *addressE;
}

template<class Type>
inline void eeprom_write(Type * addressE, const Type &t) {
    eeprom_write_impl((uint8_t*)addressE, (uint8_t*) &t, sizeof(Type));
}

#endif /* MEMORY_H_ */
