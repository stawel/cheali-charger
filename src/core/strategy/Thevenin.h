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

namespace Thevenin {

	typedef struct {
	   //R = iV/uI;
	   //when discharging the resistance is negative (iV_ < 0)
	   //in a Thevenin model this is mathematically equivalent
	   //to a positive resistance and a current flowing in the opposite direction

		int16_t iV;
		uint16_t uI;

	} Resistance;

	AnalogInputs::ValueType getReadableRth(const Resistance * R);


	typedef struct {
		AnalogInputs::ValueType VLast_;
		AnalogInputs::ValueType ILast_;
		AnalogInputs::ValueType ILastDiff_;
		AnalogInputs::ValueType Vth_;

		Resistance Rth;
	} Data;


    static inline void storeLast(Data *d, AnalogInputs::ValueType VLast, AnalogInputs::ValueType ILast) { d->VLast_ = VLast; d->ILast_ = ILast; }

    void calculateRthVth(Data *d, AnalogInputs::ValueType v, AnalogInputs::ValueType i);
    void calculateRth(Data *d, AnalogInputs::ValueType v, AnalogInputs::ValueType i);
    void calculateVth(Data *d, AnalogInputs::ValueType v, AnalogInputs::ValueType i);
    AnalogInputs::ValueType calculateI(Data *d, AnalogInputs::ValueType Vc);

    void init(Data *d, AnalogInputs::ValueType Vth,AnalogInputs::ValueType Vmax, AnalogInputs::ValueType i, bool charge);
}
#endif /* THEVENIN_H_ */
