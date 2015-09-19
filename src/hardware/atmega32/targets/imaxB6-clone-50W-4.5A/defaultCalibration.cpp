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

#include "AnalogInputsPrivate.h"
#include "memory.h"

const AnalogInputs::DefaultValues AnalogInputs::inputsP_[AnalogInputs::PHYSICAL_INPUTS] PROGMEM = {
    {{0, 0},                        {54592, 25094}},    //Vout
    {{0, 0},                        {26613, 12518}},    //VreversePolarity
    {{378, 50},                        {10916, 1000}},    //Ismps
    {{7095, 50},                        {43070, 300}},    //Idischarge

    {{0, 0},                        {0, 0}},    //VoutMux
    {{0, 0},                        {0, 0}},    //Tintern
    {{0, 0},                        {48013, 14038}},    //Vin
    {{19650, 2980},                        {18700, 6000}},    //Textern

    {{0, 0},                        {48963, 3752}},    //Vb0_pin
    {{0, 0},                        {54805, 4177}},    //Vb1_pin
    {{0, 0},                      {55707, 8392}},  //Vb2_pin
    {{0, 0},                      {55498, 4195}},  //Vb3_pin

    {{0, 0},                      {55303, 4183}},  //Vb4_pin
    {{0, 0},                      {55360, 4191}},  //Vb5_pin
    {{0, 0},                      {53157, 4184}},  //Vb6_pin

#if MAX_BANANCE_CELLS > 6
    {{0, 0},                        {50752, ANALOG_VOLT(3.865)}},   //Vb7_pin
    {{0, 0},                        {51008, ANALOG_VOLT(3.885)}},   //Vb8_pin
#endif
    //1-1 correlation
    {{380, 50},                      {10920, 1000}},  //IsmpsSet
    {{2610, 50},                      {15850, 300}},  //IdischargeSet
};


