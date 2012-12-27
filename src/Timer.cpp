#include "Timer.h"
#include "TimerOne.h"
#include "Hardware.h"
#include "Monitor.h"
#include "Buzzer.h"

static void callback() {
	timer.doInterrupt();
	smps.doInterrupt();
	discharger.doInterrupt();
	monitor.doInterrupt();
	buzzer.doInterrupt();
}


Timer timer;


Timer::Timer() : interrupts_(0) {
}

void Timer::init()
{
	Timer1.initialize(INTERRUPT_PERIOD_MICROSECONDS);         // initialize timer1, and set a 1/2 second period
	Timer1.attachInterrupt(callback);  // attaches callback() as a timer overflow interrupt
}

void Timer::doInterrupt()
{
	interrupts_++;
}
uint32_t Timer::getMiliseconds() const
{
	uint32_t retu = interrupts_;
	retu *= INTERRUPT_PERIOD_MICROSECONDS;
	retu /= 1000;

	return retu;
}

void Timer::delay(uint16_t ms) const
{
	uint32_t end;
	end = getMiliseconds() + ms;

	while(getMiliseconds() < end) {};
}

