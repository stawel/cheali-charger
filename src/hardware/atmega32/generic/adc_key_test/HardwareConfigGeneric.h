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

#define LCD_BACKLIGHT_MIN       100
#define LCD_BACKLIGHT_MAX       32000

#define MAX_BANANCE_CELLS       6
#define TIMER1_PERIOD           1024

#define ENABLE_LCD_BACKLIGHT
#define ENABLE_FAN
#define ENABLE_T_INTERNAL
#define ENABLE_STACK_INFO
#define ENABLE_EXPERT_VOLTAGE_CALIBRATION

#define ANALOG_INPUTS_ADC_RESOLUTION_BITS   10
#define ANALOG_INPUTS_ADC_BURST_COUNT       10
#define ANALOG_INPUTS_ADC_ROUND_MAX_COUNT   100
#define ANALOG_INPUTS_ADC_DELTA_SHIFT       1
#define ENABLE_ANALOG_INPUTS_ADC_NOISE

#define ANALOG_INPUTS_MAX_ADC_Vout_plus_pin     ANALOG_INPUTS_MAX_ADC_VALUE

#endif /* HARDWARE_CONFIG_GENERIC_H_ */
