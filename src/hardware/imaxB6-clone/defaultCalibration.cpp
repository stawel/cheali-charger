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
    {{0,0},                         {27328, ANALOG_VOLT(12.518)}},  //Vout
    {{0,0},                         {26613, ANALOG_VOLT(12.518)}},  //VreversePolarity
    {{778,ANALOG_AMP(0.050)},       {11121, ANALOG_AMP(1.000)}},    //Ismps
    {{0,0},                         {11992, ANALOG_AMP(0.300)}},    //Idischarge

    {{0, 0},                        {0, ANALOG_VOLT(0)}},           //VoutMux
    {{0, 0},                        {0, ANALOG_CELCIUS(0)}},        //Tintern
    {{0, 0},                        {47828, ANALOG_VOLT(14.042)}},  //Vin
    {{19650,ANALOG_CELCIUS(29.8)},  {18700, ANALOG_CELCIUS(60)}},   //Textern

    {{0, 0},                        {48963, ANALOG_VOLT(3.752)}},   //Vb0_pin
    {{0, 0},                        {54848, ANALOG_VOLT(4.179)}},   //Vb1_pin
    {{0, 0},                        {56000, ANALOG_VOLT(8.359)}},   //Vb2_pin
    {{0, 0},                        {55667, ANALOG_VOLT(4.171)}},   //Vb3_pin

    {{0, 0},                        {54577, ANALOG_VOLT(4.182)}},   //Vb4_pin
    {{0, 0},                        {54720, ANALOG_VOLT(4.178)}},   //Vb5_pin
    {{0, 0},                        {55300, ANALOG_VOLT(4.165)}},   //Vb6_pin

    //1-1 correlation
    {{0,0},                         {10000, 10000}},                //IsmpsValue
    {{0, 0},                        {551,    ANALOG_AMP(0.300)}},    //IdischargeValue
#ifdef ANALOG_INPUTS_V_UNKNOWN
    {{0,0},                         {1, 1}},                        //UNKNOWN0
    {{0,0},                         {1, 1}},                        //UNKNOWN1
#endif
};

