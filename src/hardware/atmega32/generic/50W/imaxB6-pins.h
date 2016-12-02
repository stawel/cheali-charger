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
#ifndef PINS_H_
#define PINS_H_

// pin configuration
#define MUX_ADR0_PIN            1
#define MUX_ADR1_PIN            2
#define MUX_ADR2_PIN            3

//pin 4 - 8 - reset, Vcc, GND, XTAL2, XTAL1

//pin 9 - rs232 RX
#define BALANCER4_LOAD_PIN      9
//pin 10 - rs232 TX
#define UART_TX                 10

#define BALANCER5_LOAD_PIN      11
//?? fan not present - not tested
#define FAN_PIN                 12
#define OUTPUT_DISABLE_PIN      12
#define SMPS_VALUE_BUCK_PIN     13
// pin 14 - the same pin for charge and discharge
#define DISCHARGE_VALUE_PIN     14
#define SMPS_VALUE_BOOST_PIN    14
#define SMPS_DISABLE_PIN        15
#define BALANCER6_LOAD_PIN      16

// pin 17,18 - VCC, GND
#define LCD_D3_PIN              19
#define LCD_D2_PIN              20
#define LCD_D1_PIN              21
#define LCD_D0_PIN              22
#define LCD_ENABLE_PIN          23
#define LCD_RS_PIN              24

#define BALANCER1_LOAD_PIN      25
#define BALANCER2_LOAD_PIN      26

// pin 27 - 29 - AVcc, GND, AREF

//pin 30 - A7
#define DISCHARGE_DISABLE_PIN   30
//pin 31 - A6
#define MUX0_Z_A_PIN            A6
#define MUX0_Z_D_PIN            31
//pin 32 - A5
#define BALANCER3_LOAD_PIN      32
//pin 33 - A4
#define V_IN_PIN                A4
//pin 34 - A3
#define SMPS_CURRENT_PIN        A3
//pin 35 - A2
#define DISCHARGE_CURRENT_PIN   A2
//pin 36 - A1
#define OUTPUT_VOLTAGE_MINUS_PIN    A1
//pin 37 - A0
#define OUTPUT_VOLTAGE_PLUS_PIN     A0

// pin 38,39 - Vcc, GND

#ifndef BUTTON_STOP_PIN
#define BUTTON_STOP_PIN         40
#define BUTTON_DEC_PIN          41
#define BUTTON_INC_PIN          42
#define BUTTON_START_PIN        43
#endif //BUTTON_STOP_PIN

#define BUZZER_PIN              44



//Multiplexer addresses
#define MADDR_V_BALANSER_BATT_MINUS     0
#define MADDR_V_BALANSER1               1
#define MADDR_V_BALANSER2               2
#define MADDR_V_BALANSER3               3
#define MADDR_V_BALANSER4               4
#define MADDR_V_BALANSER5               5
#define MADDR_V_BALANSER6               6
#define MADDR_T_EXTERN                  7

#endif /* PINS_H_ */
