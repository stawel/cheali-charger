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
#include "Utils.h"

#define BUTTON_DELAY                 1
#define BUTTON_DEBOUNCE_COUNT       10


namespace Keyboard {
    static const uint8_t stateDelay[]   PROGMEM = {1, 250, 125, 60,   30,  10, 1};
    static const uint8_t stayInState[]  PROGMEM = {1,   1,   3,  8,   20,  30, 1};

    uint8_t last_key_ = BUTTON_NONE;
    uint8_t last_key_count_ = 0;
    uint8_t state_ = 0;
    uint8_t delay_ = 0;

    uint8_t keyChanged(uint8_t key);

    uint8_t getSpeed()  {
        return state_;
    };

    bool isLongPressTime() {
        return state_ > 2;
    }

    uint8_t getLast() {
        return last_key_;
    }
}

uint8_t Keyboard::getPressedWithDelay()
{
    uint8_t key, i = 0;

    do {
        Time::delayDoIdle(BUTTON_DELAY);
        key = hardware::getKeyPressed();
        if(last_key_count_ == 0) {
            last_key_ = key;
            state_ = 0;
            delay_ = 0;
        }
        last_key_count_ += (last_key_ == key)? 1: -1;

        if(last_key_count_ > BUTTON_DEBOUNCE_COUNT) {
            // button pressed
            last_key_count_ = BUTTON_DEBOUNCE_COUNT;
            i++;
        } else {
            //button changed
            i = 0;
        }
    } while (i <= pgm::read(&stateDelay[state_]));

    if(state_ < sizeOfArray(stateDelay) - 1) {
        delay_++;
        if(delay_ >= pgm::read(&stayInState[state_])) {
            if(last_key_ == BUTTON_NONE) {
                state_ = 0; //we stay at state == 1 while last_key_ == BUTTON_NONE
            } else if(state_ == 0) {
                    Buzzer::soundKeyboard();
            }

            state_ ++;
            delay_ = 0;
        }
    }

    return last_key_;
}

