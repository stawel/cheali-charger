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

#include "AnalogInputsPrivate.h"
#include "memory.h"
#include "Utils.h"

//based on DieHertz eeprom

const AnalogInputs::DefaultValues AnalogInputs::inputsP_[] PROGMEM = {

    {{0,  0},         {23291,  16798}},   //Vout_plus_pin
    {{0,  0},         {23291,  16798}},   //Vout_minus_pin
    {{127*2,  100},         {5685,  1000}},   //Ismps
    {{383*2,  100},         {2366,  300}},   //Idischarge

    {{0,  0},         {44736,  23118}},   //VoutMux
    {{21500,  5200},         {41023,  2900}},   //Tintern
    {{0,  0},         {50438,  14790}},   //Vin
    {{6272,  2400},         {14300,  6000}},   //Textern

    {{0,  0},         {0,  0}},   //Vb0_pin
    {{0,  0},         {54707,  4200}},   //Vb1_pin
    {{0,  0},         {54902,  4200}},   //Vb2_pin
    {{0,  0},         {54779,  4200}},   //Vb3_pin
    {{0,  0},         {54919,  4200}},   //Vb4_pin
    {{0,  0},         {50752,  3865}},   //Vb5_pin
    {{0,  0},         {51008,  3885}},   //Vb6_pin

#if MAX_BANANCE_CELLS > 6
    {{0,  0},         {50752,  3865}},   //Vb7_pin
    {{0,  0},         {51008,  3885}},   //Vb8_pin
#endif

    {{71*2,  100},         {4003,  1000}},   //IsmpsSet
    {{152*2,  100},         {1330,  300}},   //IdischargeSet
};

namespace {
    void assert() {
        STATIC_ASSERT(sizeOfArray(AnalogInputs::inputsP_) == AnalogInputs::PHYSICAL_INPUTS);
    }
}
