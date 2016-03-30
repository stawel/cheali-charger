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

#define THEVENIN_VOUT_IDX       MAX_BANANCE_CELLS
#define THEVENIN_CELL_IDX(n)    n

namespace Thevenin {

    struct Resistance {
       //R = iV/uI;
       //when discharging the resistance is negative (iV_ < 0)
       //in a Thevenin model this is mathematically equivalent
       //to a positive resistance and a current flowing in the opposite direction

        int16_t iV;
        uint16_t uI;
    };

    struct Thevenin {
        AnalogInputs::ValueType VLast_;
        AnalogInputs::ValueType ILast_;
        AnalogInputs::ValueType ILastDiff_;
        AnalogInputs::ValueType Vth_;
        Resistance Rth;
    };


    AnalogInputs::ValueType getReadableRth(int16_t iV, uint16_t uI);
    AnalogInputs::ValueType getReadableRth(uint8_t idx);

    void storeI(AnalogInputs::ValueType I);
    void calculateRthVth(AnalogInputs::ValueType I);
    AnalogInputs::ValueType calculateI();

    void initialize(bool charge);

}

#endif /* THEVENIN_H_ */
