#ifndef THEVENINDISCHARGE_H_
#define THEVENINDISCHARGE_H_

#include "SimpleDischarge.h"
#include "Thevenin.h"

class TheveninDischarge : public SimpleDischarge {
public:
	uint16_t minDischargerValue_;
	uint16_t maxDischargerValue_;

	Thevenin t_;
	bool Ifalling_;
	uint8_t fullCount_;

	TheveninDischarge(){}
	virtual void powerOn();
	virtual statusType doStrategy();
	virtual void powerOff();
	void setI(double i);
	bool isMinDischargerValue() const;
	bool isMinVout() const;
//	bool isBalancer() const;

	void setVI(AnalogInputs::ValueType v, AnalogInputs::ValueType i);
	void setMinI(AnalogInputs::ValueType i);

};

extern TheveninDischarge theveninDischarge;


#endif /* THEVENINDISCHARGE_H_ */
