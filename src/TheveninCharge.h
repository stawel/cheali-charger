#ifndef THEVENINCHARGE_H_
#define THEVENINCHARGE_H_

#include "SimpleCharge.h"
#include "Thevenin.h"

class TheveninCharge : public SimpleCharge {
public:
	Thevenin t_;
	double smpsVcorretion_;
	uint16_t smallestSmpsValue_;

	bool Imax_;
	bool Ifalling_;

	TheveninCharge(){}
	virtual void powerOn();
	virtual statusType doStrategy();
	virtual void powerOff();
	void setI(double i);
	bool isSmallSmpsValue();
	void correctSmpsValue();
	bool isMaxVout() const;
//	bool isBalancer() const;
};

#endif /* THEVENINCHARGE_H_ */
