#include "Timer.h"
#include "TimerOne.h"
#include "Hardware.h"


static void callback() {
	timer.doInterrupt();
	smps.doInterrupt();

}



Timer::Timer() : _interrupts(0) {
}

void Timer::init()
{
	Timer1.initialize(INTERRUPT_PERIOD_MICROSECONDS);         // initialize timer1, and set a 1/2 second period
	Timer1.attachInterrupt(callback);  // attaches callback() as a timer overflow interrupt
}

void Timer::doInterrupt()
{
	_interrupts++;
}
uint32_t Timer::getMiliseconds() const
{
	uint32_t retu = _interrupts;
	retu *= INTERRUPT_PERIOD_MICROSECONDS;
	retu /= 1000;

	return retu;
}

void Timer::delay(uint32_t ms) const
{
	uint32_t end;
	end = getMiliseconds() + ms;

	while(getMiliseconds() < end) {};
}

