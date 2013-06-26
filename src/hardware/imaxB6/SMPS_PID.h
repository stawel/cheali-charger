#ifndef SMPS_PID_H_
#define SMPS_PID_H_

#include "TimerOne.h"

//MV - manipulated variable in PID
#define MAX_PID_MV_FACTOR 1.3
#define MAX_PID_MV ((uint16_t) (TIMERONE_PRECISION_PERIOD * MAX_PID_MV_FACTOR))

namespace SMPS_PID
{
    void init();
    void setPID_MV(uint16_t value);
    void powerOn();
    void powerOff();
    void update();
};

#endif //SMPS_PID_H_
