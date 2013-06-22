#include "Hardware.h"
#include "TimerOne.h"
#include "imaxB6-pins.h"
#include "SMPS_PID.h"


namespace {
    volatile uint16_t PID_setpoint;
    volatile uint16_t PID_CutOff;
    volatile long PID_MV;
    volatile bool PID_enable;
    volatile bool PID_newMeasurement;
}

#define A 1024

void hardware::doInterrupt()
{
    if(!PID_enable) return;
    if(!PID_newMeasurement) return;
    PID_newMeasurement = false;

    //test Vout and cut-off if too high
    if(analogInputs.getMeasuredValue(AnalogInputs::Vout) > PID_CutOff) {
        setChargerOutput(false);
        return;
    }

    //TODO: PID
    //this is the PID - actually it is an I
    uint16_t PV = analogInputs.getMeasuredValue(AnalogInputs::Ismps);
    long error = PID_setpoint;
    error -= PV;
    PID_MV += A*error;

    if(PID_MV<0) PID_MV = 0;
    SMPS_PID::setPID_MV(PID_MV>>16);
}


void SMPS_PID::newIsmpsMeasurement()
{
    PID_newMeasurement = true;
}


void SMPS_PID::init()
{
    PID_setpoint = 0;
    PID_MV = 0;
    PID_enable = false;
}

namespace {
    void enableChargerValue0() {
        TimerOne::disablePWM(SMPS_VALUE0_PIN);
        digitalWrite(SMPS_VALUE0_PIN, 1);
    }
    void disableChargerValue0() {
        TimerOne::disablePWM(SMPS_VALUE0_PIN);
        digitalWrite(SMPS_VALUE0_PIN, 0);
    }
    void disableChargerValue1() {
        TimerOne::disablePWM(SMPS_VALUE1_PIN);
        digitalWrite(SMPS_VALUE1_PIN, 0);
    }
}

void SMPS_PID::setPID_MV(uint16_t value) {
    if(value > MAX_PID_MV) //TODO:remove
        value = MAX_PID_MV;

    if(value <= TIMERONE_PRECISION_PERIOD) {
        disableChargerValue1();
        TimerOne::setPWM(SMPS_VALUE0_PIN, value);
    } else {
        enableChargerValue0();
        uint16_t v2 = value - TIMERONE_PRECISION_PERIOD;
        TimerOne::setPWM(SMPS_VALUE1_PIN, v2);
    }
}

void hardware::setChargerValue(uint16_t value)
{
    PID_setpoint = analogInputs.reverseCalibrateValue(AnalogInputs::Ismps, value);
    PID_CutOff = analogInputs.reverseCalibrateValue(AnalogInputs::Vout, MAX_CHARGE_V);
}

void hardware::setChargerOutput(bool enable)
{
    disableChargerValue0();
    disableChargerValue1();
    SMPS_PID::init();
    PID_enable = enable;
    digitalWrite(SMPS_DISABLE_PIN, !enable);
}


void hardware::setDischargerOutput(bool enable)
{
    digitalWrite(DISCHARGE_DISABLE_PIN, !enable);
}

void hardware::setDischargerValue(uint16_t value)
{
    TimerOne::setPWM(DISCHARGE_VALUE_PIN, value<<(TIMERONE_PRECISION));
}

