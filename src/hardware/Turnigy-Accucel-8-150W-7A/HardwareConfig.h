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

#undef  MAX_BANANCE_CELLS
#define MAX_BANANCE_CELLS 8

#define LCD_D3_PIN              22
#define LCD_D2_PIN              21
#define LCD_D1_PIN              20
#define LCD_D0_PIN              19
#define LCD_ENABLE_PIN          23
#define LCD_RS_PIN              24

#define MAX_CHARGE_V            ANALOG_VOLT(36.300)
#define MAX_CHARGE_I            ANALOG_AMP(7.000)
#define MAX_CHARGE_P            ANALOG_WATT(150.000)

#define MAX_DISCHARGE_P         ANALOG_WATT(25.000)
#define MAX_DISCHARGE_I         ANALOG_AMP(5.000)

//#define SMPS_UPPERBOUND_VALUE               TIMER1_PRECISION_PERIOD
//#define DISCHARGER_UPPERBOUND_VALUE         TIMER1_PRECISION_PERIOD

#define ENABLE_BALANCER_PWR

#define CALIBRATION_SPEED           1

#define CALIBRATION_CHARGE_POINT0_mA    50
#define CALIBRATION_CHARGE_POINT1_mA    1000
#define CALIBRATION_DISCHARGE_POINT0_mA 50
#define CALIBRATION_DISCHARGE_POINT1_mA 300


//prevent power supply (>50W chargers)
//#define ENABLE_SMOOTHCURRENT ANALOG_AMP(1.000)

//screenanimation
#define ENABLE_SCREENANIMATION

//knightrider effect is ADC working
#define ENABLE_KNIGHTRIDEREFFECT

//use RAMCG battery icons for hitachi HD44780
#define ENABLE_LCD_RAM_CG

//audio mute function
#define ENABLE_MUTEAUDIO

//extended battery info (charge time limit)
#define ENABLE_TIME_LIMIT    

//alternative battery name
//#define ENABLE_ALTERNATIVE_BATTERY_NAME


//#define ENABLE_CYCLING



#endif /* HARDWARE_CONFIG_H_ */
