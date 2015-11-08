#ifndef SMPS_PID_H_
#define SMPS_PID_H_

#include "Hardware.h"

#define SMPS_PID_UPDATE_TYPE_VOLTAGE 1
#define SMPS_PID_UPDATE_TYPE_CURRENT 2

namespace SMPS_PID
{
    void initialize();
    void powerOn();
    void powerOff();

    void update(uint8_t type);
    void setIoutPWM(uint16_t adcIout);
    void setVoutPWM(uint16_t adcVout);
};

#endif //SMPS_PID_H_
