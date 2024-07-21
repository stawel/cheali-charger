/*
    cheali-charger - open source firmware for a variety of LiPo chargers
    Copyright (C) 2014  Paweł Stawicki. All right reserved.

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

#include "AnalogInputsPrivate.h"
#include "memory.h"
#include "Utils.h"

const AnalogInputs::DefaultValues AnalogInputs::inputsP_[] PROGMEM = {
    {{0,       0}, {49676, 22830}}, // Vout_plus_pin
    {{0,       0}, {0,         0}}, // Vout_minus_pin (dummy values)
    {{140,   100}, {2567,   1000}}, // Ismps
    {{282,   100}, {1652,    500}}, // Idischarge

    {{14,      0}, {42461, 22830}}, // VoutMux
    {{8684, 3780}, {8970,   1000}}, // Tintern (borrowed from CQ3)
    {{0,       0}, {27860, 14950}}, // Vin
    {{4701, 3660}, {0,         0}}, // Textern (borrowed from CQ3)

    {{0,       0}, {0,         0}}, // Vb0_pin (dummy values)
    {{0,       0}, {24716,  3803}}, // Vb1_pin
    {{0,       0}, {24559,  3793}}, // Vb2_pin
    {{0,       0}, {24683,  3802}}, // Vb3_pin
    {{0,       0}, {24564,  3804}}, // Vb4_pin
    {{0,       0}, {25068,  3803}}, // Vb5_pin
    {{0,       0}, {24915,  3803}}, // Vb6_pin

    {{140,   100}, {2567,   1000}}, // IsmpsSet
    {{282,   100}, {1652,    500}}, // IdischargeSet
};

STATIC_ASSERT(sizeOfArray(AnalogInputs::inputsP_) == AnalogInputs::PHYSICAL_INPUTS);
