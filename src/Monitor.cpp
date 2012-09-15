#include "AnalogInputs.h"
#include "Hardware.h"

#include "Monitor.h"
#include "Utils.h"


Monitor monitor;


void Monitor::doInterrupt()
{
	bool on;
	//TODO: make configurable
	if(testTinern(on, ANALOG_CELCIUS(40), ANALOG_CELCIUS(50)))
		hardware::setFan(on);
}

Monitor::statusType Monitor::run()
{

	return monitorInternalTemperature();
}

Monitor::statusType Monitor::monitorInternalTemperature()
{
	AnalogInputs::ValueType t = analogInputs.getRealValue(AnalogInputs::Tintern);

	//TODO: make configurable
	if(t > ANALOG_CELCIUS(65))
		return ERROR;

	return OK;
}

