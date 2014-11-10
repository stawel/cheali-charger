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

#define __STDC_LIMIT_MACROS
#include <stdint.h>
#include <stdlib.h>

#include "Thevenin.h"
#include "Utils.h"

AnalogInputs::ValueType Resistance::getReadableRth()
{
    if(uI == 0)
        return 0;
    uint32_t R = abs(iV);
    R*=1000;
    R/=uI;
    return R;
}

void Thevenin::init(AnalogInputs::ValueType Vth,AnalogInputs::ValueType Vmax, AnalogInputs::ValueType i, bool charge)
{
    AnalogInputs::ValueType Vfrom, Vto;
    if(charge) {
        //safety routine - important when one cell is overcharged
        Vfrom = min(Vth, Vmax);
        Vto = max(Vth, Vmax);
    } else {
        Vfrom = max(Vth, Vmax);
        Vto = min(Vth, Vmax);
    }
    VLast_ = Vth_ = Vfrom;
    ILastDiff_ = ILast_ = 0;

    Rth.uI = i;
    Rth.iV = Vto;  Rth.iV -= Vfrom;
}

AnalogInputs::ValueType Thevenin::calculateI(AnalogInputs::ValueType v) const
{
    int32_t i;
    i  = v;
    i -= Vth_;
    i *= Rth.uI;
    i /= Rth.iV;
    if(i >  UINT16_MAX) return  UINT16_MAX;
    if(i < 0) return 0;
    return i;
}

void Thevenin::calculateRthVth(AnalogInputs::ValueType v, AnalogInputs::ValueType i)
{
    calculateRth(v, i);
    calculateVth(v, i);
}

void Thevenin::calculateRth(AnalogInputs::ValueType v, AnalogInputs::ValueType i)
{
    if(absDiff(i, ILast_) > ILastDiff_/2 && absDiff(v, VLast_) > 0) {
        int16_t rth_v;
        uint16_t rth_i;
        if(i > ILast_) {
            rth_i  = i;
            rth_i -= ILast_;
            rth_v  = v;
            rth_v -= VLast_;
        } else {
            rth_v  = VLast_;
            rth_v -= v;
            rth_i  = ILast_;
            rth_i  -= i;
        }
        if(sign(rth_v) == sign(Rth.iV)) {
            ILastDiff_ = rth_i;
            Rth.iV = rth_v;
            Rth.uI = rth_i;
        }
    }
}

void Thevenin::calculateVth(AnalogInputs::ValueType v, AnalogInputs::ValueType i)
{
    int32_t VRth;
    VRth = i;
    VRth *= Rth.iV;
    VRth /= Rth.uI;
    if(v < VRth) Vth_ = 0;
    else Vth_ = v - VRth;
}

