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
#include "Thevenin.h"
#include "Utils.h"


AnalogInputs::ValueType Resistance::getReadableRth()
{
    if(I_ == 0)
        return 0;
    uint32_t R = abs(V_);
    R*=1000;
    R/=abs(I_);
    return R;
}

AnalogInputs::ValueType Resistance::getReadableRth_calibrateI(AnalogInputs::Name name)
{
    Resistance R;
    R.I_ = AnalogInputs::calibrateValue(name, abs(I_));
    R.V_ = V_;
    return R.getReadableRth();
}



void Thevenin::init(AnalogInputs::ValueType Vth,AnalogInputs::ValueType Vmax, AnalogInputs::ValueType i)
{
    VLast_ = Vth_ = Vth;
    ILastDiff_ = ILast_ = 0;

    Rth_.I_ = i;
    Rth_.V_ = Vmax;  Rth_.V_ -= Vth;
}

AnalogInputs::ValueType Thevenin::calculateI(AnalogInputs::ValueType v) const
{
    int32_t i;
    i  = v;
    i -= Vth_;
    i *= Rth_.I_;
    i /= Rth_.V_;
    //TODO:
    if(i >  65000) return  65000;  //need change pumpcapacitor my charger?
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
        int16_t rth_v,rth_i;
        rth_v  = v;
        rth_v -= VLast_;
        rth_i  = i;
        rth_i -= ILast_;
        if(sign(rth_v)*sign(rth_i) == sign(Rth_.V_)*sign(Rth_.I_)) {
            ILastDiff_ = absDiff(i, ILast_);
            Rth_.V_ = rth_v;
            Rth_.I_ = rth_i;
        }
    }
}

void Thevenin::calculateVth(AnalogInputs::ValueType v, AnalogInputs::ValueType i)
{
    int32_t VRth;
    VRth = i;
    VRth *= Rth_.V_;
    VRth /= Rth_.I_;
    if(v < VRth) Vth_ = 0;
    else Vth_ = v - VRth;
}

