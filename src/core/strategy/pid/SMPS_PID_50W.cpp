#include "PID.h"
#include "Hardware.h"
#include "SMPS_PID.h"
#include "IO.h"
#include "AnalogInputs.h"
#include "outputPWM.h"
#include "atomic.h"

#define MIN( a, b ) ( ((a) < (b)) ? (a) : (b) )

namespace SMPS_PID {
    PID pidVoltage;
    PID pidCurrent;
    //prefix "i" - variable is used in interrupts
    //we have to use i_PID_CutOffVoltage, on some chargers (M0516) ADC can read up to 60V
    volatile uint16_t i_cutOffVoltage;
    volatile bool i_enabled;

    void setManipulatedVariable(uint16_t value);
}

#define A 4

void SMPS_PID::initialize()
{
    pidVoltage.setK(PID_KVALUE(0), PID_KVALUE(0.016), PID_KVALUE(0));
    pidCurrent.setK(PID_KVALUE(0), PID_KVALUE(0.016), PID_KVALUE(0));
    powerOff();
}


uint16_t hardware::getPIDValue()
{
/*    uint16_t v;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        v = SMPS_PID::i_ManipulatedVariable>>PID_MV_PRECISION;
    }
    return v;*/
}


void SMPS_PID::update()
{
    if(!i_enabled) return;
    //if Vout is too high disable PID

    AnalogInputs::ValueType adcVoutPlus  = AnalogInputs::getADCValue(AnalogInputs::Vout_plus_pin);
    AnalogInputs::ValueType adcVoutMinus = AnalogInputs::getADCValue(AnalogInputs::Vout_minus_pin);

    //safety check
    if(adcVoutPlus >= i_cutOffVoltage) {
        powerOff();
        return;
    }

    AnalogInputs::ValueType adcVout = 0;
    if(adcVoutPlus > adcVoutMinus) adcVout = adcVoutPlus - adcVoutMinus;

    pidVoltage.calculateOutput(adcVout);
    pidCurrent.calculateOutput(AnalogInputs::getADCValue(AnalogInputs::Ismps));

    pidCurrent.normalizeOutput(MAX_PID_MV);
    pidVoltage.normalizeOutput(MAX_PID_MV);

    uint16_t output = pidCurrent.getOutput();
    if(pidVoltage.setpoint_) {
        //voltage is set
        if(pidCurrent.output_ > pidVoltage.output_) {
            output = pidVoltage.getOutput();
            //yet another normalization, TODO: rewrite
            pidCurrent.output_ = pidVoltage.output_;
        } else {
            //yet another normalization, TODO: rewrite
            pidVoltage.output_ = pidCurrent.output_;
        }
    }

    SMPS_PID::setManipulatedVariable(output);
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


void SMPS_PID::powerOff()
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        i_enabled = false;
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
    uint16_t output = 0;

    //turn on SMPS transistor
    IO::digitalWrite(SMPS_DISABLE_PIN, false);

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        pidVoltage.setSetpoint(0);
        pidCurrent.setSetpoint(0);
        if(Vout>Vin) {
            output = OUTPUT_PWM_PRECISION_PERIOD;
        }
        pidVoltage.initialize(output);
        pidCurrent.initialize(output);
        i_enabled = true;
    }
}

void SMPS_PID::setManipulatedVariable(uint16_t value) {
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
        pidCurrent.setSetpoint(adcIout);
    }

//  TODO: test without PID
//  SMPS_PID::setPID_MV(value);
//  outputPWM::setPWM(SMPS_VALUE_BUCK_PIN, value);
}

void SMPS_PID::setVoutPWM(uint16_t adcVout)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        pidVoltage.setSetpoint(adcVout);
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

