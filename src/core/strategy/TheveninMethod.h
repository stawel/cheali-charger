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
#ifndef THEVENINMETHOD_H_
#define THEVENINMETHOD_H_

#include "Hardware.h"
#include "Thevenin.h"

namespace TheveninMethod {

    void initialize(bool charge);
    bool balance_isComplete(bool isEndVout, AnalogInputs::ValueType I);

    void calculateRthVth(AnalogInputs::ValueType I);
    AnalogInputs::ValueType calculateNewI(bool isEndVout, AnalogInputs::ValueType I);

    AnalogInputs::ValueType getReadableRthCell(uint8_t cell);
    AnalogInputs::ValueType getReadableBattRth();
    AnalogInputs::ValueType getReadableWiresRth();
};


#endif /* THEVENINCHARGE_H_ */
