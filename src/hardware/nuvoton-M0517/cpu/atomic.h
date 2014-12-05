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
extern "C" {
#include "M051Series.h"
}

extern uint8_t __atomic_h_irq_count;
static __inline__ uint8_t __iCliRetVal(void)
{
    __disable_irq();
    __atomic_h_irq_count++;
    return __atomic_h_irq_count;
}

static __inline__ void __iRestore(uint8_t *__s)
{
    __atomic_h_irq_count --;
    if(__atomic_h_irq_count == 0) {
        __enable_irq();
    }
}


#define ATOMIC_BLOCK(type) for ( type = __iCliRetVal(), __ToDo =1; \
                           __ToDo ; __ToDo = 0 )

#define ATOMIC_RESTORESTATE uint8_t sreg_save \
    __attribute__((__cleanup__(__iRestore)))

#endif /* ATOMIC_H_ */
