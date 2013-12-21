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
#ifndef GTPOWERA610_ORIGINAL_H_
#define GTPOWERA610_ORIGINAL_H_

#include "GTPowerA6-10.h"

#define MAX_CHARGE_V            ANALOG_VOLT(25.300) //25.3V
#define MAX_CHARGE_I            ANALOG_AMP(10.000)  //10A
#define MAX_CHARGE_P            ANALOG_WATT(200.000)              //200W

#define MAX_DISCHARGE_P         ANALOG_WATT(25.000)               //25W
#define MAX_DISCHARGE_I         ANALOG_AMP(5.000)   //5A

//#define SMPS_UPPERBOUND_VALUE               8192
//#define MAX_CALIBRATION_SMPS_VALUE          (SMPS_UPPERBOUND_VALUE/8)
//#define DISCHARGER_UPPERBOUND_VALUE         8192
//#define MAX_CALIBRATION_DISCHARGER_VALUE    (DISCHARGER_UPPERBOUND_VALUE/8)

#define SPMS_DISCHARGER_P0_POINT     ANALOG_AMP(0.100) 
#define DISCHARGER_P1_POINT          ANALOG_AMP(1.000) 
#define SPMS_P1_POINT                ANALOG_AMP(2.000) 

#define SPMS_DISCHARGER_P0_TEXT      "100mA" 
#define DISCHARGER_P1_TEXT           "1A" 
#define SPMS_P1_TEXT                 "2A" 


//start warning-text
//#define TESTINGALERT

//loudness programcomplete
#define LOUDSOUND

//Lixx withoutbalancer
#define CHARGE_LIXX_WITHOUT_BALANCER

//prevent power supply
#define MAX_CURRENT_RISING ANALOG_AMP(3.000)

//screenanimation
//#define SCREENANIMATION

//knightrider effect is ADC working
//#define KNIGHTRIDEREFFECT

//use RAMCG battery icons for hitachi HD44780
#define RAM_CG

#endif /* 200W_H_ */
