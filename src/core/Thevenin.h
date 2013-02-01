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
#ifndef THEVENIN_H_
#define THEVENIN_H_

#include "AnalogInputs.h"

class Thevenin {
public:

    AnalogInputs::ValueType Vth_;
    int16_t Rth_V_,Rth_I_;

    AnalogInputs::ValueType VLast_;
    AnalogInputs::ValueType ILast_;
    AnalogInputs::ValueType ILastDiff_;

    Thevenin(){};
    void storeLast(AnalogInputs::ValueType VLast, AnalogInputs::ValueType ILast) { VLast_ = VLast; ILast_ = ILast; }

    void calculateRthVth(AnalogInputs::ValueType v, AnalogInputs::ValueType i);
    void calculateRth(AnalogInputs::ValueType v, AnalogInputs::ValueType i);
    void calculateVth(AnalogInputs::ValueType v, AnalogInputs::ValueType i);
    AnalogInputs::ValueType calculateI(AnalogInputs::ValueType Vc) const;

    void init(AnalogInputs::ValueType Vth,AnalogInputs::ValueType Vmax, AnalogInputs::ValueType i);
};

#endif /* THEVENIN_H_ */
