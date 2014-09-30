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
#ifndef HARDWARE_CONFIG_GENERIC_H_
#define HARDWARE_CONFIG_GENERIC_H_


#include "AnalogInputsTypes.h"

#define LCD_LINES               2
#define LCD_COLUMNS             16

#define MAX_BANANCE_CELLS       6
#define TIMER1_PERIOD           256

#define CALIBRATION_CHARGE_POINT0_mA    50
#define CALIBRATION_CHARGE_POINT1_mA    1000
#define CALIBRATION_DISCHARGE_POINT0_mA 50
#define CALIBRATION_DISCHARGE_POINT1_mA 300


#define ENABLE_SIMPLIFIED_VB0_VB2_CIRCUIT
//TODO: should be implemented!
//when the imaxB6 discharges, the voltage on Vb0_pin
//(Vb0_pin = VBATT- on the balance port)
//drops below 0V so the ADC doesn't see it.
//This is why we see a bigger Vb1 resistance.
#define ENABLE_B0_DISCHARGE_VOLTAGE_CORRECTION
//#define ENABLE_STACK_INFO

#define ENABLE_SERIAL_LOG
#define ENABLE_EXT_TEMP_AND_UART_COMMON_OUTPUT

#define ENABLE_TX_HW_SERIAL   // if set, need to adjust TX_HW_SERIAL_PIN in imaxB6-pins.h

#define ENABLE_GET_PID_VALUE
#define ENABLE_EXPERT_VOLTAGE_CALIBRATION
#define ENABLE_T_INTERNAL

#define DEFAULT_SETTINGS_EXTERNAL_T 0

#define ANALOG_INPUTS_ADC_BURST_COUNT 		70
#define ANALOG_INPUTS_ADC_ROUND_MAX_COUNT 	100
#define ANALOG_INPUTS_ADC_DELTA_SHIFT 		4



#define ENABLE_SCREEN_ANIMATION
#define ENABLE_TIME_LIMIT
#define ENABLE_LCD_RAM_CG
#define ENABLE_SCREEN_KNIGHTRIDEREFFECT



#define ENABLE_EEPROM_CRC
#define ENABLE_EEPROM_RESTORE_DEFAULT
#define ENABLE_SETTINGS_MENU_RESET

#define ENABLE_PROGRAM_MENU_RESET_NAME
#define ENABLE_PROGRAM_MENU_EDIT_NAME

#define ENABLE_CALIBRATION

#define STRINGS_HEADER "strings/standard.h"


#endif /* HARDWARE_CONFIG_GENERIC_H_ */
