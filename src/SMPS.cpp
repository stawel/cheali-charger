#include "TimerOne.h"
#include "Hardware.h"
#include "SMPS.h"

SMPS::SMPS()
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
	if(value > SMPS_UPPERBOUND_VALUE)
		value = SMPS_UPPERBOUND_VALUE;
	value_ = value;
	Timer1.pwm(SMPS_VALUE_PIN, value_);
	analogInputs.reset();
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
               charge_+=getIcharge();
       }
}

uint32_t SMPS::getOnTimeSec() const
{
	return (timer.getMiliseconds() - startTime_ ) / 1000;
}

uint16_t SMPS::getCharge() const
{
	uint32_t retu = charge_;
#if INTERRUPT_PERIOD_MICROSECONDS == 512
//	retu *= INTERRUPT_PERIOD_MICROSECONDS;
	retu /= (1000000/64)*(3600/8);
#else
#warning "INTERRUPT_PERIOD_MICROSECONDS != 512"
	retu *= INTERRUPT_PERIOD_MICROSECONDS;
	retu /= 1000000;
	retu /= 3600;
#endif
	return retu;
}

AnalogInputs::ValueType SMPS::getVout() const
{
	return analogInputs.getRealValue(VName);
}

AnalogInputs::ValueType SMPS::getIcharge() const
{
	return analogInputs.getRealValue(IName);
}


