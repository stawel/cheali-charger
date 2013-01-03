#include "Thevenin.h"
#include "Utils.h"

void Thevenin::init(AnalogInputs::ValueType Vth,AnalogInputs::ValueType Vmax, AnalogInputs::ValueType i)
{
    VLast_ = Vth_ = Vth;
    ILastDiff_ = ILast_ = 0;

    Rth_I_ = i;
    Rth_V_ = Vmax;  Rth_V_ -= Vth;
}

AnalogInputs::ValueType Thevenin::calculateI(AnalogInputs::ValueType v) const
{
    int32_t i;
    i  = v;
    i -= Vth_;
    i *= Rth_I_;
    i /= Rth_V_;
    //TODO:
    if(i >  32000) return  32000;
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
    if(absDiff(i, ILast_) > ILastDiff_ && absDiff(v, VLast_) > 0) {
        int16_t rth_v,rth_i;
        rth_v  = v;
        rth_v -= VLast_;
        rth_i  = i;
        rth_i -= ILast_;
        if(sign(rth_v)*sign(rth_i) == sign(Rth_V_)*sign(Rth_I_)) {
            ILastDiff_ = absDiff(i, ILast_);
            Rth_V_ = rth_v;
            Rth_I_ = rth_i;
        }
    }
}

void Thevenin::calculateVth(AnalogInputs::ValueType v, AnalogInputs::ValueType i)
{
    int32_t VRth;
    VRth = i;
    VRth *= Rth_V_;
    VRth /= Rth_I_;
    if(v < VRth) Vth_ = 0;
    else Vth_ = v - VRth;
}

