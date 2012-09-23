#ifndef STRATEGY_H_
#define STRATEGY_H_

#include <inttypes.h>

class Strategy {
public:
	enum statusType {ERROR, COMPLETE, COMPLETE_AND_EXIT, RUNNING };
	Strategy() {}
	virtual void powerOn(){};
	virtual statusType doStrategy(){ return RUNNING;};
	virtual void powerOff(){};
};


#endif /* STRATEGY_H_ */
