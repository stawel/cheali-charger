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
#include <Arduino.h>
#include "imaxB6.h"
#include "TimerOne.h"
#include "imaxB6-pins.h"
#include "SMPS_PID.h"
#include "adc.h"
#include "SerialLog.h"
#include "Settings.h"


uint8_t hardware::getKeyPressed()
{
    return   (digitalRead(BUTTON_STOP_PIN) ? 0 : BUTTON_STOP)
            | (digitalRead(BUTTON_DEC_PIN)  ? 0 : BUTTON_DEC)
            | (digitalRead(BUTTON_INC_PIN)  ? 0 : BUTTON_INC)
            | (digitalRead(BUTTON_START_PIN)? 0 : BUTTON_START);
}


void hardware::setBalancerOutput(bool enable)
{
    uint8_t mode = INPUT;
    if(enable)
        mode = OUTPUT;
    pinMode(BALANCER1_LOAD_PIN, mode);
    pinMode(BALANCER2_LOAD_PIN, mode);
    pinMode(BALANCER3_LOAD_PIN, mode);
    pinMode(BALANCER4_LOAD_PIN, mode);
    pinMode(BALANCER5_LOAD_PIN, mode);
    pinMode(BALANCER6_LOAD_PIN, mode);

}


void hardware::initialize()
{
    analogReference(EXTERNAL);
    pinMode(OUTPUT_DISABLE_PIN, OUTPUT);

    pinMode(DISCHARGE_VALUE_PIN, OUTPUT);
    pinMode(DISCHARGE_DISABLE_PIN, OUTPUT);

    pinMode(BUZZER_PIN, OUTPUT);

    pinMode(SMPS_VALUE_BUCK_PIN, OUTPUT);
    pinMode(SMPS_VALUE_BOOST_PIN, OUTPUT);
    pinMode(SMPS_DISABLE_PIN, OUTPUT);

    setBatteryOutput(false);
    setBuzzer(0);

    lcd.begin(LCD_COLUMNS, LCD_LINES);

    TimerOne::initialize();
    adc::initialize();
}


namespace {
    volatile uint8_t sound_ = 0;
}
void hardware::soundInterrupt()
{
    static uint8_t on = 0;

    uint8_t f = 0;
    if(sound_ > 0) {
        on++;
    } else {
        on = 0;
    }
    if(sound_ >= 10) f=8;
    if(sound_ >= 20) f=4;
    if(sound_ >= 30) f=2;

    digitalWrite(BUZZER_PIN, on&f);
}

void hardware::setBuzzer(uint8_t val)
{
    sound_ = val;
}

void hardware::setBatteryOutput(bool enable)
{
    digitalWrite(OUTPUT_DISABLE_PIN, !enable);
}

void hardware::setBalancer(uint8_t v)
{
    digitalWrite(BALANCER1_LOAD_PIN, v&1);
    digitalWrite(BALANCER2_LOAD_PIN, v&2);
    digitalWrite(BALANCER3_LOAD_PIN, v&4);
    digitalWrite(BALANCER4_LOAD_PIN, v&8);
    digitalWrite(BALANCER5_LOAD_PIN, v&16);
    digitalWrite(BALANCER6_LOAD_PIN, v&32);
}

LiquidCrystal lcd;

