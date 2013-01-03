#ifndef THEVENINCHARGE_H_
#define THEVENINCHARGE_H_

#include "SimpleCharge.h"
#include "Thevenin.h"
#include "TheveninMethod.h"

class TheveninCharge : public SimpleCharge {
public:
    TheveninCharge(){}
    virtual void powerOn();
    virtual statusType doStrategy();
    virtual void powerOff();
    bool isEndVout() const;

    void setVI(AnalogInputs::ValueType v, AnalogInputs::ValueType i);
    void setMinI(AnalogInputs::ValueType i);

};

extern TheveninCharge theveninCharge;


#endif /* THEVENINCHARGE_H_ */
