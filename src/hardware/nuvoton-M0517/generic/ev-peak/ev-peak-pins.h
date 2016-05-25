/*
    cheali-charger - open source firmware for a variety of LiPo chargers
    Copyright (C) 2016  Paweł Stawicki. All right reserved.

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
#ifndef EV_PEAK_PINS_H_
#define EV_PEAK_PINS_H_

// pin configuration



#define LCD_D0_PIN                      10 //(10)       P3.4 - T0, SDA
#define LCD_D1_PIN                      11 //(11)       P3.5 - T1, SCL, CKO[2]
#define BUZZER_PIN                      12 //(12)       P4.3 - PWM3[2]

//--

#define LCD_D2_PIN                      13 //(1+12)     P3.6 - nWR, CKO, ACMP0_O
#define LCD_D3_PIN                      14 //(2+12)     P3.7 - nRD
//pin 15 - XTAL2                           //(3+12)     XTAL2
//pin 16 - XTAL1                           //(4+12)     XTAL1
//pin 17 - VSS                             //(5+12)     VSS
//pin 18 - LDO_CAP                         //(6+12)     LDO_CAP (internnal voltage regulator CAP)
#define LCD_ENABLE_PIN                  19 //(7+12)     P2.0 - PWM0[2], AD8
#define LCD_RS_PIN                      20 //(8+12)     P2.1 - PWM1[2], AD9
//not used yet
//#define BACKLIGHT_PIN                   21 //(9+12)     P2.2 - PWM2[2], AD10

#define SMPS_DISABLE_PIN                24 //(12+12)    P4.0 - PWM0[2], T2EX

//--

#define SMPS_DISABLE2_PIN               25 //(1+2*12)   P2.5 - PWM5, AD13, SDA1[2]

#define BUTTON_STOP_PIN                 28 //(4+2*12)   P4.4 - nCS, SCL1
#define BUTTON_DEC_PIN                  29 //(5+2*12)   P4.5 - ALE, SDA1
#define BUTTON_INC_PIN                  30 //(6+2*12)   P4.6 - ICE_CLK (debug wire)
#define BUTTON_START_PIN                31 //(7+2*12)   P4.7 - ICE_DAT (debug wire)
#define OUTPUT_DISABLE_PIN              32 //(8+2*12)   P0.7 - SPICLK1, AD7
#define DISCHARGE_DISABLE_PIN           33 //(9+2*12)   P0.6 - MISO_1, AD6


#endif /* IMAXB6_PINS_H_ */
