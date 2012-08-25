//#include <Arduino.h>
#include "TimerOne.h"
#include "Hardware.h"
#include "SMPS.h"
#include "ChargingStrategy.h"

#ifndef DUMMYSMPS_H_

SMPS::SMPS(AnalogInputs::Name inV, AnalogInputs::Name inI):
Vm_(inV), Im_(inI),
startTime_(0), charge_(0)
{
	pinMode(SMPS_VALUE_PIN, OUTPUT);
	pinMode(SMPS_DISABLE_PIN, OUTPUT);

	setValue(0);
	powerOff(CHARGING_COMPLETE);
}


void SMPS::setError(const char * error1,const char * error2)
{
	powerOff(ERROR);
	error1_ = error1;
	error2_ = error2;
	return;
}

void SMPS::setValue(uint16_t value)
{
	if(value > UPPERBOUND_SMP_VALUE)
		value = UPPERBOUND_SMP_VALUE;
	value_ = value;
	Timer1.pwm(SMPS_VALUE_PIN, value_);
}
void SMPS::setRealValue(uint16_t I)
{
	uint16_t value = analogInputs.reverseCalibrateValue(AnalogInputs::IsmpsValue, I);
	setValue(value);
}

void SMPS::powerOn()
{
	if(isPowerOn())
		return;

	hardware::setBatteryOutput(true);
	setValue(0);
	digitalWrite(SMPS_DISABLE_PIN, false);
	analogInputs.doFullMeasurement();
	state_ = CHARGING;
	startTime_ = timer.getMiliseconds();
	charge_ = 0;
}

#include "GTPowerA610.h"


void SMPS::powerOff(STATE reason)
{
	if(!isPowerOn() || reason == CHARGING)
		return;

	digitalWrite(SMPS_DISABLE_PIN, true);
	hardware::setBatteryOutput(false);
	state_ = reason;
	setError(PSTR("charging"), PSTR("complete !!"));
}

void SMPS::doInterrupt()
{
       if(isPowerOn()) {
               charge_+=getIout();
       }
}

uint32_t SMPS::getOnTimeSec() const
{
	return (timer.getMiliseconds() - startTime_ ) / 1000;
}

double SMPS::getCharge() const
{
	double retu = charge_;
	retu *= INTERRUPT_PERIOD_MICROSECONDS;
	retu /= 1000000;
	retu /= 3600;
	return retu;
}

AnalogInputs::ValueType SMPS::getVout() const
{
	return analogInputs.getRealValue(Vm_);
}

AnalogInputs::ValueType SMPS::getIout() const
{
	return analogInputs.getRealValue(Im_);
}


#endif //DUMMYSMPS_H_

