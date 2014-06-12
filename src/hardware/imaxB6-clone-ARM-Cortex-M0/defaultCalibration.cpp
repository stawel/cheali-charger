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
    {{0, 0},                        {1, 1}},    //Vout
    {{0, 0},                        {1, 1}},    //VreversePolarity
    {{0, 0},                        {1, 1}},    //Ismps
    {{0, 0},                        {1, 1}},    //Idischarge

    {{0, 0},                        {1, 1}},    //VoutMux
    {{0, 0},                        {1, 1}},    //Tintern
    {{0, 0},                        {1, 1}},    //Vin
    {{0, 0},                        {1, 1}},    //Textern

    {{0, 0},                        {1, 1}},    //Vb0_pin
    {{0, 0},                        {1, 1}},    //Vb1_pin
    {{0, 0},                      {1, 1}},  //Vb2_pin
    {{0, 0},                      {1, 1}},  //Vb3_pin

    {{0, 0},                      {1, 1}},  //Vb4_pin
    {{0, 0},                      {1, 1}},  //Vb5_pin
    {{0, 0},                      {1, 1}},  //Vb6_pin

#if MAX_BANANCE_CELLS > 6
    {{0, 0},                        {50752, ANALOG_VOLT(3.865)}},   //Vb7_pin
    {{0, 0},                        {51008, ANALOG_VOLT(3.885)}},   //Vb8_pin
#endif
    //1-1 correlation
    {{0, 0},                      {1, 1}},  //IsmpsValue
    {{0, 0},                      {1, 1}},  //IdischargeValue
};


