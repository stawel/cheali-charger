#ifndef STORAGE_H_
#define STORAGE_H_

#include "SimpleCharge.h"

class Storage : public Strategy {
public:
	enum State  {Charge, Discharge, Balance};

	AnalogInputs::ValueType V_;
	State state;
	bool doBalance_;

	Storage(){}
	virtual void powerOn();
	virtual statusType doStrategy();
	virtual void powerOff();
	void setVII(AnalogInputs::ValueType V, AnalogInputs::ValueType Ic, AnalogInputs::ValueType Id);
	void setDoBalance(bool v);
};

extern Storage storage;


#endif /* STORAGE_H_ */
