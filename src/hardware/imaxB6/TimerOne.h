#ifndef TIMERONE_H_
#define TIMERONE_H_

#define TIMERONE_PERIOD 256
namespace TimerOne
{
    void initialize();
    void disablePWM(char pin);
    void setPWM(char pin, unsigned int duty);
};

#endif //TIMERONE_H_
