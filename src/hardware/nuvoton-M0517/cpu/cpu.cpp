/*
    cheali-charger - open source firmware for a variety of LiPo chargers
    Copyright (C) 2014 Paweł Stawicki. All right reserved.

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

extern "C" {
#include "M051Series.h"
}

uint8_t __atomic_h_irq_count;

namespace cpu {
    void init() {
    	__atomic_h_irq_count = 0;

        /* Unlock protected registers */
        SYS_UnlockReg();

        /* Enable external XTAL 12MHz clock */
        CLK_EnableXtalRC(CLK_PWRCON_XTL12M_EN_Msk);

        /* Waiting for external XTAL clock ready */
        CLK_WaitClockReady(CLK_CLKSTATUS_XTL12M_STB_Msk);

//        CLK_SetHCLK(CLK_CLKSEL0_HCLK_S_HXT, CLK_CLKDIV_HCLK(1));
        CLK_SetCoreClock(FREQ_50MHZ);
        SYS_LockReg();

    }
}

