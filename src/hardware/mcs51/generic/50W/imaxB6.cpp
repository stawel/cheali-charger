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
#include "Timer1.h"
#include "imaxB6-pins.h"
#include "SMPS_PID.h"
#include "AnalogInputsADC.h"
#include "SerialLog.h"
#include "IO.h"
#include "Keyboard.h"


uint8_t hardware::getKeyPressed()
{
}


void hardware::setBalancerOutput(bool enable)
{

}


void hardware::initializePins()
{
}

void hardware::initialize()
{
}


namespace {
    volatile uint8_t sound_ = 0;
}
void hardware::soundInterrupt()
{
}

void hardware::setBuzzer(uint8_t val)
{
}

void hardware::setBatteryOutput(bool enable)
{
}

void hardware::setBalancer(uint8_t v)
{
}


