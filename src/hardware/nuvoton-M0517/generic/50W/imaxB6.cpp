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
#include "imaxB6.h"
#include "imaxB6-pins.h"
#include "SMPS_PID.h"
#include "AnalogInputsADC.h"
#include "SerialLog.h"
#include "IO.h"
#include "Keyboard.h"
#include "outputPWM.h"


uint8_t hardware::getKeyPressed()
{
    return   (IO::digitalRead(BUTTON_STOP_PIN) ? 0 : BUTTON_STOP)
            | (IO::digitalRead(BUTTON_DEC_PIN)  ? 0 : BUTTON_DEC)
            | (IO::digitalRead(BUTTON_INC_PIN)  ? 0 : BUTTON_INC)
            | (IO::digitalRead(BUTTON_START_PIN)? 0 : BUTTON_START);
}


void hardware::setBalancerOutput(bool enable)
{
}

void hardware::initializePins()
{
    setBalancer(0);
    setBatteryOutput(false);
    setBuzzer(0);

    IO::pinMode(BALANCER1_LOAD_PIN, OUTPUT);
    IO::pinMode(BALANCER2_LOAD_PIN, OUTPUT);
    IO::pinMode(BALANCER3_LOAD_PIN, OUTPUT);
    IO::pinMode(BALANCER4_LOAD_PIN, OUTPUT);
    IO::pinMode(BALANCER5_LOAD_PIN, OUTPUT);
    IO::pinMode(BALANCER6_LOAD_PIN, OUTPUT);

    IO::pinMode(BUTTON_STOP_PIN, INPUT);
    IO::pinMode(BUTTON_DEC_PIN, INPUT);
    IO::pinMode(BUTTON_INC_PIN, INPUT);
    IO::pinMode(BUTTON_START_PIN, INPUT);
    IO::pinMode(OUTPUT_DISABLE_PIN, OUTPUT);
    IO::pinMode(BUZZER_PIN, OUTPUT);

    IO::pinMode(DISCHARGE_VALUE_PIN, OUTPUT);
    IO::pinMode(DISCHARGE_DISABLE_PIN, OUTPUT);


    IO::pinMode(SMPS_VALUE_BUCK_PIN, OUTPUT);
    IO::pinMode(SMPS_VALUE_BOOST_PIN, OUTPUT);
    IO::pinMode(SMPS_DISABLE_PIN, OUTPUT);

}


void hardware::initialize()
{
    lcd.begin(LCD_COLUMNS, LCD_LINES);
    AnalogInputsADC::initialize();
    outputPWM::initialize();
    setVoutCutoff(MAX_CHARGE_V);
}


void hardware::soundInterrupt()
{}

void hardware::setBuzzer(uint8_t val)
{
    IO::digitalWrite(BUZZER_PIN, (val&1));
}

void hardware::setBatteryOutput(bool enable)
{
    IO::digitalWrite(OUTPUT_DISABLE_PIN, !enable);
    if(!enable) {
        setChargerOutput(false);
        setDischargerOutput(false);
    }
}

void hardware::setBalancer(uint8_t v)
{
    IO::digitalWrite(BALANCER1_LOAD_PIN, v&1);
    IO::digitalWrite(BALANCER2_LOAD_PIN, v&2);
    IO::digitalWrite(BALANCER3_LOAD_PIN, v&4);
    IO::digitalWrite(BALANCER4_LOAD_PIN, v&8);
    IO::digitalWrite(BALANCER5_LOAD_PIN, v&16);
    IO::digitalWrite(BALANCER6_LOAD_PIN, v&32);
}

LiquidCrystal lcd;

void hardware::setExternalTemperatueOutput(bool enable)
{
    if(enable) {
        IO::pinMode(T_EXTERNAL_PIN, ANALOG_INPUT);
    }
}

