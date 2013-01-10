#ifndef SIMPLECHARGE_H_
#define SIMPLECHARGE_H_


#include "Strategy.h"

class SimpleCharge : public Strategy {
public:
    bool testVout_;
    SimpleCharge(){};
    virtual void powerOn();
    virtual statusType doStrategy();
    virtual void powerOff();
    bool isStable() const;
    void calculateThevenin() const;
};

extern SimpleCharge simpleCharge;

#endif /* SIMPLECHARGE_H_ */

