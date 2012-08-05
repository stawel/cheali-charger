#ifndef MEMORY_H_
#define MEMORY_H_

#include <avr/pgmspace.h>
#include <avr/eeprom.h>

template<class Type>
inline Type pgm_read(const Type * addressP) {
	Type t;
	memcpy_P(&t, addressP, sizeof(Type));
	return t;
}

template<class Type>
inline void pgm_read(Type &t, const Type * addressP) {
	memcpy_P(&t, addressP, sizeof(Type));
}

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
