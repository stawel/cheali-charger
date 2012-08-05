#include "Thevenin.h"


void Thevenin::init(AnalogInputs::ValueType Vth)
{
	VLast_ = Vth_ = Vth;
	ILastDiff_ = ILast_ = 0;
	//TODO: ?
	Rth_ = 0.0030;
}

double Thevenin::calculateI(AnalogInputs::ValueType Vc) const
{
	double i = 0;
	if(Vc > Vth_) i = Vc-Vth_;
	i /= Rth_;
	return i;
}

void Thevenin::calculateRthVth(AnalogInputs::ValueType v, AnalogInputs::ValueType i)
{
	calculateRth(v, i);
	calculateVth(v, i);
}


void Thevenin::calculateRth(AnalogInputs::ValueType v, AnalogInputs::ValueType i)
{
	if(absDiff(i, ILast_) > ILastDiff_) {
		ILastDiff_ = absDiff(i, ILast_);
		double R,I;
		R= v;
		R-= VLast_;
		I = i;
		I-=ILast_;
		R /= I;
		//TODO: ??
		if(R>0.000001) Rth_ = R;
	}
}

void Thevenin::calculateVth(AnalogInputs::ValueType v, AnalogInputs::ValueType i)
{
	double VRth;
	VRth = Rth_ * i;
	if(v < VRth) Vth_ = 0;
	else Vth_ = v - VRth;
}

void Thevenin::storeLast(AnalogInputs::ValueType VLast, AnalogInputs::ValueType ILast)
{
	VLast_ = VLast;
	ILast_ = ILast;
}
