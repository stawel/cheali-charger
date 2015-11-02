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
#ifndef HARDWARE_CONFIG_H_
#define HARDWARE_CONFIG_H_

#include "GlobalConfig.h"
#include "HardwareConfigGeneric.h"

//Turnigy Mega 200w X2 has a common display, so we disable custom characters
#undef ENABLE_LCD_RAM_CG

#define MAX_CHARGE_V            ANALOG_VOLT(27.000)
#define MAX_CHARGE_I            ANALOG_AMP(10.000)
#define MAX_CHARGE_P            ANALOG_WATT(200.000)

#define MAX_DISCHARGE_P         ANALOG_WATT(25.000)
#define MAX_DISCHARGE_I         ANALOG_AMP(5.000)

#define SMPS_MAX_I_OUT_PWM               TIMER1_PRECISION_PERIOD
#define DISCHARGER_UPPERBOUND_VALUE         TIMER1_PRECISION_PERIOD
#define ADC_KEY_BORDER 128

#endif /* HARDWARE_CONFIG_H_ */
