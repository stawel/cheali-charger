/*
    cheali-charger - open source firmware for a variety of LiPo chargers
    Copyright (C) 2013 Paweł Stawicki. All right reserved.

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
#ifndef ATOMIC_H_
#define ATOMIC_H_

#include <inttypes.h>

static __inline__ uint8_t __iSeiRetVal(void)
{
//TODO: implement
//    sei();
    return 1;
}

static __inline__ uint8_t __iCliRetVal(void)
{
    //TODO: implement
//    cli();
    return 1;
}

static __inline__ void __iRestore(const  uint8_t *__s)
{
    //TODO: implement
//    SREG = *__s;
//    __asm__ volatile ("" ::: "memory");
}


#define ATOMIC_BLOCK(type) for ( type, __ToDo = __iCliRetVal(); \
                           __ToDo ; __ToDo = 0 )

//TODO: implement
#define ATOMIC_RESTORESTATE uint8_t sreg_save \
    __attribute__((__cleanup__(__iRestore))) = 0
//__attribute__((__cleanup__(__iRestore))) = SREG

#endif /* ATOMIC_H_ */
