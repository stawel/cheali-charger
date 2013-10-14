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
    enum FallingState {NotFalling, LastRthMesurment, Falling};

    extern AnalogInputs::ValueType Vend_;
    extern Thevenin tVout_;
    extern Thevenin tBal_[MAX_BANANCE_CELLS];
    extern AnalogInputs::Name iName_;

    void initialize(AnalogInputs::Name iName);
    bool isComlete(bool isEndVout, AnalogInputs::ValueType value);
    bool isBelowMin(AnalogInputs::ValueType value);
    void calculateRthVth(AnalogInputs::ValueType oldValue);
    AnalogInputs::ValueType calculateNewValue(bool isEndVout, AnalogInputs::ValueType oldValue);
    AnalogInputs::ValueType calculateI();
    AnalogInputs::ValueType normalizeI(AnalogInputs::ValueType value, AnalogInputs::ValueType oldValue);
    void storeOldValue(AnalogInputs::ValueType oldValue);

    void setVIB(AnalogInputs::ValueType v, AnalogInputs::ValueType i, bool balance);
    void setMinI(AnalogInputs::ValueType i);
    AnalogInputs::ValueType getImax();

    AnalogInputs::ValueType getReadableRthCell(uint8_t cell);
    AnalogInputs::ValueType getReadableBattRth();
    AnalogInputs::ValueType getReadableWiresRth();
};


#endif /* THEVENINCHARGE_H_ */
