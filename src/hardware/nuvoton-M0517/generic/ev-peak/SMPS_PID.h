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

#ifndef SMPS_PID_H_
#define SMPS_PID_H_

#include "Hardware.h"

//MV - manipulated variable in PID
#define MAX_PID_MV_FACTOR 1.5
#define MAX_PID_MV ((uint16_t) (OUTPUT_PWM_PRECISION_PERIOD * MAX_PID_MV_FACTOR))
#define PID_MV_PRECISION 8
#define MAX_PID_MV_PRECISION (((uint32_t) MAX_PID_MV)<<PID_MV_PRECISION)

namespace SMPS_PID
{
    void init(uint16_t Vin, uint16_t Vout);
    void setPID_MV(uint16_t value);
    void powerOn();
    void powerOff();
    void update();
};

#endif //SMPS_PID_H_
