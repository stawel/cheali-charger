#ifndef TIMERONE_H_
#define TIMERONE_H_

#define TIMERONE_PERIOD 256
#define TIMERONE_PRECISION 6
#define TIMERONE_PRECISION_PERIOD (TIMERONE_PERIOD<<TIMERONE_PRECISION)

namespace TimerOne
{
    void initialize();
    void disablePWM(char pin);
    void setPWM(char pin, unsigned int duty);
};

#endif //TIMERONE_H_
