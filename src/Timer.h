#ifndef TIMER_H_
#define TIMER_H_

#include <inttypes.h>

#define INTERRUPT_PERIOD_MICROSECONDS     512
#define TIMER_SLOW_INTERRUPT_INTERVAL    225
#define SLOW_INTERRUPT_PERIOD_MILISECONDS ((long)INTERRUPT_PERIOD_MICROSECONDS*TIMER_SLOW_INTERRUPT_INTERVAL/1000)

class Timer {
    volatile uint32_t interrupts_;
public:
    Timer();
    void init();
    void doInterrupt();
    uint32_t getInterrupts() const { return interrupts_; }
    uint32_t getMiliseconds() const;
    void delay(uint16_t ms) const;
};

extern Timer timer;

#endif /* TIMER_H_ */
