#include "Hardware.h"
#include "imaxB6-pins.h"
#include "SMPS_PID.h"
#include "IO.h"
#include "AnalogInputs.h"


namespace {
    volatile uint16_t PID_setpoint;
    volatile uint16_t PID_CutOff;
    volatile long PID_MV;
    volatile bool PID_enable;
}

#define A 4

uint16_t hardware::getPIDValue()
{
    return PID_MV>>PID_MV_PRECISION;
}


void SMPS_PID::update()
{
    if(!PID_enable) return;
    //if Vout is too high disable PID
    if(AnalogInputs::getADCValue(AnalogInputs::Vout_plus_pin) > PID_CutOff) {
        hardware::setChargerOutput(false);
        PID_enable = false;
        return;
    }

    //TODO: rewrite PID
    //this is the PID - actually it is an I (Integral part) - should be rewritten
    uint16_t PV = AnalogInputs::getADCValue(AnalogInputs::Ismps);
    long error = PID_setpoint;
    error -= PV;
    PID_MV += error*A;

/*    if(PID_MV<0) PID_MV = 0;
    if(PID_MV > MAX_PID_MV_PRECISION) {
        PID_MV = MAX_PID_MV_PRECISION;
    }
*/
    SMPS_PID::setPID_MV(PID_MV>>PID_MV_PRECISION);
}

void SMPS_PID::init(uint16_t Vin, uint16_t Vout)
{
/*
    PID_setpoint = 0;
    if(Vout>Vin) {
        PID_MV = TIMER1_PRECISION_PERIOD;
    } else {
        PID_MV = 0;
    }
    PID_MV <<= PID_MV_PRECISION;
    PID_enable = true;
*/
}

namespace {
    void enableChargerBuck() {
//        Timer1::disablePWM(SMPS_VALUE_BUCK_PIN);
        IO::digitalWrite(SMPS_VALUE_BUCK_PIN, 1);
    }
    void disableChargerBuck() {
//        Timer1::disablePWM(SMPS_VALUE_BUCK_PIN);
        IO::digitalWrite(SMPS_VALUE_BUCK_PIN, 0);
    }
    void disableChargerBoost() {
//        Timer1::disablePWM(SMPS_VALUE_BOOST_PIN);
        IO::digitalWrite(SMPS_VALUE_BOOST_PIN, 0);
    }
}

void SMPS_PID::setPID_MV(uint16_t value) {
/*
    if(value > MAX_PID_MV)
        value = MAX_PID_MV;

    if(value <= TIMER1_PRECISION_PERIOD) {
        disableChargerBoost();
        Timer1::setPWM(SMPS_VALUE_BUCK_PIN, value);
    } else {
        enableChargerBuck();
        uint16_t v2 = value - TIMER1_PRECISION_PERIOD;
        Timer1::setPWM(SMPS_VALUE_BOOST_PIN, v2);
    }
*/
}

void hardware::setChargerValue(uint16_t value)
{

    PID_setpoint = value;
    PID_CutOff = AnalogInputs::reverseCalibrateValue(AnalogInputs::Vout_plus_pin, PID_CUTOFF_VOLTAGE);
}

void hardware::setChargerOutput(bool enable)
{
    if(enable) setDischargerOutput(false);
    disableChargerBuck();
    disableChargerBoost();
    PID_enable = false;
    IO::digitalWrite(SMPS_DISABLE_PIN, !enable);
    if(enable) {
        SMPS_PID::init(AnalogInputs::getRealValue(AnalogInputs::Vin), AnalogInputs::getRealValue(AnalogInputs::Vout_plus_pin));
    }
}


void hardware::setDischargerOutput(bool enable)
{
    if(enable) setChargerOutput(false);
    IO::digitalWrite(DISCHARGE_DISABLE_PIN, !enable);
}

void hardware::setDischargerValue(uint16_t value)
{
//    Timer1::setPWM(DISCHARGE_VALUE_PIN, value);
}

