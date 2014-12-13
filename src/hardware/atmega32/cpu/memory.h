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

#define CHEALI_EEPROM_PACKED __attribute__((packed))

namespace pgm {

    inline char *strncpy(char * buf, const char *str, size_t s) {
        return strncpy_P(buf, str, s);
    }

    inline size_t strlen(const char *s) {
        return strlen_P(s);
    }

    template<class Type, int n>
    struct read_impl{
        inline Type operator()(const Type * addressP) {
            Type t;
            memcpy_P(&t, addressP, sizeof(Type));
            return t;
        }
        inline void operator()(Type &t, const Type * addressP) {
            memcpy_P(&t, addressP, sizeof(Type));
        }
    };

    template<class Type>
    struct read_impl<Type, 4> {
        union Type_union{
            Type t;
            uint32_t u;
        };
        inline Type operator()(const Type * addressP) {
            Type_union tu;
            tu.u = pgm_read_dword(addressP);
            return tu.t;
        }
        inline void operator()(Type &t, const Type * addressP) {
            Type_union tu;
            tu.u = pgm_read_dword(addressP);
            t = tu.t;
        }
    };

    template<class Type>
    struct read_impl<Type, 2> {
        inline Type operator()(const Type * addressP) {
            return (Type)pgm_read_word(addressP);
        }
        inline void operator()(Type &t, const Type * addressP) {
            t = (Type)pgm_read_word(addressP);
        }
    };

    template<class Type>
    struct read_impl<Type, 1> {
        inline Type operator()(const Type * addressP) {
            return (Type)pgm_read_byte(addressP);
        }
        inline void operator()(Type &t, const Type * addressP) {
            t = (Type)pgm_read_byte(addressP);
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
            Type t;
            eeprom_read_block(&t, addressE, sizeof(Type));
            return t;
        }
        inline void operator()(Type &t, const Type * addressE) {
            eeprom_read_block(&t, addressE, sizeof(Type));
        }
    };

//    Increases program size

    /*     template<class Type>
    struct read_n<Type, 4> {
        union Type_union{
            Type t;
            uint32_t u;
        };
        inline Type operator()(const Type * addressE) {
            Type_union tu;
            tu.u = eeprom_read_dword((const uint32_t*)addressE);
            return tu.t;
        }
        inline void operator()(Type &t, const Type * addressE) {
            Type_union tu;
            tu.u = eeprom_read_dword((const uint32_t*)addressE);
            t = tu.t;
        }
    };*/

    template<class Type>
    struct read_impl<Type, 2> {
        inline Type operator()(const Type * addressE) {
            return (Type)eeprom_read_word((const uint16_t*)addressE);
        }
        inline void operator()(Type &t, const Type * addressE) {
            t = (Type)eeprom_read_word((const uint16_t*)addressE);
        }
    };

    template<class Type>
    struct read_impl<Type, 1> {
        inline Type operator()(const Type * addressE) {
            return (Type)eeprom_read_byte(addressE);
        }
        inline void operator()(Type &t, const Type * addressE) {
            t = (Type)eeprom_read_byte(addressE);
        }
    };



    template<class Type, int n>
    struct write_impl{
        inline void operator()(Type * addressE, const Type &t) {
            eeprom_update_block(&t, addressE, sizeof(Type));
        }
    };

    template<class Type>
    struct write_impl<Type, 2> {
        inline void operator()(Type * addressE, const Type &t) {
            eeprom_update_word((uint16_t*) addressE,(uint16_t) t);
        }
    };

    template<class Type>
    struct write_impl<Type, 1> {
        inline void operator()(Type * addressE, const Type &t) {
            eeprom_update_byte((const uint8_t*) addressE, t);
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
