#ifndef SMPS_PID_H_
#define SMPS_PID_H_

namespace SMPS_PID
{
    void init();
    void setPID_MV(uint16_t value);
    void powerOn();
    void powerOff();
};

#endif //SMPS_PID_H_
