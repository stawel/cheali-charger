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

#include <avr/pgmspace.h>
#include <avr/eeprom.h>


#define PROGMEM_PTR
#define EEMEM_PTR

// *_ptr - needed for mcs51 compatibility
typedef const  char *        const_char_ptr;
typedef const  void *        const_void_ptr;
typedef const  uint16_t *    const_uint16_ptr;
typedef const  uint32_t *    const_uint32_ptr;
typedef uint16_t * uint16_ptr;
typedef int16_t  * int16_ptr;
typedef uint32_t * uint32_ptr;
typedef uint16_t * ee_uint16_ptr;


inline char *pgm_strncpy(char * buf, const char *str, size_t s) {
    return strncpy_P(buf, str, s);
}

inline size_t pgm_strlen(const char *s) {
    return strlen_P(s);
}

template<class Type, int n>
struct pgm_read_impl{
    inline void operator()(Type &t, const Type * addressP) {
        memcpy_P(&t, addressP, sizeof(Type));
    }
};

template<class Type>
struct pgm_read_impl<Type, 4> {
    inline void operator()(Type &t, const Type * addressP) {
        uint32_t u = pgm_read_dword(addressP);
        t = reinterpret_cast<Type&>(u);
    }
};

template<class Type>
struct pgm_read_impl<Type, 2> {
    inline void operator()(Type &t, const Type * addressP) {
        uint16_t v = pgm_read_word(addressP);
        t = reinterpret_cast<Type&>(v);
    }
};

template<class Type>
struct pgm_read_impl<Type, 1> {
    inline void operator()(Type &t, const Type * addressP) {
        t = (Type)pgm_read_byte(addressP);
    }
};


template<class Type>
static void pgm_read(Type &t, const Type * addressP) {
    pgm_read_impl<Type, sizeof(Type)>() (t, addressP);
}





template<class Type, int n>
struct eeprom_read_impl{
    inline void operator()(Type &t, const Type * addressE) {
        eeprom_read_block(&t, addressE, sizeof(Type));
    }
};

template<class Type>
struct eeprom_read_impl<Type, 2> {
    inline void operator()(Type &t, const Type * addressE) {
        t = (Type)eeprom_read_word((const uint16_t*)addressE);
    }
};

template<class Type>
struct eeprom_read_impl<Type, 1> {
    inline void operator()(Type &t, const Type * addressE) {
        t = (Type)eeprom_read_byte(addressE);
    }
};


template<class Type, int n>
struct eeprom_write_impl{
    inline void operator()(Type * addressE, const Type &t) {
        eeprom_update_block(&t, addressE, sizeof(Type));
    }
};

template<class Type>
struct eeprom_write_impl<Type, 2> {
    inline void operator()(Type * addressE, const Type &t) {
        eeprom_update_word((uint16_t*) addressE,(uint16_t) t);
    }
};

template<class Type>
struct eeprom_write_impl<Type, 1> {
    inline void operator()(Type * addressE, const Type &t) {
        eeprom_update_byte((const uint8_t*) addressE, t);
    }
};


template<class Type>
static void eeprom_read(Type &t, const Type * addressE) {
    return eeprom_read_impl<Type, sizeof(Type)>() (t, addressE);
}

template<class Type>
static void eeprom_write(Type * addressE, const Type &t) {
    return eeprom_write_impl<Type, sizeof(Type)>() (addressE, t);
}

#endif /* MEMORY_H_ */
