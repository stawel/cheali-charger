//Switched-mode power supply

#ifndef SMPS_H_
#define SMPS_H_

#include "AnalogInputs.h"
#include "Timer.h"

#define SMPS_MAX_VALUE             ((F_CPU / 2000000) * INTERRUPT_PERIOD_MICROSECONDS)
#define SMPS_UPPERBOUND_VALUE     (SMPS_MAX_VALUE/4)

class Strategy;

class SMPS {
public:
    enum STATE { CHARGING, CHARGING_COMPLETE, ERROR, STOP};
    static const AnalogInputs::Name VName = AnalogInputs::VoutBalancer;
    static const AnalogInputs::Name IName = AnalogInputs::Ismps;

    SMPS();

    AnalogInputs::ValueType getVout() const;
    AnalogInputs::ValueType getIcharge() const;

    STATE getState() const { return state_; }
    bool isPowerOn() const { return getState() == CHARGING; }
    bool isWorking() const { return value_ != 0; }


    uint16_t getValue() const { return value_; }
    void setValue(uint16_t value);
    void setRealValue(uint16_t I);

    void powerOn();
    void powerOff(STATE reason = CHARGING_COMPLETE);

    void doSlowInterrupt();

    uint16_t getCharge() const;

protected:

    STATE state_;
    uint16_t value_;
    uint32_t charge_;
};



#endif /* SMPS_H_ */
