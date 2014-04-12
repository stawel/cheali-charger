#ifndef TIMER_1_H_
#define TIMER_1_H_

#include "HardwareConfig.h"

#define TIMER1_PRECISION 6
#define TIMER1_PRECISION_PERIOD (TIMER1_PERIOD<<TIMER1_PRECISION)

namespace Timer1
{
    void initialize();
    void disablePWM(char pin);
    void setPWM(char pin, unsigned int duty);
};

#endif //TIMER_1_H_
