/*
    cheali-charger - open source firmware for a variety of LiPo chargers
    Copyright (C) 2014 Paweł Stawicki. All right reserved.

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
#ifndef IRQ_PRIORITY_H_
#define IRQ_PRIORITY_H_


#define SOFTWARE_SERIAL_IRQ_PRIORITY    0
#define TIMER_IRQ_PRIORITY              2
#define HARDWARE_SERIAL_IRQ_PRIORITY    3
#define ADC_IRQ_PRIORITY                2
#define ADC_C_DISCHARGE_IRQ_PRIORITY    2
#define OUTPUT_PWM_IRQ_PRIORITY         1


#endif /* IRQ_PRIORITY_H_ */

