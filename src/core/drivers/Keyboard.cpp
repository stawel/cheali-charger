/*
    cheali-charger - open source firmware for a variety of LiPo chargers
    Copyright (C) 2013  Paweł Stawicki. All right reserved.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "Hardware.h"
#include "Keyboard.h"
#include "Buzzer.h"
#include "memory.h"


namespace Keyboard {
    uint8_t last_key_ = BUTTON_NONE;
    uint8_t speed_ = 0;
    uint8_t this_speed_ = 0;

    uint8_t keyChanged(uint8_t key);

    uint8_t getSpeed()  {
        return speed_;
    };
    bool isLongPressTime() {
        return speed_ > 1;
    }
}



uint8_t Keyboard::getPressed()
{
    return hardware::getKeyPressed();
}

static const uint16_t speedTable[]  PROGMEM = {1, 2, 4, 6,  8, 10, 12};//, 4096, 16384, 65535};
static const uint16_t thisSpeedT[]  PROGMEM = {1, 3, 8, 9, 10, 11, 12};//,    7,     7,     7};
static const uint16_t speedFactor[] PROGMEM = {1, 1, 1, 1,  1,  10,  100};//,   16,    64,   256};

uint16_t Keyboard::getSpeedFactor()
{
    return pgm::read(&speedFactor[speed_]);
}

uint8_t Keyboard::keyChanged(uint8_t key)
{
    uint8_t key2;

    key2 = getPressed();
    while(key2 != key) {
        Time::delayDoIdle(BUTTON_CHANGE_DELAY);
        key = key2;
        key2 = getPressed();
    }

    speed_ = 0;
    this_speed_ = 0;
    last_key_ = key;

    if(key != BUTTON_NONE) {
        Buzzer::soundKeyboard();
    }

    return last_key_;
}

uint8_t Keyboard::getPressedWithSpeed()
{
    uint8_t key = BUTTON_NONE;
    for(uint16_t i = 0; i < (BUTTON_DELAY_TIMES / pgm::read(&speedTable[speed_])) + 1; i++) {
        key = getPressed();
        Time::delayDoIdle(BUTTON_DELAY);
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


