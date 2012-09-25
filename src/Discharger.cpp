#include "Discharger.h"
#include "Hardware.h"
#include "TimerOne.h"
#include "Utils.h"

Discharger::Discharger():
startTime_(0), discharge_(0)
{
	pinMode(DISCHARGE_VALUE_PIN, OUTPUT);
	pinMode(DISCHARGE_DISABLE_PIN, OUTPUT);

	setValue(0);
	powerOff(DISCHARGING_COMPLETE);
}


AnalogInputs::ValueType Discharger::getVout()
{
	return analogInputs.getRealValue(VName);
}
AnalogInputs::ValueType Discharger::getIout()
{
	return analogInputs.getRealValue(IName);
}

void Discharger::setValue(uint16_t value)
{
	if(value > DISCHARGER_UPPERBOUND_VALUE)
		value = DISCHARGER_UPPERBOUND_VALUE;
	valueSet_ = value;
	finalizeValueTintern(true);
}

uint16_t Discharger::correctValueTintern(uint16_t v)
{
	testTinern(tempcutoff_, DISCHARGER_TEMPERATURE_ENABLE, DISCHARGER_TEMPERATURE_DISABLE);

	if(tempcutoff_)
		v = 0;
	return v;
}

void Discharger::finalizeValueTintern(bool force)
{
	uint16_t  v = correctValueTintern(valueSet_);
	if(v != value_ || force) {
		value_ = v;
		Timer1.pwm(DISCHARGE_VALUE_PIN, value_);
		analogInputs.reset();
	}
}

void Discharger::setRealValue(uint16_t I)
{
	uint16_t value = analogInputs.reverseCalibrateValue(AnalogInputs::IdischargeValue, I);
	setValue(value);
}

void Discharger::powerOn()
{
	if(isPowerOn())
		return;

	hardware::setBatteryOutput(true);
	setValue(0);
	digitalWrite(DISCHARGE_DISABLE_PIN, false);
	analogInputs.doFullMeasurement();
	state_ = DISCHARGING;
	startTime_ = timer.getMiliseconds();
	discharge_ = 0;
}

void Discharger::powerOff(STATE reason)
{
	if(!isPowerOn() || reason == DISCHARGING)
		return;

	digitalWrite(DISCHARGE_DISABLE_PIN, true);
	hardware::setBatteryOutput(false);
	state_ = reason;
}

void Discharger::doInterrupt()
{
    if(isPowerOn()) {
            discharge_+=getIout();
            finalizeValueTintern(false);
    }
}

uint32_t Discharger::getOnTimeSec() const
{
	return (timer.getMiliseconds() - startTime_ ) / 1000;
}

double Discharger::getDischarge() const
{
	double retu = discharge_;
	retu *= INTERRUPT_PERIOD_MICROSECONDS;
	retu /= 1000000;
	retu /= 3600;
	return retu;
}
