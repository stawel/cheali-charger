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

#define OUTPUT_VOLTAGE_MINUS_PIN        1  //(1)        P1.5 - MOSI_0, AIN5, ACMP0_P
#define OUTPUT_VOLTAGE_PLUS_PIN         2  //(2)        P1.6 - MISO_0, AIN6, ACMP2_N
#define DISCHARGE_CURRENT_PIN           3  //(3)        P1.7 - SPICLK0, AIN7, ACMP2_P
//pin 4 - nRST                             //(4)        nRST (debug wire)
//[HW Uart Rx pin: 5(P3.0) or 37(P0.3) - selectable (not connected)]
#define RX_HW_SERIAL_PIN5               5  //(5)        P3.0 - RXD[2], ACMP1_N
//pin 6 - AVSS                             //(6)        AVSS
//[HW Uart Tx pin: 7(P3.1) or 38(P0.2) - selectable (not connected)]
#define TX_HW_SERIAL_PIN7               7  //(7)        P3.1 - TXD[2], ACMP1_P
#define BUTTON_STOP_PIN                 8  //(8)        P3.2 - nINT0, STADC, T0EX
#define BUTTON_DEC_PIN                  9  //(9)        P3.3 - nINT1, MCLK, T1EX
#define BUTTON_INC_PIN                  10 //(10)       P3.4 - T0, SDA
#define BUTTON_START_PIN                11 //(11)       P3.5 - T1, SCL, CKO[2]
#define BUZZER_PIN                      12 //(12)       P4.3 - PWM3[2]


#define BALANCER5_LOAD_PIN              13 //(1+12)     P3.6 - nWR, CKO, ACMP0_O
#define BALANCER4_LOAD_PIN              14 //(2+12)     P3.7 - nRD
//pin 15 - XTAL2                           //(3+12)     XTAL2
//pin 16 - XTAL1                           //(4+12)     XTAL1
//pin 17 - VSS                             //(5+12)     VSS
//pin 18 - LDO_CAP                         //(6+12)     LDO_CAP (internnal voltage regulator CAP)
#define OUTPUT_DISABLE_PIN              19 //(7+12)     P2.0 - PWM0[2], AD8
//[the same pin for charge and discharge: pin 20]
#define DISCHARGE_VALUE_PIN             20 //(8+12)     P2.1 - PWM1[2], AD9
#define SMPS_VALUE_BOOST_PIN            20 //(8+12)
#define BALANCER3_LOAD_PIN              21 //(9+12)     P2.2 - PWM2[2], AD10
#define BALANCER2_LOAD_PIN              22 //(10+12)    P2.3 - PWM3[2], AD11
#define BALANCER6_LOAD_PIN              23 //(11+12)    P2.4 - PWM4, AD12, SCL1[2]
#define BALANCER1_LOAD_PIN              24 //(12+12)    P4.0 - PWM0[2], T2EX


#define MUX_ADR2_PIN                    25 //(1+2*12)   P2.5 - PWM5, AD13, SDA1[2]
#define SMPS_VALUE_BUCK_PIN             26 //(2+2*12)   P2.6 - PWM6, AD14, ACMP1_O
#define SMPS_DISABLE_PIN                27 //(3+2*12)   P2.7 - PWM7, AD15
#define MUX_ADR1_PIN                    28 //(4+2*12)   P4.4 - nCS, SCL1
#define MUX_ADR0_PIN                    29 //(5+2*12)   P4.5 - ALE, SDA1
//pin 30 - ICE_CLK                         //(6+2*12)   P4.6 - ICE_CLK (debug wire)
//pin 31 - ICE_DAT                         //(7+2*12)   P4.7 - ICE_DAT (debug wire)
#define LCD_D3_PIN                      32 //(8+2*12)   P0.7 - SPICLK1, AD7
#define LCD_D2_PIN                      33 //(9+2*12)   P0.6 - MISO_1, AD6
#define LCD_D1_PIN                      34 //(10+2*12)  P0.5 - MOSI_1, AD5
#define LCD_D0_PIN                      35 //(11+2*12)  P0.4 - SPISS1, AD4
#define LCD_ENABLE_PIN                  36 //(12+2*12)  P4.1 - PWM1[2], T3EX


#define LCD_RS_PIN                      37 //(1+3*12)   P0.3 - RTS0, AD3, RXD[2]
//[HW Uart Tx pin: 7(P3.1) or 38(P0.2) - selectable (not connected)]
#define TX_HW_SERIAL_PIN38              38 //(2+3*12)   P0.2 - TXD, CTS0, AD2
//pin 39 - UNKNOWN                         //(3+3*12)   P0.1 - ACMP3_N, RXD1, RTS1, AD1
//pin 40 - UNKNOWN                         //(4+3*12)   P0.0 - ACMP3_P, TXD1, CTS1, AD0
//pin 41 - VDD                             //(5+3*12)   VDD
//pin 42 - AVDD                            //(6+3*12)   AVDD
#define MUX0_Z_D_PIN                    43 //(7+3*12)   P1.0 - T2, AIN0, nWRL
#define DISCHARGE_DISABLE_PIN           44 //(8+3*12)   P1.1 - T3, AIN1, nWRH
#define UART_TX_PIN                     45 //(9+3*12)   P1.2 - RXD1[2], AIN2
#define T_EXTERNAL_PIN                  45 //(9+3*12)
#define V_IN_PIN                        46 //(10+3*12)  P1.3 - TXD1[2], AIN3
#define SMPS_CURRENT_PIN                47 //(11+3*12)  P1.4 - SPISS0, AIN4, ACMP0_N
//pin 48 - UNKNOWN                         //(12+3*12)  P4.2 - PWM2[2]



//virtual pin
#define T_INTERNAL_PIN                  3+128

//Multiplexer addresses:
#define MADDR_V_BALANSER_BATT_MINUS     0
#define MADDR_V_BALANSER1               1
#define MADDR_V_BALANSER2               2
#define MADDR_V_BALANSER3               3
#define MADDR_V_BALANSER4               4
#define MADDR_V_BALANSER5               5
#define MADDR_V_BALANSER6               6
//#define MADDR_T_EXTERN                  7


#endif /* PINS_H_ */
