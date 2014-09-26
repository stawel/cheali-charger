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
#include "Timer0.h"
#include "Hardware.h"
#include "IO.h"

// ADC measurement interval


void Timer0::initialize()
{
#if F_CPU != 16000000
#error "F_CPU != 16000000 - not implemented"
#endif
    TCNT0=0;
    OCR0=TIMER0_FROM_MICROSECONDS(TIMER0_INTERRUPT_PERIOD_MICROSECONDS);
    TCCR0=(1<<WGM01);               //Clear Timer on Compare Match (CTC) Mode
    TCCR0|=(1<<CS00) | (1<<CS01);   //clk/64 (From prescaler)
    TIMSK|=(1<<OCIE0);              //OCIE0: Timer/Counter0 Output Compare Match Interrupt Enable
}
