//Switched-mode power supply

#ifndef SMPS_H_
#define SMPS_H_

#include "AnalogInputs.h"
#include "Timer.h"

#define MAX_SMPS_VALUE 			((F_CPU / 2000000) * INTERRUPT_PERIOD_MICROSECONDS)
#define UPPERBOUND_SMP_VALUE 	(MAX_SMPS_VALUE/4)

class ChargingStrategy;

class SMPS {
public:
	enum STATE { CHARGING, CHARGING_COMPLETE, ERROR, STOP};

	SMPS(AnalogInputs::Name inV, AnalogInputs::Name inI);

	AnalogInputs::ValueType getVout() const;
	AnalogInputs::ValueType getIout() const;

	STATE getState() const { return state_; }
	bool isPowerOn() const { return getState() == CHARGING; }

	uint16_t getValue() const { return value_; }
	void setValue(uint16_t value);
	void setRealValue(uint16_t I);

	void powerOn();
	void powerOff(STATE reason = CHARGING_COMPLETE);

	void doInterrupt();

	void setError(const char * error1, const char * error2);
	const char * getError1() { return error1_; }
	const char * getError2() { return error2_; }

	uint32_t getOnTimeSec() const;
	double getCharge() const;

	AnalogInputs::Name Vm_;
	AnalogInputs::Name Im_;
protected:

	STATE state_;
	uint16_t value_;
	const char *error1_, *error2_;
	uint16_t startTime_;
	double charge_;
};



#endif /* SMPS_H_ */
