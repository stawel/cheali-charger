#ifndef THEVENIN_H_
#define THEVENIN_H_

#include "AnalogInputs.h"

class Thevenin {
public:

	AnalogInputs::ValueType Vth_;
	int16_t Rth_V_,Rth_I_;

	AnalogInputs::ValueType VLast_;
	AnalogInputs::ValueType ILast_;
	AnalogInputs::ValueType ILastDiff_;

	Thevenin(){};
	void storeLast(AnalogInputs::ValueType VLast, AnalogInputs::ValueType ILast) { VLast_ = VLast; ILast_ = ILast; }

	void calculateRthVth(AnalogInputs::ValueType v, AnalogInputs::ValueType i);
	void calculateRth(AnalogInputs::ValueType v, AnalogInputs::ValueType i);
	void calculateVth(AnalogInputs::ValueType v, AnalogInputs::ValueType i);
	AnalogInputs::ValueType calculateI(AnalogInputs::ValueType Vc) const;

	void init(AnalogInputs::ValueType Vth,AnalogInputs::ValueType Vmax, AnalogInputs::ValueType i);
};

#endif /* THEVENIN_H_ */
