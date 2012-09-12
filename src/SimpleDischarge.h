#ifndef SIMPLEDISCHARGE_H_
#define SIMPLEDISCHARGE_H_

#include "ChargingStrategy.h"
#include "AnalogInputs.h"

class SimpleDischarge : public ChargingStrategy {
public:

	AnalogInputs::ValueType I_;
	AnalogInputs::ValueType V_;

	SimpleDischarge(){}
	virtual void powerOn();
	virtual statusType doStrategy();
	virtual void powerOff();

	bool isMinVout() const;
	void setVI(AnalogInputs::ValueType V, AnalogInputs::ValueType I) { I_ = I; V_ = V; };
};

extern SimpleDischarge simpleDischarge;


#endif /* SIMPLEDISCHARGE_H_ */
