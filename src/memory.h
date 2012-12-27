#ifndef MEMORY_H_
#define MEMORY_H_

#include <avr/pgmspace.h>
#include <avr/eeprom.h>


struct pgm {
	template<class Type, int n>
	struct read_n{
		inline const Type operator()(const Type * addressP) {
			Type t;
			memcpy_P(&t, addressP, sizeof(Type));
			return t;
		}
		inline void operator()(Type &t, const Type * addressP) {
			memcpy_P(&t, addressP, sizeof(Type));
		}
	};

	template<class Type>
	struct read_n<Type, 4> {
		union Type_union{
			Type t;
			uint32_t u;
		};
		inline const Type operator()(const Type * addressP) {
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
	struct read_n<Type, 2> {
		inline const Type operator()(const Type * addressP) {
			return (Type)pgm_read_word(addressP);
		}
		inline void operator()(Type &t, const Type * addressP) {
			t = pgm_read_word(addressP);
		}
	};

	template<class Type>
	struct read_n<Type, 1> {
		inline const Type operator()(const Type * addressP) {
			return (Type)pgm_read_byte(addressP);
		}
		inline void operator()(Type &t, const Type * addressP) {
			t = pgm_read_byte(addressP);
		}
	};

	template<class Type>
	static const Type read(const Type * addressP) {
		return read_n<Type, sizeof(Type)>() (addressP);
	}

	template<class Type>
	static void read(Type &t, const Type * addressP) {
		read_n<Type, sizeof(Type)>() (t, addressP);
	}
};


template<class Type>
inline Type eeprom_read(const Type * addressE) {
	Type t;
	eeprom_read_block(&t, addressE, sizeof(Type));
	return t;
}
template<class Type>
inline void eeprom_read(Type &t, const Type * addressE) {
	eeprom_read_block(&t, addressE, sizeof(Type));
}

template<class Type>
inline void eeprom_write(Type * addressE, const Type &t) {
	eeprom_update_block(&t, addressE, sizeof(Type));
}



#endif /* MEMORY_H_ */
