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
#ifndef GTPOWERA610_H_
#define GTPOWERA610_H_

#include "AnalogInputs.h"

#define LCD_LINES               2
#define LCD_COLUMNS             16
#define LCD_BACKLIGHT_MIN       100
#define LCD_BACKLIGHT_MAX       3000


#define MAX_CHARGE_V            ANALOG_VOLT(25.300) //25.3V
#define MAX_CHARGE_I            ANALOG_AMP(10.000)  //10A
#define MAX_CHARGE_P            200000              //200W

#define MAX_DISCHARGE_P         25000               //25W
#define MAX_DISCHARGE_I         ANALOG_AMP(5.000)   //5A

#define MAX_BANANCE_CELLS       6

//use unknown mux values
//#define ANALOG_INPUTS_V_UNKNOWN
#define TIMER1_PERIOD_MICROSECONDS 512
#define SMPS_MAX_VALUE             ((F_CPU / 2000000) * TIMER1_PERIOD_MICROSECONDS)
#define SMPS_UPPERBOUND_VALUE     (SMPS_MAX_VALUE/4)
#define MAX_CALIBRATION_SMPS_VALUE 760

#define ENABLE_LCD_BACKLIGHT
#define ENABLE_FAN
#define ENABLE_T_INTERNAL

#define ENABLE_STACK_INFO

#include "LiquidCrystal.h"
#include "Keyboard.h"
#include "Timer.h"
#include "SMPS.h"
#include "Discharger.h"
#include "Balancer.h"
#include "TimerOne.h"

class Timer;
class Discharger;
class SMPS;

extern LiquidCrystal lcd;
extern AnalogInputs analogInputs;
extern Timer timer;
extern SMPS smps;
extern Discharger discharger;
extern Balancer balancer;

namespace hardware {
    void initialize();
    uint8_t getKeyPressed();
    void delay(uint16_t t);
    void setLCDBacklight(uint8_t val);
    void setBuzzer(uint16_t val);
    void setBatteryOutput(bool enable);
    void setChargerOutput(bool enable);
    void setBalancerOutput(bool enable);
    void setDischargerOutput(bool enable);
    void setChargerValue(uint16_t value);
    void setDischargerValue(uint16_t value);
    void setFan(bool enable);
    void setBalancer(uint8_t balance);
    inline void doInterrupt(){}
}


#endif /* GTPOWERA610_H_ */
