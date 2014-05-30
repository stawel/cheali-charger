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
    {{0,0},                         {50816, ANALOG_VOLT(23.118)}},  //Vout
    {{0,0},                         {27516, ANALOG_VOLT(12.552)}},  //VreversePolarity
    {{256,   ANALOG_AMP(0.051)},    {10240, ANALOG_AMP(2.000)}},    //Ismps
    {{384,   ANALOG_AMP(0.051)},    { 9024, ANALOG_AMP(1.000)}},    //Idischarge

    {{0, 0},                        {44736, ANALOG_VOLT(23.118)}},  //VoutMux
    {{21500, ANALOG_CELCIUS(52)},   {41023, ANALOG_CELCIUS(29)}},   //Tintern
    {{0, 0},                        {47872, ANALOG_VOLT(14.020)}},  //Vin
    {{6272,  ANALOG_CELCIUS(24)},   {14300, ANALOG_CELCIUS(60)}},   //Textern

    {{0, 0},                        {0, ANALOG_VOLT(0)}},           //Vb0_pin -??
    {{0, 0},                        {50244, ANALOG_VOLT(3.834)}},   //Vb1_pin
    {{0, 0},                        {50176, ANALOG_VOLT(3.835)}},   //Vb2_pin
    {{0, 0},                        {50280, ANALOG_VOLT(3.837)}},   //Vb3_pin

    {{0, 0},                        {50602, ANALOG_VOLT(3.862)}},   //Vb4_pin
    {{0, 0},                        {50752, ANALOG_VOLT(3.865)}},   //Vb5_pin
    {{0, 0},                        {51008, ANALOG_VOLT(3.885)}},   //Vb6_pin
#if MAX_BANANCE_CELLS > 6
    {{0, 0},                        {50752, ANALOG_VOLT(3.865)}},   //Vb7_pin
    {{0, 0},                        {51008, ANALOG_VOLT(3.885)}},   //Vb8_pin
#endif
    {{176, ANALOG_AMP(0.050)},       {176*2, ANALOG_AMP(1.000)}},      //IsmpsValue
    {{335, ANALOG_AMP(0.050)},       {335*2, ANALOG_AMP(0.300)}},      //IdischargeValue
};


