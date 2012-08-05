#ifndef THEVENIN_H_
#define THEVENIN_H_

#include "AnalogInputs.h"

class Thevenin {
public:
	double Vth_;
	double Rth_;

	AnalogInputs::ValueType VLast_;
	AnalogInputs::ValueType ILast_;
	AnalogInputs::ValueType ILastDiff_;


	Thevenin(){};
	void storeLast(AnalogInputs::ValueType VLast, AnalogInputs::ValueType ILast);
	void calculateRthVth(AnalogInputs::ValueType v, AnalogInputs::ValueType i);
	void calculateRth(AnalogInputs::ValueType v, AnalogInputs::ValueType i);
	void calculateVth(AnalogInputs::ValueType v, AnalogInputs::ValueType i);
	double calculateI(AnalogInputs::ValueType Vc) const;

	void init(AnalogInputs::ValueType Vth);
};

#endif /* THEVENIN_H_ */
