#ifndef THEVENIN_H_
#define THEVENIN_H_

#include "AnalogInputs.h"

class Thevenin {
public:
	typedef int16_t CurrentType;
	double Vth_;
	double Rth_;

	AnalogInputs::ValueType VLast_;
	CurrentType ILast_;
	CurrentType ILastDiff_;


	Thevenin(){};
	void storeLast(AnalogInputs::ValueType VLast, CurrentType ILast);
	void calculateRthVth(AnalogInputs::ValueType v, CurrentType i);
	void calculateRth(AnalogInputs::ValueType v, CurrentType i);
	void calculateVth(AnalogInputs::ValueType v, CurrentType i);
	double calculateI(AnalogInputs::ValueType Vc) const;

	void init(AnalogInputs::ValueType Vth);
};

#endif /* THEVENIN_H_ */
