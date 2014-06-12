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
#include "memory.h"

extern "C" {
#include "DrvFMC.h"
#include "DrvSYS.h"
}

#define PAGE_SIZE         512
#define PAGE_SIZE_32B     128


namespace eeprom {

void write_impl_less(void * addressE, const void * data, int size)
{
	uint32_t buf[PAGE_SIZE_32B];
	int i;
	uint32_t * p_adr_start = (uint32_t*) (((int)addressE) & (~(PAGE_SIZE - 1)));
	uint8_t * p_adr_pos =  ((uint8_t*)buf) + (((int)addressE) & (PAGE_SIZE - 1));

	for(i=0; i<PAGE_SIZE_32B; i++)
		buf[i] = p_adr_start[i];

	for(i=0; i<size; i++)
		p_adr_pos[i] = ((uint8_t*)data)[i];


	DrvFMC_Erase((uint32_t)p_adr_start);

	for(i = 0; i < PAGE_SIZE_32B; i++)
		DrvFMC_Write((uint32_t)&p_adr_start[i], buf[i]);

}

void write_impl(void * addressE, const void * data, int size)
{
	DrvFMC_EnableISP();

	while(size > 0) {
		int adr = (int) addressE & (PAGE_SIZE - 1);
		int less_size;
		if(adr+size > PAGE_SIZE) {
			 less_size = PAGE_SIZE - adr;
		} else {
			less_size = size;
		}
		write_impl_less(addressE, data, less_size);
		addressE += less_size;
		data += less_size;
		size -= less_size;
	}


	/* Disable ISP function */
	DrvFMC_DisableISP();

}

} // namespace eeprom

