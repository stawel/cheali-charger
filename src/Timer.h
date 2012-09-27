#ifndef TIMER_H_
#define TIMER_H_

#include <inttypes.h>

#define INTERRUPT_PERIOD_MICROSECONDS 512

class Timer {
	volatile uint32_t interrupts_;
public:
	Timer();
	void init();
	void doInterrupt();
	uint32_t getInterrupts() const { return interrupts_; }
	uint32_t getMiliseconds() const;
	void delay(uint32_t ms) const;
};

extern Timer timer;

#endif /* TIMER_H_ */
