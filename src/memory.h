#ifndef MEMORY_H_
#define MEMORY_H_

#include <avr/pgmspace.h>
#include <avr/eeprom.h>


struct pgm {
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


struct eeprom {

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
