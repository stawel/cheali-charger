#include "Hardware.h"
#include "imaxB6-pins.h"
#include "SMPS_PID.h"
#include "IO.h"
#include "AnalogInputs.h"
#include "atomic.h"

namespace {
    volatile uint16_t i_PID_setpoint;
    //we have to use i_PID_CutOffVoltage, on some chargers (M0516) ADC can read up to 60V
    volatile uint16_t i_PID_CutOffVoltage;
    volatile long i_PID_MV;
    volatile bool i_PID_enable;
}

#define A 4

uint16_t hardware::getPIDValue()
{
}


void SMPS_PID::update()
{
}

void SMPS_PID::init(uint16_t Vin, uint16_t Vout)
{
}


void SMPS_PID::setPID_MV(uint16_t value) {
}

void hardware::setVoutCutoff(AnalogInputs::ValueType v) {
}

void hardware::setChargerValue(uint16_t value)
{
}

void hardware::setChargerOutput(bool enable)
{
}


void hardware::setDischargerOutput(bool enable)
{
}

void hardware::setDischargerValue(uint16_t value)
{
}

