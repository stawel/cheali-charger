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
#include "AnalogInputsTypes.h"

namespace SMPS {
    void initialize();

    bool isPowerOn();
    bool isWorking();

    //returns the truly set Iout
    AnalogInputs::ValueType getIout();
    void trySetIout(AnalogInputs::ValueType I);

    void setVout(AnalogInputs::ValueType V);


    uint16_t getIoutPWM();
    void setIoutPWM(uint16_t IoutPWM);

    void powerOn();
    void powerOff();

};



#endif /* SMPS_H_ */
