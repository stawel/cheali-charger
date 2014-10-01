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

#include "HardwareConfigGeneric.h"

//based on: http://www.dx.com/p/2-5-lcd-ac-dc-dual-power-b6ac-80w-rc-balance-charger-discharger-123252

#undef ENABLE_SIMPLIFIED_VB0_VB2_CIRCUIT

#define MAX_CHARGE_V            ANALOG_VOLT(27.000) //27.0V
#define MAX_CHARGE_I            ANALOG_AMP(6.000)   //6A
#define MAX_CHARGE_P            ANALOG_WATT(80.000)               //80W

#define MAX_DISCHARGE_P         ANALOG_WATT(10.000)                //10W
#define MAX_DISCHARGE_I         ANALOG_AMP(2.000)


//1-13? correlation
#define SMPS_UPPERBOUND_VALUE               65000
#define DISCHARGER_UPPERBOUND_VALUE         TIMER1_PRECISION_PERIOD

#endif /* HARDWARE_CONFIG_H_ */
