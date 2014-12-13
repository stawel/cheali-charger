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
#define EEMEM __attribute__((section(".data_flash")))
#define CHEALI_EEPROM_PACKED __attribute__((packed))

namespace pgm {

    inline char *strncpy(char * buf, const char *str, size_t s) {
        return std::strncpy(buf, str, s);
    }

    inline size_t strlen(const char *s) {
        return std::strlen(s);
    }

    template<class Type>
    static void read(Type &t, const Type * addressP) {
        std::memcpy(&t, addressP, sizeof(Type));
    }

    template<class Type>
    static Type read(const Type * addressP) {
        Type t;
        read(t, addressP);
        return t;
    }

};


namespace eeprom {

    void write_impl(uint8_t * addressE, const uint8_t * data, int size);

    template<class Type>
    static Type read(const Type * addressE) {
        return  *addressE;
    }
    template<class Type>
    static void read(Type &t, const Type * addressE) {
        t = read(addressE);
    }

    template<class Type>
    static void write(Type * addressE, const Type &t) {
        write_impl((uint8_t*)addressE, (uint8_t*) &t, sizeof(Type));
    }
};

#endif /* MEMORY_H_ */
