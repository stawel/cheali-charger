#ifndef DELAYSTRATEGY_H_
#define DELAYSTRATEGY_H_

#include "Strategy.h"

namespace DelayStrategy {
    
    
    
    
    
    extern const Strategy::VTable vtable;
    bool isDelay();
    void powerOn();
    Strategy::statusType doStrategy();
    void powerOff();
    void setDelay(int16_t minutes);
   
};


#endif /* DELAYSTRATEGY_H_ */