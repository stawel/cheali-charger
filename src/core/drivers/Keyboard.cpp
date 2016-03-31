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

//delay until next key read, must not be smaller than 7ms (see: atmeag32/generic/200W/AnalogInputsADC.cpp:adc_keyboard_)
#define BUTTON_DELAY                 7
#define BUTTON_DEBOUNCE_COUNT        3


namespace Keyboard {
    static const PROGMEM uint8_t stateDelay[]   = {   25,    12,     3,     1,     1,     1};
    static const PROGMEM uint8_t stayInState[]  = {    1,     3,    24,    71,   142,     1};
    static const PROGMEM uint8_t speedFactor[]  = {    1,     1,     1,     2,    10,    30};
           //timing (assuming screen is not redrawn)
           //change per:                           175ms,  84ms,  21ms,   7ms,   7ms,   7ms
           //inState:                              175ms, 252ms, 504ms, 497ms, 994ms, for ever
           //changes/second (with speed factor):     5.7,  11.9,  47.6, 285.7,  1428,  4285

    uint8_t last_key_ = BUTTON_NONE;
    uint8_t debounce_ = 0;

    uint8_t inState_ = 0;

    //state_ - "key pressed" state
    //state_ == 0 - new key pressed (or we are in key == BUTTON_NONE)
    //state_ == n - key is pressed and hold
    uint8_t state_ = 0;

    bool isLongPressTime() {
        return state_ > 2;
    }

    uint8_t getLast() {
        return last_key_;
    }

    uint8_t getSpeedFactor() {
        uint8_t v;
        pgm_read(v, &speedFactor[state_]);
        return v;
    }
}

uint8_t Keyboard::getPressedWithDelay()
{
    uint8_t key, delay = 0, currentStateDelay;

    pgm_read(currentStateDelay, &stateDelay[state_]);

    do {
        Time::delayDoIdle(BUTTON_DELAY);
        key = hardware::getKeyPressed();
        if(last_key_ != key) {
            if(debounce_ == 0) {
                //key changed
                last_key_ = key;
                state_ = 0;
                inState_ = 0;
                if(key != BUTTON_NONE) {
                    Buzzer::soundKeyboard();
                }
                return key;
            }
            debounce_--;
        } else {
            debounce_++;
        }
        if(debounce_ > BUTTON_DEBOUNCE_COUNT) {
            debounce_ = BUTTON_DEBOUNCE_COUNT;
            delay++;
        }
    } while (delay <= currentStateDelay);

    //change state if necessary
    if(state_ < sizeOfArray(stateDelay) - 1 && key != BUTTON_NONE) {
        uint8_t InStateLimit;
        inState_++;
        pgm_read(InStateLimit, &stayInState[state_]);
        if(inState_ >= InStateLimit) {
            state_ ++;
            inState_ = 0;
        }
    }

    return key;
}

