#ifndef DELTACHARGESTRATEGY_H_
#define DELTACHARGESTRATEGY_H_


#include "SimpleCharge.h"

class DeltaChargeStrategy : public SimpleCharge {
public:
    enum StateType {PreCharge, RapidCharge};

    bool testDeltaT_;
    bool testDeltaV_;

    StateType state_;

    DeltaChargeStrategy(){};

    virtual void powerOn();
    virtual statusType doStrategy();
    void setTestTV(bool t, bool v) {
        testDeltaT_ = t;
        testDeltaV_ = v;
    }
};

extern DeltaChargeStrategy deltaChargeStrategy;

#endif /* DELTACHARGESTRATEGY_H_ */

