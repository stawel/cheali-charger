#include <Arduino.h>
#include "Hardware.h"
#include "Keyboard.h"
#include "Buzzer.h"
#include "memory.h"

Keyboard::Keyboard(): last_key_(BUTTON_NONE), speed_(0), this_speed_(0)
{}


uint8_t Keyboard::getPressed() const
{
    return hardware::getKeyPressed();
}

static const uint16_t speedTable[]  PROGMEM = {1, 2, 4, 16, 64, 256, 1024};//, 4096, 16384, 65535};
static const uint16_t thisSpeedT[]  PROGMEM = {1, 3, 8,  7,  7,   7,    7};//,    7,     7,     7};
static const uint16_t speedFactor[] PROGMEM = {1, 1, 1,  1,  1,   1,    4};//,   16,    64,   256};

uint16_t Keyboard::getSpeedFactor() const
{
    return pgm::read(&speedFactor[speed_]);
}

uint8_t Keyboard::keyChanged(uint8_t key)
{
    uint8_t key2;

    key2 = getPressed();
    while(key2 != key) {
        hardware::delay(BUTTON_CHANGE_DELAY);
        key = key2;
        key2 = getPressed();
    }

    speed_ = 0;
    this_speed_ = 0;
    last_key_ = key;

    if(key != BUTTON_NONE)
        buzzer.soundKeyboard();

    return last_key_;
}

uint8_t Keyboard::getPressedWithSpeed()
{
    uint8_t key = BUTTON_NONE;
    for(uint16_t i = 0; i < (BUTTON_DELAY_TIMES / pgm::read(&speedTable[speed_])) + 1; i++) {
        key = getPressed();
        hardware::delay(BUTTON_DELAY);
        if(key == last_key_)
            continue;

        return keyChanged(key);
    }

    if(key != BUTTON_NONE) {
        this_speed_++;
        if(this_speed_ >= pgm::read(&thisSpeedT[speed_])) {
            if(speed_ < sizeof(speedTable) / sizeof(speedTable[0]) - 1)
                speed_ ++;
            this_speed_ = 0;
        }
    }

    return last_key_;
}

uint8_t Keyboard::getPressTime(uint8_t limit)
{
    uint8_t last = last_key_;
    uint8_t key;
    do {
        key = getPressedWithSpeed();
        if(speed_ == limit)
            return speed_;
    } while(key == last);
    return speed_;
}


