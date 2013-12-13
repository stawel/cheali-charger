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

#include "AnalogInputs.h"

const AnalogInputs::DefaultValues AnalogInputs::inputsP_[AnalogInputs::PHYSICAL_INPUTS] PROGMEM = {
    {{0, 0},                        {55040, 25144}},    //Vout
    {{0, 0},                        {27516, 12552}},    //VreversePolarity
    {{3408, 50},                        {28770, 5000}},    //Ismps
    {{3072, 50},                        {8240, 1000}},    //Idischarge

    {{0, 0},                        {44736, 23118}},    //VoutMux
    {{54784, 1800},                        {26511, 3500}},    //Tintern
    {{0, 0},                        {41491, 12158}},    //Vin
    {{280, 2292},                        {14300, 6000}},    //Textern

    {{0, 0},                        {0, 0}},    //Vb0_pin
    {{0, 0},                        {54848, 4170}},    //Vb1_pin
    {{0, 0},                      {55488, 4198}},  //Vb2_pin
    {{0, 0},                      {55168, 4193}},  //Vb3_pin

    {{0, 0},                      {55488, 4186}},  //Vb4_pin
    {{0, 0},                      {55758, 4196}},  //Vb5_pin
    {{0, 0},                      {55104, 4208}},  //Vb6_pin

    //1-1 correlation
    {{252, 50},                      {2070, 5000}},  //IsmpsValue
    {{246, 50},                      {616, 300}},  //IdischargeValue
#ifdef ANALOG_INPUTS_V_UNKNOWN
    {{0, 0},                         {1, 1}},           //UNKNOWN0
    {{0, 0},                         {1, 1}},           //UNKNOWN1
#endif
};


