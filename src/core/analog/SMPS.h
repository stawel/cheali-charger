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
//Switched-mode power supply

#ifndef SMPS_H_
#define SMPS_H_

#include <inttypes.h>

namespace SMPS {
    enum STATE { CHARGING, CHARGING_COMPLETE, ERROR, STOP};

    void initialize();

    STATE getState();
    bool isPowerOn();
    bool isWorking();

    uint16_t getValue();
    void setValue(uint16_t value);
    uint16_t setSmoothI(uint16_t value, uint16_t value_);
    void setRealValue(uint16_t I);

    void powerOn();
    void powerOff(STATE reason = CHARGING_COMPLETE);

};



#endif /* SMPS_H_ */
