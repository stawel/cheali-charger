#include "Hardware.h"
#include "TimerOne.h"
#include "imaxB6-pins.h"
#include "SMPS_PID.h"


namespace {
    volatile uint16_t PID_setpoint;
    volatile uint16_t PID_CutOff;
    volatile long PID_MV;
    volatile bool PID_enable;
}

#define A 4

void SMPS_PID::update()
{
    if(!PID_enable) return;
    //if Vout is too high disable PID
    if(analogInputs.getMeasuredValue(AnalogInputs::Vout) > PID_CutOff) {
        hardware::setChargerOutput(false);
        PID_enable = false;
        return;
    }

    //TODO: rewrite PID
    //this is the PID - actually it is an I (Integral part) - should be rewritten
    uint16_t PV = analogInputs.getMeasuredValue(AnalogInputs::Ismps);
    long error = PID_setpoint;
    error -= PV;
    PID_MV += error*A;

    if(PID_MV<0) PID_MV = 0;
    if(PID_MV > MAX_PID_MV_PRECISION) {
        PID_MV = MAX_PID_MV_PRECISION;
    }
    SMPS_PID::setPID_MV(PID_MV>>PID_MV_PRECISION);
}

void SMPS_PID::init()
{
    PID_setpoint = 0;
    PID_MV = 0;
    PID_enable = false;
}

namespace {
    void enableChargerBuck() {
        TimerOne::disablePWM(SMPS_VALUE_BUCK_PIN);
        digitalWrite(SMPS_VALUE_BUCK_PIN, 1);
    }
    void disableChargerBuck() {
        TimerOne::disablePWM(SMPS_VALUE_BUCK_PIN);
        digitalWrite(SMPS_VALUE_BUCK_PIN, 0);
    }
    void disableChargerBoost() {
        TimerOne::disablePWM(SMPS_VALUE_BOOST_PIN);
        digitalWrite(SMPS_VALUE_BOOST_PIN, 0);
    }
}

void SMPS_PID::setPID_MV(uint16_t value) {
    if(value > MAX_PID_MV)
        value = MAX_PID_MV;

    if(value <= TIMERONE_PRECISION_PERIOD) {
        disableChargerBoost();
        TimerOne::setPWM(SMPS_VALUE_BUCK_PIN, value);
    } else {
        enableChargerBuck();
        uint16_t v2 = value - TIMERONE_PRECISION_PERIOD;
        TimerOne::setPWM(SMPS_VALUE_BOOST_PIN, v2);
    }
}

void hardware::setChargerValue(uint16_t value)
{

    PID_setpoint = analogInputs.reverseCalibrateValue(AnalogInputs::Ismps, value);
    PID_CutOff = analogInputs.reverseCalibrateValue(AnalogInputs::Vout, PID_CUTOFF_VOLTAGE);
}

void hardware::setChargerOutput(bool enable)
{
    if(enable) setDischargerOutput(false);
    disableChargerBuck();
    disableChargerBoost();
    SMPS_PID::init();
    PID_enable = enable;
    digitalWrite(SMPS_DISABLE_PIN, !enable);
}


void hardware::setDischargerOutput(bool enable)
{
    if(enable) setChargerOutput(false);
    digitalWrite(DISCHARGE_DISABLE_PIN, !enable);
}

void hardware::setDischargerValue(uint16_t value)
{
    TimerOne::setPWM(DISCHARGE_VALUE_PIN, value<<3);
}

