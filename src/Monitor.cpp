#include "AnalogInputs.h"
#include "Hardware.h"

#include "Monitor.h"
#include "Utils.h"
#include "Settings.h"

Monitor monitor;


void Monitor::doInterrupt()
{
    bool on;
    if(testTinern(on, settings.fanTempOff_, settings.fanTempOn_))
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

