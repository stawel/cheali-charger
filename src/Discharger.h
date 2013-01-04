#ifndef DISCHARGER_H
#define DISCHARGER_H

#include "AnalogInputs.h"
#include "Timer.h"

#define DISCHARGER_UPPERBOUND_VALUE ((F_CPU / 2000000) * INTERRUPT_PERIOD_MICROSECONDS)

class Discharger {
public:
    enum STATE { DISCHARGING, DISCHARGING_COMPLETE, ERROR};
    static const AnalogInputs::Name VName = AnalogInputs::VoutBalancer;
    static const AnalogInputs::Name IName = AnalogInputs::Idischarge;

    Discharger();

    static AnalogInputs::ValueType getVout();
    static AnalogInputs::ValueType getIdischarge();

    STATE getState() const { return state_; }
    bool isPowerOn() const { return getState() == DISCHARGING; }
    bool isWorking() const { return value_ != 0; }


    uint16_t getValue() const { return value_; }
    void setValue(uint16_t value);
    void setRealValue(uint16_t I);

    void powerOn();
    void powerOff(STATE reason = DISCHARGING_COMPLETE);

    void doSlowInterrupt();

    uint16_t getDischarge() const;
    uint16_t correctValueTintern(uint16_t v);

protected:
    void finalizeValueTintern(bool force);
    STATE state_;
    uint16_t value_,valueSet_;
    bool tempcutoff_;
    uint32_t discharge_;
};


#endif //DISCHARGER_H
