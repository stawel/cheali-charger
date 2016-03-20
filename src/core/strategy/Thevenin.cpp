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

AnalogInputs::ValueType Thevenin::getReadableRth(const Resistance * r)
{
    uint32_t R;
    if(r->uI == 0)
        return 0;
    R = abs(r->iV);
    R *= ANALOG_VOLT(1.0);
    R/=r->uI;
    return R;
}

void Thevenin::init(Data *d, AnalogInputs::ValueType Vth,AnalogInputs::ValueType Vmax, AnalogInputs::ValueType i, bool charge)
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
    d->VLast_ = d->Vth_ = Vfrom;
    d->ILastDiff_ = d->ILast_ = 0;

    d->Rth.uI = i;
    d->Rth.iV = Vto;  d->Rth.iV -= Vfrom;
}

AnalogInputs::ValueType Thevenin::calculateI(Data *d, AnalogInputs::ValueType v)
{
    int32_t i;
    i  = v;
    i -= d->Vth_;
    i *= d->Rth.uI;
    if(d->Rth.iV == 0) return UINT16_MAX;
    i /= d->Rth.iV;
    if(i >  UINT16_MAX) return  UINT16_MAX;
    if(i < 0) return 0;
    return i;
}

void Thevenin::calculateRthVth(Data *d, AnalogInputs::ValueType v, AnalogInputs::ValueType i)
{
    calculateRth(d, v, i);
    calculateVth(d, v, i);
}

void Thevenin::calculateRth(Data *d, AnalogInputs::ValueType v, AnalogInputs::ValueType i)
{
    if(absDiff(i, d->ILast_) > d->ILastDiff_/2) {
        int16_t rth_v;
        uint16_t rth_i;
        if(i > d->ILast_) {
            rth_i  = i;
            rth_i -= d->ILast_;
            rth_v  = v;
            rth_v -= d->VLast_;
        } else {
            rth_v  = d->VLast_;
            rth_v -= v;
            rth_i  = d->ILast_;
            rth_i  -= i;
        }
        if(sign(rth_v) == sign(d->Rth.iV)) {
        	d->ILastDiff_ = rth_i;
        	d->Rth.iV = rth_v;
        	d->Rth.uI = rth_i;
        }
    }
}

void Thevenin::calculateVth(Data *d, AnalogInputs::ValueType v, AnalogInputs::ValueType i)
{
    int32_t VRth;
    VRth = i;
    VRth *= d->Rth.iV;
    VRth /= d->Rth.uI;
    if(v < VRth) d->Vth_ = 0;
    else d->Vth_ = v - VRth;
}

