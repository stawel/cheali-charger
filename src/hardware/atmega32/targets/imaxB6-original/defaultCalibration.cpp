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
    {{0, 0},                        {55102, 25141}},    //Vout
    {{0, 0},                        {26613, 12518}},    //VreversePolarity
    {{775, 50},                        {11080, 1000}},    //Ismps
    {{2017, 50},                        {11971, 300}},    //Idischarge

    {{0, 0},                        {0, 0}},    //VoutMux
    {{0, 0},                        {0, 0}},    //Tintern
    {{0, 0},                        {48061, 14047}},    //Vin
    {{19650, 2980},                        {18700, 6000}},    //Textern

    {{0, 0},                        {48963, 3752}},    //Vb0_pin
    {{0, 0},                        {55048, 4187}},    //Vb1_pin
    {{0, 0},                      {56248, 8378}},  //Vb2_pin
    {{0, 0},                      {56067, 4191}},  //Vb3_pin

    {{0, 0},                      {54784, 4193}},  //Vb4_pin
    {{0, 0},                      {55070, 4196}},  //Vb5_pin
    {{0, 0},                      {55808, 4193}},  //Vb6_pin

#if MAX_BANANCE_CELLS > 6
    {{0, 0},                        {50752, ANALOG_VOLT(3.865)}},   //Vb7_pin
    {{0, 0},                        {51008, ANALOG_VOLT(3.885)}},   //Vb8_pin
#endif
    //1-1 correlation
    {{780, 50},                      {11080, 1000}},  //IsmpsValue
    {{730, 50},                      {4410, 300}},  //IdischargeValue
};


