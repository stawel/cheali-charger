#ifndef BLINK_H
#define BLINK_H

#include "LcdPrint.h"

#define BLINK_SPEED_TIME 2

class Blink {
public:
    Blink():blinkIndex_(-1){}

    bool getBlinkOff() const;
    bool getBlinkChanged() const;

    int8_t getBlinkIndex() const    {
        if(getBlinkOff()) {
            return blinkIndex_;
        }
        return -1;
    }

    void startBlinkOn(int8_t index) {
        blinkIndex_ = index;
        blinkTime_ = getBlinkTime();
    }
    void startBlinkOff(int8_t index) {
        blinkIndex_ = index;
        blinkTime_ = 0;
    }

    void stopBlink() {    blinkIndex_ = -1; }
    void incBlinkTime() { blinkTime_++; }
    uint8_t getBlinkTime() const { return BLINK_SPEED_TIME; }

    int8_t blinkIndex_;
    uint8_t blinkTime_;
};

#endif /* BLINK_H */
