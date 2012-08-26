#ifndef SIMPLEDISCHARGE_H_
#define SIMPLEDISCHARGE_H_

#include "ChargingStrategy.h"
#include "AnalogInputs.h"

class SimpleDischarge : public ChargingStrategy {
public:

	SimpleDischarge(){}
	virtual void powerOn();
	virtual statusType doStrategy();
	virtual void powerOff();

	bool isMinVout() const;
};

#endif /* SIMPLEDISCHARGE_H_ */
