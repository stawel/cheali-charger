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
#include "Time.h"
#include "Hardware.h"
#include "irq_priority.h"

extern "C" {
#include "M051Series.h"
}
// time measurement - measure TIMER_INTERRUPT_PERIOD_MICROSECONDS

extern "C" {
void TMR0_IRQHandler(void)
{
    /* Clear Timer0 time-out interrupt flag */
    TIMER_ClearIntFlag(TIMER0);
    Time::callback();
}
}


void Time::initialize()
{
	CLK_EnableModuleClock(TMR0_MODULE);
	CLK_SetModuleClock(TMR0_MODULE,CLK_CLKSEL1_TMR0_S_HCLK,CLK_CLKDIV_UART(1));
    TIMER_Open(TIMER0, TIMER_PERIODIC_MODE, 1000000/TIMER_INTERRUPT_PERIOD_MICROSECONDS);
    TIMER_EnableInt(TIMER0);
    NVIC_EnableIRQ(TMR0_IRQn);
    NVIC_SetPriority(TMR0_IRQn, TIMER_IRQ_PRIORITY);
    TIMER_Start(TIMER0);             /* Start counting */

}
