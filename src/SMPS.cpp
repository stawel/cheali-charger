#include "TimerOne.h"
#include "Hardware.h"
#include "SMPS.h"

SMPS::SMPS()
{
    pinMode(SMPS_VALUE_PIN, OUTPUT);
    pinMode(SMPS_DISABLE_PIN, OUTPUT);

    setValue(0);
    powerOff(CHARGING_COMPLETE);
}


void SMPS::setValue(uint16_t value)
{
    if(value > SMPS_UPPERBOUND_VALUE)
        value = SMPS_UPPERBOUND_VALUE;
    value_ = value;
    Timer1.pwm(SMPS_VALUE_PIN, value_);
    analogInputs.reset();
}
void SMPS::setRealValue(uint16_t I)
{
    uint16_t value = analogInputs.reverseCalibrateValue(AnalogInputs::IsmpsValue, I);
    setValue(value);
}

void SMPS::powerOn()
{
    if(isPowerOn())
        return;

    hardware::setBatteryOutput(true);
    setValue(0);
    digitalWrite(SMPS_DISABLE_PIN, false);
    analogInputs.doFullMeasurement();
    state_ = CHARGING;
    charge_ = 0;
}

#include "GTPowerA610.h"


void SMPS::powerOff(STATE reason)
{
    if(!isPowerOn() || reason == CHARGING)
        return;

    setValue(0);
    digitalWrite(SMPS_DISABLE_PIN, true);
    hardware::setBatteryOutput(false);
    state_ = reason;
}

void SMPS::doSlowInterrupt()
{
   if(isPowerOn()) {
           charge_ += getIcharge();
   }
}

uint16_t SMPS::getCharge() const
{
    uint32_t retu = charge_;
#if INTERRUPT_PERIOD_MICROSECONDS == 512
//    retu *= INTERRUPT_PERIOD_MICROSECONDS;
    retu /= (1000000/32);//*(3600/16) == TIMER_SLOW_INTERRUPT_INTERVAL
#else
#warning "INTERRUPT_PERIOD_MICROSECONDS != 512"
    retu *= INTERRUPT_PERIOD_MICROSECONDS;
    retu /= 1000000;
    retu /= 3600/TIMER_SLOW_INTERRUPT_INTERVAL;
#endif
    return retu;
}

AnalogInputs::ValueType SMPS::getVout() const
{
    return analogInputs.getRealValue(VName);
}

AnalogInputs::ValueType SMPS::getIcharge() const
{
    return analogInputs.getRealValue(IName);
}


