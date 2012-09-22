#ifndef THEVENINVTLIMITCHARGE_H_
#define THEVENINVTLIMITCHARGE_H_

#include "SimpleCharge.h"
#include "AnalogInputs.h"

class TheveninVtLimitCharge {
public:

	AnalogInputs::ValueType VtLimit_;

	TheveninVtLimitCharge(){}
	void powerOn();
	ChargingStrategy::statusType doStrategy();
	void powerOff();

	void setVtLimit(AnalogInputs::ValueType v) {VtLimit_ = v;}
};

//extern TheveninVtLimitCharge theveninVtLimitCharge;


#endif /* THEVENINVTLIMITCHARGE_H_ */
