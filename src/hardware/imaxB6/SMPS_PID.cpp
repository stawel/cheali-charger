#include "Hardware.h"
#include "TimerOne.h"
#include "imaxB6-pins.h"


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


void hardware::setChargerValue(uint16_t value)
{
    uint16_t pwmPeriodScaled = TIMERONE_PERIOD << 2;
    if(value == 0) {
        disableChargerValue0();
        disableChargerValue1();
    } else if(value < pwmPeriodScaled) {
        disableChargerValue1();
        TimerOne::setPWM(SMPS_VALUE0_PIN, value);
    } else if(value == pwmPeriodScaled) {
        disableChargerValue1();
        enableChargerValue0();
    } else {
        enableChargerValue0();
        uint16_t v2 = value - pwmPeriodScaled;
        if(v2 > pwmPeriodScaled/2)
            v2 = pwmPeriodScaled/2;
        TimerOne::setPWM(SMPS_VALUE1_PIN, v2);
    }
}

void hardware::setChargerOutput(bool enable)
{
    disableChargerValue0();
    disableChargerValue1();
    digitalWrite(SMPS_DISABLE_PIN, !enable);
}


void hardware::setDischargerOutput(bool enable)
{
    digitalWrite(DISCHARGE_DISABLE_PIN, !enable);
}

void hardware::setDischargerValue(uint16_t value)
{
    TimerOne::setPWM(DISCHARGE_VALUE_PIN, value);
}

