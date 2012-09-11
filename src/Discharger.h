#ifndef DISCHARGER_H
#define DISCHARGER_H

#include "AnalogInputs.h"
#include "Timer.h"

#define UPPERBOUND_DISCHARGER_VALUE ((F_CPU / 2000000) * INTERRUPT_PERIOD_MICROSECONDS)

class Discharger {
public:
	enum STATE { DISCHARGING, DISCHARGING_COMPLETE, ERROR};
	static const AnalogInputs::Name VName = AnalogInputs::Vout;
	static const AnalogInputs::Name IName = AnalogInputs::Idischarge;

	Discharger();

	static AnalogInputs::ValueType getVout();
	static AnalogInputs::ValueType getIout();

	STATE getState() const { return state_; }
	bool isPowerOn() const { return getState() == DISCHARGING; }

	uint16_t getValue() const { return value_; }
	void setValue(uint16_t value);
	void setRealValue(uint16_t I);

	void powerOn();
	void powerOff(STATE reason = DISCHARGING_COMPLETE);

	void doInterrupt();

	uint32_t getOnTimeSec() const;
	double getDischarge() const;

protected:
	STATE state_;
	uint16_t value_;
	uint16_t startTime_;
	double discharge_;
};


#endif //DISCHARGER_H
