#include "Hardware.h"
#include "SMPS_PID.h"
#include "IO.h"
#include "AnalogInputs.h"
#include "atomic.h"


void SMPS_PID::update(){}
void SMPS_PID::initialize(){}
//not implemented
void SMPS_PID::setVoutPWM(uint16_t adcVout){}


void SMPS_PID::powerOff()
{
    hardware::setChargerOutput(false);
}

void SMPS_PID::powerOn()
{
    hardware::setChargerOutput(true);
}


void SMPS_PID::setIoutPWM(uint16_t IoutPWM)
{
    hardware::setChargerValue(IoutPWM);
}

