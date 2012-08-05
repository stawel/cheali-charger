#ifndef SIMPLECHARGE_H_
#define SIMPLECHARGE_H_


#include "ChargingStrategy.h"
#include "AnalogInputs.h"


class SimpleCharge : public ChargingStrategy {
public:
	SimpleCharge(){};
	virtual void powerOn(SMPS &s);
	virtual void doStrategy(SMPS &);
	bool isStable(SMPS &s);
};


#endif /* SIMPLECHARGE_H_ */

