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
#ifndef MULTIPLEXER_H_
#define MULTIPLEXER_H_

#include "AnalogInputs.h"

#define MUXINPUT0     0
#define MUXINPUT1     8


template<uint8_t MUX_ADR0_PIN, uint8_t MUX_ADR1_PIN, uint8_t MUX_ADR2_PIN,
          uint8_t MUX0_Z_D_PIN, uint8_t MUX1_Z_D_PIN,
          uint8_t MUX0_Z_A_PIN, uint8_t MUX1_Z_A_PIN>
class Multiplexer {
public:
    static inline void setAddress(uint8_t address) {
        digitalWrite(MUX_ADR0_PIN, address&1);
        digitalWrite(MUX_ADR1_PIN, address&2);
        digitalWrite(MUX_ADR2_PIN, address&4);
    }

    static uint8_t digitalRead(uint8_t address) {
        setAddress(address);
        uint8_t retu  = ::digitalRead(address&MUXINPUT1 ? MUX1_Z_D_PIN : MUX0_Z_D_PIN);
        return retu;
    }

    static uint16_t analogRead(uint8_t address){
        setAddress(address);
        if(address&MUXINPUT1)
            return AnalogInputs::analogValue<MUX1_Z_A_PIN>();
        else
            return AnalogInputs::analogValue<MUX0_Z_A_PIN>();
    }


    template<uint8_t address>
    static uint16_t analogRead() {
        return analogRead(address);
    }
    template<uint8_t address>
    static uint8_t digitalRead() {
        return digitalRead(address);
    }
};



#endif /* MULTIPLEXER_H_ */
