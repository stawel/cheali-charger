#include "AnalogInputs.h"
#include "Hardware.h"

#include "Monitor.h"


Monitor monitor;


void Monitor::doInterrupt()
{
	AnalogInputs::ValueType t = analogInputs.getRealValue(AnalogInputs::Tintern);

	//TODO: make configurable
	if(t < ANALOG_CELCIUS(40))
		hardware::setFan(false);

	if(t > ANALOG_CELCIUS(50))
		hardware::setFan(true);
}

Monitor::statusType Monitor::run()
{

	return monitorInternalTemperature();
}

Monitor::statusType Monitor::monitorInternalTemperature()
{
	AnalogInputs::ValueType t = analogInputs.getRealValue(AnalogInputs::Tintern);

	//TODO: make configurable
	if(t > ANALOG_CELCIUS(60))
		return ERROR;

	return OK;
}

