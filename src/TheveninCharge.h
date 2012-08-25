#ifndef THEVENINCHARGE_H_
#define THEVENINCHARGE_H_

#include "SimpleCharge.h"
#include "Thevenin.h"

class TheveninCharge : public SimpleCharge {
public:
	Thevenin t_;
	double smpsVcorretion_;
	uint16_t smallestSmpsValue_;

	double minICharge_;
	bool Imax_, Imin_;

	TheveninCharge(){}
	virtual void powerOn();
	virtual statusType doStrategy();
	virtual void powerOff();
	void setI(double i);
	bool isSmallSmpsValue();
	void correctSmpsValue();
//	bool isBalancer() const;
};

#endif /* THEVENINCHARGE_H_ */
