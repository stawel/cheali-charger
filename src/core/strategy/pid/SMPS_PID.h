#ifndef SMPS_PID_H_
#define SMPS_PID_H_

#include "Hardware.h"

namespace SMPS_PID
{
    void initialize();
    void powerOn();
    void powerOff();

    void update();
    void setIoutPWM(uint16_t adcIout);
    void setVoutPWM(uint16_t adcVout);
};

#endif //SMPS_PID_H_
