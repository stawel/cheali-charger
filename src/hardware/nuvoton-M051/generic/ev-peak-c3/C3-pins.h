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

#define V_IN_PIN                         1

#define OUTPUT_VOLTAGE_POLARITY_PIN     36
#define OUTPUT_VOLTAGE_PIN              43
#define OUTPUT_DISABLE_PIN              32

#define DISCHARGE_CURRENT_PIN           45
#define DISCHARGE_VALUE_PIN             23
#define DISCHARGE_DISABLE_PIN           33

#define SMPS_VALUE_BOOST_PIN            23
#define SMPS_VALUE_BUCK_PIN             22
#define SMPS_DISABLE_PIN                25
#define SMPS_BOOST_DISABLE_PIN          24
#define SMPS_CURRENT_PIN                44

#define UART_TX_PIN                      7
#define BUZZER_PIN                      12
#define BACKLIGHT_PIN                   21
#define FAN_ENABLE                      26

#define LCD_D0_PIN                      10
#define LCD_D1_PIN                      11
#define LCD_D2_PIN                      13
#define LCD_D3_PIN                      14
#define LCD_ENABLE_PIN                  19
#define LCD_RS_PIN                      20

#define BUTTON_STOP_PIN                 28
#define BUTTON_DEC_PIN                  29
#define BUTTON_INC_PIN                  30
#define BUTTON_START_PIN                31

#define BALANCER1_LOAD_PIN              40
#define BALANCER2_LOAD_PIN              39
#define BALANCER3_LOAD_PIN              38
#define BALANCER4_LOAD_PIN              37
#define BALANCER5_LOAD_PIN              35
#define BALANCER6_LOAD_PIN              34

#define MUX_ADR0_PIN                     9
#define MUX_ADR1_PIN                     8
#define MUX_ADR2_PIN                    48
#define MUX0_Z_D_PIN                    47

#define MADDR_V_BALANSER1                0
#define MADDR_V_BALANSER2                1
#define MADDR_V_BALANSER3                2
#define MADDR_V_BALANSER4                3
#define MADDR_V_BALANSER5                4
#define MADDR_V_BALANSER6                5
#define MADDR_V_GND_REF                  6
#define MADDR_V_OUTMUX                   7

#define T_EXTERNAL_PIN                   3
#define T_INTERNAL_PIN                  3+128

#endif /* PINS_H_ */
