/*
    cheali-charger - open source firmware for a variety of LiPo chargers
    Copyright (C) 2014  Paweł Stawicki. All right reserved.

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
#ifndef PINS_H_
#define PINS_H_

// pin configuration
#define SMPS_CURRENT_PIN                1  //P1.5 - ACMP0_P, MOSI_0, AIN5
#define DISCHARGE_CURRENT_PIN           2  //P1.6 - ACMP2_N, MISO_0, AIN6
#define OUTPUT_VOLTAGE_PIN              3  //ACMP2_P, SPICLK0, AIN7, P1.7
//pin 4 - nRST                             //nRST
#define RX_HW_SERIAL_PIN5               5  //(5)        P3.0 - RXD[2], ACMP1_N
//pin 6 - AVSS                             //(6)        AVSS
#define TX_HW_SERIAL_PIN7               7  //(7)        P3.1 - TXD[2], ACMP1_P
#define MUX_ADR1_PIN                    8  //P3.2 - T0EX, STADC, nINT0
#define MUX_ADR2_PIN                    9  //P3.3 - T1EX, MCLK, nINT1
#define LCD_D0_PIN                      10 //P3.4 - SDA0, T0
#define LCD_D1_PIN                      11 //P3.5 - CKO, SCL0, T1
#define BUZZER_PIN                      12 //P4.3 - PWM3
#define LCD_D2_PIN                      13 //P3.6 - nWR, CKO, ACMP0_O
#define LCD_D3_PIN                      14 //P3.7 - nRD
//pin 15 - XTAL2                           //XTAL2
//pin 16 - XTAL1                           //XTAL1
//pin 17 - VSS                             //VSS
//pin 18 - LDO_CAP                         //LDO_CAP (internnal voltage regulator CAP)
#define LCD_ENABLE_PIN                  19 //P2.0 - AD8, PWM0
#define LCD_RS_PIN                      20 //P2.1 - AD9, PWM1
#define BACKLIGHT_PIN                   21 //P2.2, AD10, PWM2
#define SMPS_VALUE_BUCK_PIN             22 //P2.3, AD11, PWM3
//[the same pin for charge and discharge: pin 23]
#define DISCHARGE_VALUE_PIN             23 //P2.4, AD12, PWM4, SCL1
#define SMPS_VALUE_BOOST_PIN            23 //P2.4, AD12, PWM4, SCL1
#define SMPS_DISABLE_PIN                24 //P4.0 - PWM0, T2EX
#define SMPS_DISABLE2_PIN               25 //P2.5 - AD13, PWM5, SDA
#define FAN_ENABLE                      26 //P2.6, AD14, PWM6, ACMP1_O
#define MCU_BUS_PIN                     27 //P2.7, AD15, PWM7
#define BUTTON_STOP_PIN                 28 //P4.4 - nCS, SCL1
#define BUTTON_DEC_PIN                  29 //P4.5 - ALE, SDA1
#define BUTTON_INC_PIN                  30 //P4.6 - ICE_CLK
#define BUTTON_START_PIN                31 //P4.7 - ICE_DAT
#define OUTPUT_DISABLE_PIN              32 //P0.7 - AD7, SPICLK
#define DISCHARGE_DISABLE_PIN           33 //P0.6 - AD6, MISO_1
#define BALANCER6_LOAD_PIN              34 //P0.5 - AD5, MOSI_1
#define BALANCER5_LOAD_PIN              35 //P0.4 - AD4, SPISS1
#define OUTPUT_VOLTAGE_POLARITY_PIN     36 //P4.1 - PWM1, T3
#define BALANCER4_LOAD_PIN              37 //P0.3 - RXD, RTS0, AD3,
#define BALANCER3_LOAD_PIN              38 //P0.2 - TXD, CTS0, AD2
#define BALANCER2_LOAD_PIN              39 //P0.1 - ACMP3_N, RXD1, RTS1, AD1
#define BALANCER1_LOAD_PIN              40 //P0.0 - ACMP3_P, TXD1, CTS1, AD0
//pin 41 - VDD                             //VDD
//pin 42 - AVDD                            //AVDD
//pin 43 - UNKNOWN                         //UNKNOWN
#define T_EXTERNAL_PIN                  44 //P1.0 - nWRL,T2, AIN0
#define V_IN_PIN                        45 //P1.1 - nWRH, T3, AIN1
//pin 46 - UNKNOWN                         //UNKNOWN
#define MUX0_Z_D_PIN                    47 //P1.4 - ACMP0_N,SPISS0, AIN4
#define MUX_ADR0_PIN                    48 //P4.2 - PWM2


//virtual pin
#define T_INTERNAL_PIN                  3+128


//Multiplexer addresses:
#define MADDR_V_BALANSER1               0
#define MADDR_V_BALANSER2               1
#define MADDR_V_BALANSER3               2
#define MADDR_V_BALANSER4               3
#define MADDR_V_BALANSER5               4
#define MADDR_V_BALANSER6               5
#define MADDR_V_GND_REF                 6
#define MADDR_V_OUTMUX                  7


#endif /* PINS_H_ */
