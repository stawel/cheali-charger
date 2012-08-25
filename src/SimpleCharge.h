#ifndef SIMPLECHARGE_H_
#define SIMPLECHARGE_H_


#include "ChargingStrategy.h"
#include "AnalogInputs.h"


class SimpleCharge : public ChargingStrategy {
public:
	SimpleCharge(){};
	virtual void powerOn();
	virtual statusType doStrategy();
	virtual void powerOff();
	bool isStable();
};


#endif /* SIMPLECHARGE_H_ */

