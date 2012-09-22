#ifndef THEVENINCHARGE_H_
#define THEVENINCHARGE_H_

#include "SimpleCharge.h"
#include "Thevenin.h"

class TheveninCharge : public SimpleCharge {
public:
	Thevenin t_;
	uint16_t minSmpsValue_;
	uint16_t maxSmpsValue_;

	bool Ifalling_;
	uint16_t minChargeC_;
	uint8_t fullCount_;

	TheveninCharge(){}
	virtual void powerOn();
	virtual statusType doStrategy();
	virtual void powerOff();
	void setI(double i);
	bool isMinSmpsValue() const;
	bool isMaxVout() const;
//	bool isBalancer() const;

	void setMinChargeC(uint16_t v = 10);
};

extern TheveninCharge theveninCharge;


#endif /* THEVENINCHARGE_H_ */
