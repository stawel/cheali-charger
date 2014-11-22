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
#ifndef OUTPUT_PWM_H_
#define OUTPUT_PWM_H_

//32kHz based on PWM_GET_CNR(PWMA, PWM_CH1);
#define OUTPUT_PWM_PERIOD 780

#define OUTPUT_PWM_PRECISION_FACTOR 42
#define OUTPUT_PWM_PRECISION_PERIOD (OUTPUT_PWM_PERIOD * OUTPUT_PWM_PRECISION_FACTOR)


namespace outputPWM {


	void initialize(void);
	void setPWM(uint8_t pin, uint32_t value);
	void disablePWM(uint8_t pin);

} //namespace outputPWM


#endif //OUTPUT_PWM_H_
