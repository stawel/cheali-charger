#ifndef CHARGINGSTRATEGY_H_
#define CHARGINGSTRATEGY_H_

#include <inttypes.h>

class SMPS;

class ChargingStrategy {
public:
	ChargingStrategy() {}
	virtual void powerOn(SMPS &){};
	virtual void doStrategy(SMPS &);
};


#endif /* CHARGINGSTRATEGY_H_ */
