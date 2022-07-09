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
    {{0,  0},         {65535,  30021}}, //Vout_plus_pin
    {{0,  0},         {0,  0}},         //Vout_minus_pin
    {{160,  100},     {2668,  1000}},   //Ismps
    {{478,  100},     {6638,  1000}},   //Idischarge

    {{0,  0},         {5062,  1000}},   //VoutMux

    /*
    Calculated using the following from the datasheet:
    Vtemp (mV) = Gain (mV/°C) x Temperature (°C) + Offset (mV)

    Typical gain: -1.75mV/°C
    Typical offset: 724mV

    @ T=0, V=724mV, ADC=(.724/5)*1024=148, Adj ADC (<<4)=2368
    @ T=50, V=724mV+(50*-1.75mV)=636.5mV, ADC=(.6365/5)*1024=130, Adj ADC (<<4)=2080
    */
    {{2080,  5000},   {2368,  0}},      //Tintern
    {{0,  0},         {65535,  35228}}, //Vin
    {{4701,  3660},   {0,  0}},         //Textern

    {{0,  0},         {0,  0}},         //Vb0_pin
    {{0,  0},         {65535,  10117}}, //Vb1_pin
    {{0,  0},         {65535,  10117}}, //Vb2_pin
    {{0,  0},         {65535,  10117}}, //Vb3_pin
    {{0,  0},         {65535,  10117}}, //Vb4_pin
    {{0,  0},         {65535,  10117}}, //Vb5_pin
    {{0,  0},         {65535,  10117}}, //Vb6_pin

    {{162,  100},     {2670,  1000}},   //IsmpsSet
    {{502,  100},     {6858,  1000}},   //IdischargeSet
};

STATIC_ASSERT(sizeOfArray(AnalogInputs::inputsP_) == AnalogInputs::PHYSICAL_INPUTS);
