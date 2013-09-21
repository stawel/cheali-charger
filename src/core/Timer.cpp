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
#include "Monitor.h"
#include "Buzzer.h"
#include "Screen.h"
#include <util/atomic.h>

static void callback() {
    static uint8_t slowInterval = TIMER_SLOW_INTERRUPT_INTERVAL;
    timer.doInterrupt();
    Buzzer::doInterrupt();
    Monitor::doInterrupt();
    hardware::doInterrupt();
    if(--slowInterval == 0){
        slowInterval = TIMER_SLOW_INTERRUPT_INTERVAL;
        smps.doSlowInterrupt();
        discharger.doSlowInterrupt();
        screen.doSlowInterrupt();
    }
}

ISR(TIMER0_COMP_vect)
{
    callback();
}


Timer timer;


Timer::Timer() : interrupts_(0) {
}

void Timer::init()
{
#if F_CPU != 16000000
#error "F_CPU != 16000000 - not implemented"
#endif
#if TIMER_INTERRUPT_PERIOD_MICROSECONDS != 512
#error "TIMER_INTERRUPT_PERIOD_MICROSECONDS != 512 - not implemented"
#endif

    TCNT0=0;
    OCR0=TIMER_INTERRUPT_PERIOD_MICROSECONDS/4;
    TCCR0=(1<<WGM01);
    TCCR0|=(1<<CS00) | (1<<CS01);   //clk/64 (From prescaler)
    TIMSK|=(1<<OCIE0);

}

void Timer::doInterrupt()
{
    interrupts_++;
}
uint32_t Timer::getMiliseconds() const
{
    uint32_t retu;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        retu = interrupts_;
    }
#if TIMER_INTERRUPT_PERIOD_MICROSECONDS == 512
    retu *= (TIMER_INTERRUPT_PERIOD_MICROSECONDS/8);
    retu /= (1000/8);
#else
#warning "TIMER_INTERRUPT_PERIOD_MICROSECONDS != 512"
    retu *= TIMER_INTERRUPT_PERIOD_MICROSECONDS;
    retu /= 1000;
#endif

    return retu;
}

void Timer::delay(uint16_t ms) const
{
    uint32_t end;
    end = getMiliseconds() + ms;

    while(getMiliseconds() < end) {};
}

