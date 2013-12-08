#ifndef DELAYSTRATEGY_H_
#define DELAYSTRATEGY_H_

#include "Strategy.h"

namespace DelayStrategy 
{
    extern const Strategy::VTable vtable;
    
    void powerOn();
    void powerOff();
    Strategy::statusType doStrategy();
    void setDelay(int16_t minutes);
    bool isDelay();
   
};


#endif /* DELAYSTRATEGY_H_ */