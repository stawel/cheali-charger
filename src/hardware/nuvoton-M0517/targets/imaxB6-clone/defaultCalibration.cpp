/*
    cheali-charger - open source firmware for a variety of LiPo chargers
    Copyright (C) 2013  Pawe³ Stawicki. All right reserved.

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

  {{0, ANALOG_VOLT (0.000)}, {25124, ANALOG_VOLT (23.328)}},    //Vout_plus_pin
  {{0, ANALOG_VOLT (0.000)}, {25124, ANALOG_VOLT (23.328)}},    //Vout_minus_pin
  {{113, ANALOG_AMP (0.050)}, {5025, ANALOG_AMP (1.000)}},      //Ismps
  {{857, ANALOG_AMP (0.050)}, {5703, ANALOG_AMP (0.300)}},      //Idischarge


  {{0, ANALOG_VOLT (0.000)}, {1, ANALOG_VOLT (0.001)}}, //VoutMux
  {{8000, ANALOG_CELCIUS (59.40)}, {8642, ANALOG_CELCIUS (34.80)}},   //Tintern
  {{0, ANALOG_VOLT (0.000)}, {23508, ANALOG_VOLT (14.012)}},    //Vin
  {{9780, ANALOG_CELCIUS (28.60)}, {9144, ANALOG_CELCIUS (60.00)}},   //Textern

  {{0, ANALOG_VOLT (0.000)}, {23765, ANALOG_VOLT (3.901)}}, 	//Vb0_pin
  {{0, ANALOG_VOLT (0.000)}, {23765, ANALOG_VOLT (3.901)}},     //Vb1_pin
  {{0, ANALOG_VOLT (0.000)}, {48029, ANALOG_VOLT (7.613)}},     //Vb2_pin
  {{0, ANALOG_VOLT (0.000)}, {24315, ANALOG_VOLT (3.750)}},     //Vb3_pin
  {{0, ANALOG_VOLT (0.000)}, {25561, ANALOG_VOLT (3.957)}},     //Vb4_pin
  {{0, ANALOG_VOLT (0.000)}, {26409, ANALOG_VOLT (4.092)}},     //Vb5_pin
  {{0, ANALOG_VOLT (0.000)}, {26614, ANALOG_VOLT (4.108)}},     //Vb6_pin

#if MAX_BANANCE_CELLS > 6
  {{,}, {,}},                   //Vb7_pin
  {{,}, {,}},                   //Vb8_pin
#endif

  {{114, ANALOG_AMP (0.050)}, {5028, ANALOG_AMP (1.000)}},      //IsmpsSet
  {{68, ANALOG_AMP (0.050)}, {431, ANALOG_AMP (0.300)}},        //IdischargeSet
};

namespace
{
  void assert ()
  {
    STATIC_ASSERT (sizeOfArray (AnalogInputs::inputsP_) == AnalogInputs::PHYSICAL_INPUTS);
  }
}
