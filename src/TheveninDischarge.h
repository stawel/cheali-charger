#ifndef THEVENINDISCHARGE_H_
#define THEVENINDISCHARGE_H_

#include "SimpleDischarge.h"
#include "TheveninMethod.h"

class TheveninDischarge : public SimpleDischarge {
public:
	TheveninDischarge(){}
	virtual void powerOn();
	virtual statusType doStrategy();
	virtual void powerOff();

	void setVI(AnalogInputs::ValueType v, AnalogInputs::ValueType i);
	void setMinI(AnalogInputs::ValueType i);

};

extern TheveninDischarge theveninDischarge;


#endif /* THEVENINDISCHARGE_H_ */
