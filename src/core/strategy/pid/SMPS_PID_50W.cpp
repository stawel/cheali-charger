#include "Hardware.h"
#include "SMPS_PID.h"
#include "IO.h"
#include "AnalogInputs.h"
#include "outputPWM.h"
#include "atomic.h"

//MV - manipulated variable in PID
#define MAX_PID_MV_FACTOR 1.5
#define MAX_PID_MV ((uint16_t) (OUTPUT_PWM_PRECISION_PERIOD * MAX_PID_MV_FACTOR))
#define PID_MV_PRECISION 8
#define MAX_PID_MV_PRECISION (((uint32_t) MAX_PID_MV)<<PID_MV_PRECISION)


#define MIN( a, b ) ( ((a) < (b)) ? (a) : (b) )

namespace SMPS_PID {
    //prefix "i" - variable is used in interrupts
    volatile uint16_t i_setpoint_adcIout;
    volatile uint16_t i_setpoint_adcVout;
    //we have to use i_PID_CutOffVoltage, on some chargers (M0516) ADC can read up to 60V
    volatile uint16_t i_cutOffVoltage;
    volatile long i_MV;
    volatile bool i_enable;

    void setPID_MV(uint16_t value);
}

#define A 4

uint16_t hardware::getPIDValue()
{
    uint16_t v;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        v = SMPS_PID::i_MV>>PID_MV_PRECISION;
    }
    return v;
}


void SMPS_PID::update()
{
    if(!i_enable) return;
    //if Vout is too high disable PID

    AnalogInputs::ValueType adcVoutPlus  = AnalogInputs::getADCValue(AnalogInputs::Vout_plus_pin);
    AnalogInputs::ValueType adcVoutMinus = AnalogInputs::getADCValue(AnalogInputs::Vout_minus_pin);

    //safety check
    if(adcVoutPlus >= i_cutOffVoltage) {
        powerOff();
        return;
    }

    //PID
    //TODO: rewrite PID
    //this is the PID - actually it is an I (Integral part) - should be rewritten

    long error_Iout = i_setpoint_adcIout;
    error_Iout -= AnalogInputs::getADCValue(AnalogInputs::Ismps);


    long error_Vout = error_Iout;
    if(i_setpoint_adcVout) {
        error_Vout = i_setpoint_adcVout;
        error_Vout -= adcVoutPlus;
        error_Vout += adcVoutMinus;
    }

    long error = MIN(error_Iout, error_Vout);

    i_MV += error*A;

    if(i_MV<0) i_MV = 0;
    if((uint32_t)i_MV > MAX_PID_MV_PRECISION) {
        i_MV = MAX_PID_MV_PRECISION;
    }

    SMPS_PID::setPID_MV(i_MV>>PID_MV_PRECISION);
}

namespace {
    void enableBuckTransistor() {
        outputPWM::disablePWM(SMPS_VALUE_BUCK_PIN);
        IO::digitalWrite(SMPS_VALUE_BUCK_PIN, 1);
    }
    void disableBuckTransistor() {
        outputPWM::disablePWM(SMPS_VALUE_BUCK_PIN);
        IO::digitalWrite(SMPS_VALUE_BUCK_PIN, 0);
    }
    void disableBoostTransistor() {
        outputPWM::disablePWM(SMPS_VALUE_BOOST_PIN);
        IO::digitalWrite(SMPS_VALUE_BOOST_PIN, 0);
    }
}


void SMPS_PID::initialize()
{
    powerOff();
}

void SMPS_PID::powerOff()
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        i_enable = false;
        disableBuckTransistor();
        disableBoostTransistor();
    }
    //turn off SMPS transistor
    IO::digitalWrite(SMPS_DISABLE_PIN, true);
}

void SMPS_PID::powerOn()
{
    uint16_t Vin = AnalogInputs::getRealValue(AnalogInputs::Vin);
    uint16_t Vout = AnalogInputs::getRealValue(AnalogInputs::Vout_plus_pin);

    //turn on SMPS transistor
    IO::digitalWrite(SMPS_DISABLE_PIN, false);

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        i_setpoint_adcIout = 0;
        i_setpoint_adcVout = 0;
        if(Vout>Vin) {
            i_MV = OUTPUT_PWM_PRECISION_PERIOD;
        } else {
            i_MV = 0;
        }
        i_MV <<= PID_MV_PRECISION;
        i_enable = true;
    }
}

void SMPS_PID::setPID_MV(uint16_t value) {
    if(value > MAX_PID_MV)
        value = MAX_PID_MV;

    if(value <= OUTPUT_PWM_PRECISION_PERIOD) {
        disableBoostTransistor();
        outputPWM::setPWM(SMPS_VALUE_BUCK_PIN, value);
    } else {
        enableBuckTransistor();
        uint16_t v = value - OUTPUT_PWM_PRECISION_PERIOD;
        outputPWM::setPWM(SMPS_VALUE_BOOST_PIN, v);
    }
}

void hardware::setVoutCutoff(AnalogInputs::ValueType v) {
    if(v > MAX_CHARGE_V) {
        v = MAX_CHARGE_V;
    }
    AnalogInputs::ValueType cutOff = AnalogInputs::reverseCalibrateValue(AnalogInputs::Vout_plus_pin, v);
    if(cutOff > ANALOG_INPUTS_MAX_ADC_Vout_plus_pin) {
        //extra limit if calibration is wrong
        cutOff = ANALOG_INPUTS_MAX_ADC_Vout_plus_pin;
    }

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        SMPS_PID::i_cutOffVoltage = cutOff;
    }
}

void SMPS_PID::setIoutPWM(uint16_t adcIout)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        i_setpoint_adcIout = adcIout;
    }

//  TODO: test without PID
//  SMPS_PID::setPID_MV(value);
//  outputPWM::setPWM(SMPS_VALUE_BUCK_PIN, value);
}

void SMPS_PID::setVoutPWM(uint16_t adcVout)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        i_setpoint_adcVout = adcVout;
    }
}


void hardware::setDischargerOutput(bool enable)
{
    IO::digitalWrite(DISCHARGE_DISABLE_PIN, !enable);
}

void hardware::setDischargerValue(uint16_t value)
{
    outputPWM::setPWM(DISCHARGE_VALUE_PIN, value);
}

