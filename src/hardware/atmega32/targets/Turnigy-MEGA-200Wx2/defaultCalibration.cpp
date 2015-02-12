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

    {{0,  0},         {50444,  23122}},   //Vout_plus_pin
    {{0,  0},         {50444,  23122}},   //Vout_minus_pin
    {{246,  100},         {4624,  1000}},   //Ismps
    {{512,  100},         {2240,  300}},   //Idischarge

    {{0,  0},         {44736,  23118}},   //VoutMux
    {{54784,  1800},         {26511,  3500}},   //Tintern
    {{0,  0},         {41152,  12000}},   //Vin
    {{280,  2292},         {14300,  6000}},   //Textern

    {{0,  0},         {0,  0}},   //Vb0_pin
    {{0,  0},         {50566,  3851}},   //Vb1_pin
    {{0,  0},         {50591,  3854}},   //Vb2_pin
    {{0,  0},         {50757,  3848}},   //Vb3_pin
    {{0,  0},         {50230,  3852}},   //Vb4_pin
    {{0,  0},         {51359,  3851}},   //Vb5_pin
    {{0,  0},         {49655,  3861}},   //Vb6_pin


    {{74,  100},         {2254,  1000}},   //IsmpsSet
    {{293,  100},         {1298,  300}},   //IdischargeSet
};

namespace {
    void assert() {
        STATIC_ASSERT(sizeOfArray(AnalogInputs::inputsP_) == AnalogInputs::PHYSICAL_INPUTS);
    }
}
