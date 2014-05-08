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

#define SPMS_DISCHARGER_P0_POINT     ANALOG_AMP(0.200) 
#define DISCHARGER_P1_POINT          ANALOG_AMP(1.000) 
#define SPMS_P1_POINT                ANALOG_AMP(2.000) 

#define SPMS_DISCHARGER_P0_TEXT      "200mA" 
#define DISCHARGER_P1_TEXT           "1A" 
#define SPMS_P1_TEXT                 "2A"     


//temporary
#define SMPS_UPPERBOUND_VALUE               65535
#define DISCHARGER_UPPERBOUND_VALUE         65535



#endif /* HARDWARE_CONFIG_H_ */
