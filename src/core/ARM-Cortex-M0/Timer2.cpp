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
#include "Timer.h"
#include "Hardware.h"

extern "C" {
#include "DrvTIMER.h"
}
// time measurement - measure TIMER_INTERRUPT_PERIOD_MICROSECONDS

extern "C" {
void TMR0_IRQHandler(void)
{
    TIMER0->TISR.TIF = 1;
    Timer::callback();
}
}


void Timer::initialize()
{
    //TODO: implement

    DrvTIMER_Init();
    DrvTIMER_Open(E_TMR0, 1000000/TIMER_INTERRUPT_PERIOD_MICROSECONDS, E_PERIODIC_MODE);
    DrvTIMER_EnableInt(E_TMR0);
    DrvTIMER_Start(E_TMR0);             /* Start counting */

}
