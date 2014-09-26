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

#define PSTR(x) x
#define PROGMEM
#define EEMEM

namespace pgm {

    inline char *strncpy(char * buf, const char *str, size_t s) {
        return strncpy(buf, str, s);
    }

    inline size_t strlen(const char *s) {
        return strlen(s);
    }

    template<class Type, int n>
    struct read_impl{
        inline Type operator()(const Type * addressP) {
            Type t;
            memcpy(&t, addressP, sizeof(Type));
            return t;
        }
        inline void operator()(Type &t, const Type * addressP) {
            memcpy(&t, addressP, sizeof(Type));
        }
    };

    template<class Type>
    static Type read(const Type * addressP) {
        return read_impl<Type, sizeof(Type)>() (addressP);
    }

    template<class Type>
    static void read(Type &t, const Type * addressP) {
        read_impl<Type, sizeof(Type)>() (t, addressP);
    }
};


namespace eeprom {

    template<class Type, int n>
    struct read_impl{
        inline Type operator()(const Type * addressE) {
            //TODO:implement
            Type t = *addressE;
            //eeprom_read_block(&t, addressE, sizeof(Type));
            return t;
        }
        inline void operator()(Type &t, const Type * addressE) {
            //TODO:implement
            t = *addressE;
            //eeprom_read_block(&t, addressE, sizeof(Type));
        }
    };

    template<class Type, int n>
    struct write_impl{
        inline void operator()(Type * addressE, const Type &t) {
            //TODO:implement
            *addressE = t;
            //eeprom_update_block(&t, addressE, sizeof(Type));
        }
    };

    template<class Type>
    static Type read(const Type * addressE) {
        return read_impl<Type, sizeof(Type)>() (addressE);
    }
    template<class Type>
    static void read(Type &t, const Type * addressE) {
        return read_impl<Type, sizeof(Type)>() (t, addressE);
    }

    template<class Type>
    static void write(Type * addressE, const Type &t) {
        return write_impl<Type, sizeof(Type)>() (addressE, t);
    }
};

#endif /* MEMORY_H_ */
