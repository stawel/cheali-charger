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

//info: for some reason the ADC has a 50mV shift, see https://groups.google.com/d/msg/cheali-charger/3q0wF0qCGio/oCG1BMjol0QJ
    {{0,  50},         {25540,  23735}},   //Vout_plus_pin
    {{0,  50},         {25540,  23735}},   //Vout_minus_pin
    {{417,  100},         {5062,  1000}},   //Ismps
    {{1983,  100},         {5839,  300}},   //Idischarge

    {{0,  0},         {1,  1}},   //VoutMux
    {{8000,  5940},         {8642,  3479}},   //Tintern
    {{0,  0},         {23492,  14052}},   //Vin
    {{4701,  3660},         {0,  0}},   //Textern

    {{0,  0},         {25219,  3946}},   //Vb0_pin
    {{0,  0},         {25219,  3946}},   //Vb1_pin
    {{0,  0},         {50664,  7892}},   //Vb2_pin
    {{0,  0},         {26372,  4082}},   //Vb3_pin
    {{0,  0},         {25184,  3912}},   //Vb4_pin
    {{0,  0},         {25169,  3916}},   //Vb5_pin
    {{0,  0},         {25405,  3933}},   //Vb6_pin


    {{415,  100},         {5066,  1000}},   //IsmpsSet
    {{3175,  100},         {9278,  300}},   //IdischargeSet
};

STATIC_ASSERT(sizeOfArray(AnalogInputs::inputsP_) == AnalogInputs::PHYSICAL_INPUTS);
