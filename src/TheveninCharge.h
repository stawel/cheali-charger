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
	virtual void powerOn(SMPS &s);
	virtual void doStrategy(SMPS &);
	void setI(SMPS &s, double i);
	bool isSmallSmpsValue(SMPS &s);
	void correctSmpsValue(SMPS &s);
//	bool isBalancer() const;
};

#endif /* THEVENINCHARGE_H_ */
