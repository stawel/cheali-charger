#ifndef CHARGINGSTRATEGY_H_
#define CHARGINGSTRATEGY_H_

#include <inttypes.h>

class ChargingStrategy {
public:
	enum statusType {ERROR, COMPLETE, RUNNING};
	ChargingStrategy() {}
	virtual void powerOn(){};
	virtual statusType doStrategy(){ return RUNNING;};
	virtual void powerOff(){};
};


#endif /* CHARGINGSTRATEGY_H_ */
