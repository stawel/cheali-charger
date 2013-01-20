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

class TheveninMethod {
public:
    static const AnalogInputs::Name VName = AnalogInputs::VoutBalancer;
    enum FallingState {NotFalling, LastRthMesurment, Falling};
    uint16_t minValue_;
    uint16_t maxValue_;
    AnalogInputs::ValueType Vend_;
    AnalogInputs::ValueType valueTh_;

    Thevenin tVout_;
    Thevenin tBal_[MAX_BANANCE_CELLS];
    FallingState Ifalling_;
    uint8_t fullCount_;
    uint8_t cells_;

    TheveninMethod(){}

    void init();
    bool isComlete(bool isEndVout, AnalogInputs::ValueType value);
    void calculateRthVth(AnalogInputs::ValueType oldValue);
    AnalogInputs::ValueType calculateNewValue(bool isEndVout, AnalogInputs::ValueType oldValue);
    AnalogInputs::ValueType calculateI();
    AnalogInputs::ValueType normalizeI(AnalogInputs::ValueType value, AnalogInputs::ValueType oldValue);
    AnalogInputs::ValueType storeOldValue(AnalogInputs::ValueType oldValue);

    static AnalogInputs::ValueType getVout() { return analogInputs.getRealValue(VName); }

    void setVI(AnalogInputs::ValueType v, AnalogInputs::ValueType i);
    void setMinI(AnalogInputs::ValueType i) {    minValue_ = i; };

};

extern TheveninMethod theveninMethod;


#endif /* THEVENINCHARGE_H_ */
