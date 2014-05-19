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

#define MAX_CHARGE_V            ANALOG_VOLT(26.100) //25.3V
#define MAX_CHARGE_I            ANALOG_AMP(10.000)  //10A
#define MAX_CHARGE_P            ANALOG_WATT(200.000)  //200W

#define MAX_DISCHARGE_P         ANALOG_WATT(25.000)   //25W
#define MAX_DISCHARGE_I         ANALOG_AMP(5.000)   //5A

//#define SMPS_UPPERBOUND_VALUE               TIMER1_PRECISION_PERIOD
//#define DISCHARGER_UPPERBOUND_VALUE         TIMER1_PRECISION_PERIOD


#define CALIBRATION_SPEED           1

#define CALIBRATION_CHARGE_POINT0_mA    50
#define CALIBRATION_CHARGE_POINT1_mA    1000
#define CALIBRATION_DISCHARGE_POINT0_mA 50
#define CALIBRATION_DISCHARGE_POINT1_mA 300




//Lixx withoutbalancer
//#define CHARGE_LIXX_WITHOUT_BALANCER

//prevent power supply
#define MAX_CURRENT_RISING ANALOG_AMP(3.000)

//screenanimation
#define SCREENANIMATION

//knightrider effect is ADC working
#define KNIGHTRIDEREFFECT

//use RAMCG battery icons for hitachi HD44780
#define RAM_CG

//temporary
#define SMPS_UPPERBOUND_VALUE               65535
#define DISCHARGER_UPPERBOUND_VALUE         65535





#define ENABLE_TIME_LIMIT
//#define ENABLE_SCREENANIMATION
//#define ENABLE_KNIGHTRIDEREFFECT
//#define ENABLE_RAM_CG
//#define ENABLE_SMOOTHCURRENTANALOG_AMP(3.000)
//#define ENABLE_CYCLING
//#define ENABLE_MUTEAUDIO

#endif /* HARDWARE_CONFIG_H_ */
