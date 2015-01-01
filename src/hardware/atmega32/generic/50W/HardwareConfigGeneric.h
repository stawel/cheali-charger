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
#ifndef HARDWARE_CONFIG_GENERIC_H_
#define HARDWARE_CONFIG_GENERIC_H_


#include "AnalogInputsTypes.h"

#define MAX_BANANCE_CELLS       6
#define TIMER1_PERIOD           512

#define ENABLE_SIMPLIFIED_VB0_VB2_CIRCUIT
//TODO: should be implemented!
//when the imaxB6 discharges, the voltage on Vb0_pin
//(Vb0_pin = VBATT- on the balance port)
//drops below 0V so the ADC doesn't see it.
//This is why we see a bigger Vb1 resistance.
#define ENABLE_B0_DISCHARGE_VOLTAGE_CORRECTION
#define ENABLE_STACK_INFO
#define ENABLE_GET_PID_VALUE
#define ENABLE_EXPERT_VOLTAGE_CALIBRATION

#define SETTINGS_EXTERNAL_T_DEFAULT         0

#define ANALOG_INPUTS_ADC_RESOLUTION_BITS   10
#define ANALOG_INPUTS_ADC_BURST_COUNT       10
#define ANALOG_INPUTS_ADC_ROUND_MAX_COUNT   100
#define ANALOG_INPUTS_ADC_DELTA_SHIFT       1
#define ENABLE_ANALOG_INPUTS_ADC_NOISE

#define ANALOG_INPUTS_MAX_ADC_Vout_plus_pin ANALOG_INPUTS_MAX_ADC_VALUE

#endif /* HARDWARE_CONFIG_GENERIC_H_ */
