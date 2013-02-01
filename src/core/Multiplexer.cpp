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
#include <avr/pgmspace.h>
#include "Multiplexer.h"
#include "Hardware.h"
#include "AnalogInputs.h"


inline void setAddress(uint8_t address)
{
    digitalWrite(MUX_ADR0_PIN, address&1);
    digitalWrite(MUX_ADR1_PIN, address&2);
    digitalWrite(MUX_ADR2_PIN, address&4);
}

void Multiplexer::init()
{
    pinMode(MUX_ADR0_PIN, OUTPUT);
    pinMode(MUX_ADR1_PIN, OUTPUT);
    pinMode(MUX_ADR2_PIN, OUTPUT);

    pinMode(MUX0_Z_D_PIN, INPUT);
    pinMode(MUX1_Z_D_PIN, INPUT);
}

uint8_t Multiplexer::digitalRead(uint8_t address)
{
    setAddress(address);
    uint8_t retu  = ::digitalRead(address&MUXINPUT1 ? MUX1_Z_D_PIN : MUX0_Z_D_PIN);
    return retu;
}

uint16_t Multiplexer::analogRead(uint8_t address)
{
    setAddress(address);
    if(address&MUXINPUT1)
        return AnalogInputs::analogValue<MUX1_Z_A_PIN>();
    else
        return AnalogInputs::analogValue<MUX0_Z_A_PIN>();
}
