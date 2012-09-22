#ifndef THEVENINCHARGE_H_
#define THEVENINCHARGE_H_

#include "SimpleCharge.h"
#include "Thevenin.h"

class TheveninCharge : public SimpleCharge {
public:
	uint16_t minSmpsValue_;
	uint16_t maxSmpsValue_;
	AnalogInputs::ValueType maxV_;

	Thevenin t_;
	bool Ifalling_;
	uint8_t fullCount_;

	TheveninCharge(){}
	virtual void powerOn();
	virtual statusType doStrategy();
	virtual void powerOff();
	void setI(double i);
	bool isMinSmpsValue() const;
	bool isMaxVout() const;
//	bool isBalancer() const;

	void setVI(AnalogInputs::ValueType v, AnalogInputs::ValueType i);
	void setMinI(AnalogInputs::ValueType i);

};

extern TheveninCharge theveninCharge;


#endif /* THEVENINCHARGE_H_ */
