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
#ifndef IMAXB6_PINS_H_
#define IMAXB6_PINS_H_

// pin configuration

#define OUTPUT_VOLTAGE_MINUS_PIN    1 //??
#define OUTPUT_VOLTAGE_PLUS_PIN     2 //??

#define DISCHARGE_CURRENT_PIN   3 //??
//pin 4 - nRST (debug wire)
//pin 5 - ACMP1_N, RXD, P3.0 - UNKNOWN
//pin 6 - AVSS
//pin 7 - ACMP1_P, TXD, P3.1 - not connected
#define TX_HW_SERIAL_PIN 7  //HW Uart Tx pin: 7(P3.1) or 38(P0.2)

#define BUTTON_STOP_PIN         8
#define BUTTON_DEC_PIN          9
#define BUTTON_INC_PIN          10
#define BUTTON_START_PIN        11
#define BUZZER_PIN              12


#define BALANCER5_LOAD_PIN      13 //(1+12)
#define BALANCER4_LOAD_PIN      14 //(2+12)
//pin 15 - XTAL2
//pin 16 - XTAL1
//pin 17 - VSS
//pin 18 - LDO_CAP - (internal voltage regulator CAP)
#define OUTPUT_DISABLE_PIN      19 //?? (7+12)
// pin 20 - the same pin for charge and discharge
#define DISCHARGE_VALUE_PIN     20 //(8+12)
#define SMPS_VALUE_BOOST_PIN    20 //(8+12)
#define BALANCER3_LOAD_PIN      21 //(9+12)
#define BALANCER2_LOAD_PIN      22 //(10+12)
#define BALANCER6_LOAD_PIN      23 //(11+12)
#define BALANCER1_LOAD_PIN      24 //(12+12)


#define MUX_ADR2_PIN            25 //(1+12+12)
#define SMPS_VALUE_BUCK_PIN     26 //(2+12+12)
#define SMPS_DISABLE_PIN        27//??(3+12+12)
#define MUX_ADR1_PIN            28 //(4+12+12)
#define MUX_ADR0_PIN            29 //(5+12+12)
//pin 30 - P4.6, ICE_CLK (debug wire)
//pin 31 - P4.7, ICE_DAT (debug wire)
#define LCD_D3_PIN              32 //(8+12+12)
#define LCD_D2_PIN              33 //(9+12+12)
#define LCD_D1_PIN              34 //(10+12+12)
#define LCD_D0_PIN              35 //(11+12+12)
#define LCD_ENABLE_PIN          36 //(12+12+12)


#define LCD_RS_PIN              37 //(1+12+12+12)
//pin 38 - TXD, CTS0, AD2, P0.2 - UNKNOWN
//pin 39 - ACMP3_N, RXD1, RTS1, AD1, P0.1 - UNKNOWN
//pin 40 - ACMP3_P, TXD1, CTS1, AD0, P0.0 - UNKNOWN
//pin 41 - VDD
//pin 42 - AVDD
//#define MUX0_Z_A_PIN
#define MUX0_Z_D_PIN            43 //(7+12+12+12)
#define DISCHARGE_DISABLE_PIN   44 //(8+12+12+12)
#define UART_TX_PIN             45 //(9+12+12+12)
#define T_EXTERNAL_PIN          45 //(9+12+12+12)
#define V_IN_PIN                46 //(10+12+12+12)
#define SMPS_CURRENT_PIN        47  //?? (11+12+12+12)
//pin 48 - PWM2, P4.2 - UNKNOWN

//virtual pin
#define T_INTERNAL_PIN			3+128

//Multiplexer addresses ??? TODO:
#define MADDR_V_BALANSER_BATT_MINUS     0
#define MADDR_V_BALANSER1               1
#define MADDR_V_BALANSER2               2
#define MADDR_V_BALANSER3               3
#define MADDR_V_BALANSER4               4
#define MADDR_V_BALANSER5               5
#define MADDR_V_BALANSER6               6
//#define MADDR_T_EXTERN                  7


#endif /* IMAXB6_PINS_H_ */
