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
//#define ENABLE_DEBUG
#include "debug.h"

#define BUTTON_DELAY                 1
#define BUTTON_DEBOUNCE_COUNT        4


namespace Keyboard {
    static const uint8_t stateDelay[]   PROGMEM = {BUTTON_DEBOUNCE_COUNT, 250, 125, 60,   30,  10,   1};
    static const uint8_t stayInState[]  PROGMEM = {                    1,   1,   3,  8,   20,  30,   1};
    static const uint8_t speedFactor[]  PROGMEM = {                    1,   1,   1, 11,   11,  10, 100};

    uint8_t last_key_ = BUTTON_NONE;
    uint8_t inState_ = 0;

    //state_ - "key pressed" state
    //state_ == 0 - Denounce state
    //state_ == 1 - new key pressed (or we are in key == BUTTON_NONE)
    //state_ == n - key is pressed and hold
    uint8_t state_ = 0;

    bool isLongPressTime() {
        return state_ > 2;
    }

    uint8_t getLast() {
        return last_key_;
    }

    uint16_t getSpeedFactor() {
        uint8_t v;
        pgm::read(v, &speedFactor[state_]);
        return v;
    }
}

uint8_t Keyboard::getPressedWithDelay()
{
    uint8_t key, delay = 0;
    uint8_t stateDelayVal;
    do {
        delay++;
        Time::delayDoIdle(BUTTON_DELAY);
        key = hardware::getKeyPressed();
        if(last_key_ != key) {
            //key changed
            last_key_ = key;
            state_ = 0;
            inState_ = 0;
            delay = 0;
        }
        pgm::read(stateDelayVal, &stateDelay[state_]);
    } while (delay <= stateDelayVal);

    if(state_ < sizeOfArray(stateDelay) - 1) {
        inState_++;
        if(inState_ >= stateDelayVal) {
            state_ ++;
            inState_ = 0;
            if(last_key_ == BUTTON_NONE) {
                state_ = 1; //we stay at state := 1 while last_key_ == BUTTON_NONE
            } else if(state_ == 1) { //key pressed (changed)
                //this method has also a small delay
                Buzzer::soundKeyboard();
            }
        }
    }

    return last_key_;
}

