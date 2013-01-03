#ifndef STARTINFOSTRATEGY_H_
#define STARTINFOSTRATEGY_H_


#include "Strategy.h"

class StartInfoStrategy : public Strategy {
public:
    uint8_t ok_;
    bool balancePort_;
    StartInfoStrategy(){};
    virtual void powerOn();
    virtual statusType doStrategy();
    virtual void powerOff();
    void setBalancePort(bool p) {balancePort_ = p;}
};

extern StartInfoStrategy startInfoStrategy;

#endif /* STARTINFOSTRATEGY_H_ */

