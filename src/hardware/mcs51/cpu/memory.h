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

#define PSTR(x) x
#define PROGMEM
#define EEMEM
//#define CHEALI_EEPROM_PACKED


//#include <avr/pgmspace.h>
//#include <avr/eeprom.h>
//#include <cstddef>
#include <stdint.h>

#define write(adr, x) write_(adr, &(x), sizeof(x))
#define read(x, adr) read_(&(x), adr, sizeof(x))

namespace eeprom {

    void write_(void * adr, const void * const x, uint8_t size);
    void read_(void *x, const void * const adr, uint8_t size);

}

namespace pgm {
    void read_(void *x, const void * const adr, uint8_t size);
    uint8_t strlen(const char * str);
}


#endif /* MEMORY_H_ */
