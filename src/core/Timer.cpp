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
#include "SerialLog.h"
#include "Discharger.h"
#include "AnalogInputsPrivate.h"
#include "atomic.h"


// time measurement

namespace Timer {
    volatile uint32_t interrupts_ = 0;

    uint32_t getInterrupts() {
        uint32_t v;
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            v = interrupts_;
        }
        return v;
    }
    inline void doInterrupt() {
        interrupts_++;
    }

    void doIdle() {
        Monitor::doIdle();
        SerialLog::doIdle();
        Discharger::doIdle();
        Buzzer::doIdle();
        AnalogInputs::doIdle();
    }

    void callback() {
        static uint8_t slowInterval = TIMER_SLOW_INTERRUPT_INTERVAL;
        Timer::doInterrupt();
        if(--slowInterval == 0){
            slowInterval = TIMER_SLOW_INTERRUPT_INTERVAL;
            AnalogInputs::doSlowInterrupt();
            Screen::doSlowInterrupt();
        }
    }
}

uint32_t Timer::getMiliseconds()
{
    uint32_t retu = getInterrupts();
#if TIMER_INTERRUPT_PERIOD_MICROSECONDS == 500
    retu /= 2;
#else
#warning "TIMER_INTERRUPT_PERIOD_MICROSECONDS != 500"
    retu *= TIMER_INTERRUPT_PERIOD_MICROSECONDS;
    retu /= 1000;
#endif

    return retu;
}

void Timer::delay(uint16_t ms)
{
    uint32_t end;
    end = getMiliseconds() + ms;

    while(getMiliseconds() < end) {};
}

void Timer::delayIdle(uint16_t ms)
{
    uint32_t end;
    end = getMiliseconds() + ms;

    while(getMiliseconds() < end) {
        doIdle();
    };
}

