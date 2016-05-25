/*
    cheali-charger - open source firmware for a variety of LiPo chargers
    Copyright (C) 2016  Paweł Stawicki. All right reserved.

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
#include "SMPS_PID.h"
#include "IO.h"
#include "AnalogInputs.h"
#include "outputPWM.h"
#include "atomic.h"
#include "Monitor.h"


uint16_t hardware::getPIDValue()
{
    return 0;
}


void SMPS_PID::update()
{
}

void SMPS_PID::init(uint16_t Vin, uint16_t Vout)
{
}


void SMPS_PID::setPID_MV(uint16_t value)
{
}

void hardware::setVoutCutoff(AnalogInputs::ValueType v) {
}


void hardware::setChargerValue(uint16_t value)
{
}

void hardware::setChargerOutput(bool enable)
{
    //TODO:
    IO::digitalWrite(SMPS_DISABLE_PIN, true);
    IO::digitalWrite(SMPS_DISABLE2_PIN, true);
}


void hardware::setDischargerOutput(bool enable)
{
    //TODO:
    IO::digitalWrite(DISCHARGE_DISABLE_PIN, true);
}

void hardware::setDischargerValue(uint16_t value)
{
}

