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
#include "AnalogInputs.h"

const AnalogInputs::DefaultValues AnalogInputs::inputsP_[AnalogInputs::PHYSICAL_INPUTS] PROGMEM = {
    {{0, 0},                        {50118, 22849}},    //Vout
    {{0, 0},                        {27516, 12552}},    //VreversePolarity
    {{371, 200},                        {5376, 2000}},    //Ismps
    {{960, 200},                        {5925, 1000}},    //Idischarge

    {{0, 0},                        {44736, 23118}},    //VoutMux
    {{21500, 5200},                        {41023, 2900}},    //Tintern
    {{0, 0},                        {61639, 17961}},    //Vin
    {{6272, 2400},                        {14300, 6000}},    //Textern

    {{0, 0},                        {0, 0}},    //Vb0_pin
    {{0, 0},                        {50185, 3803}},    //Vb1_pin
    {{0, 0},                      {49969, 3808}},  //Vb2_pin
    {{0, 0},                      {49791, 3811}},  //Vb3_pin

    {{0, 0},                      {49893, 3803}},  //Vb4_pin
    {{0, 0},                      {50037, 3814}},  //Vb5_pin
    {{0, 0},                      {50425, 3813}},  //Vb6_pin

    //1-1 correlation
    {{17, 200},                      {310, 2000}},  //IsmpsValue
    {{71, 200},                      {436, 1000}},  //IdischargeValue
#ifdef ANALOG_INPUTS_V_UNKNOWN
    {{0, 0},                         {1, 1}},           //UNKNOWN0
    {{0, 0},                         {1, 1}},           //UNKNOWN1
#endif
};


