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
#ifndef THEVENINCHARGESTRATEGY_H_
#define THEVENINCHARGESTRATEGY_H_

#include "SimpleChargeStrategy.h"
#include "Thevenin.h"
#include "TheveninMethod.h"

namespace TheveninChargeStrategy
{
    extern const Strategy::VTable vtable;

    void powerOn();
    Strategy::statusType doStrategy();
    void powerOff();
    bool isEndVout();

    void setVIB(AnalogInputs::ValueType v, AnalogInputs::ValueType i, bool balance);
    void setMinI(AnalogInputs::ValueType i);

};


#endif /* THEVENINCHARGESTRATEGY_H_ */
