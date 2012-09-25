#include "Hardware.h"
#include "ProgramData.h"
#include "Screen.h"
#include "TheveninDischarge.h"


TheveninDischarge theveninDischarge;

void TheveninDischarge::powerOff()
{
	discharger.powerOff();
}


void TheveninDischarge::powerOn()
{
	discharger.powerOn();
	balancer.powerOn();
	analogInputs.resetStable();
	theveninMethod.init();
}

void TheveninDischarge::setVI(AnalogInputs::ValueType v, AnalogInputs::ValueType i)
{
	SimpleDischarge::setVI(v,i);
	theveninMethod.setVI(v, analogInputs.reverseCalibrateValue(AnalogInputs::IdischargeValue, i));
	setMinI(i/10);
}
void TheveninDischarge::setMinI(AnalogInputs::ValueType i)
{
	theveninMethod.setMinI(analogInputs.reverseCalibrateValue(AnalogInputs::IdischargeValue, i));
}

Strategy::statusType TheveninDischarge::doStrategy()
{
	bool stable;
	bool isEndVout = isMinVout();
	uint16_t oldValue = discharger.getValue();

	//when discharging near the end, the battery voltage is very unstable
	//but we need new discharge values at that point
	stable = isStable() || isEndVout;

	//test for charge complete
	if(theveninMethod.isComlete(isEndVout, oldValue)) {
		discharger.powerOff(Discharger::DISCHARGING_COMPLETE);
		return COMPLETE;
	}

	if(stable) {
		uint16_t value = theveninMethod.calculateNewValue(isEndVout, oldValue);
		if(value != oldValue) {
			discharger.setValue(value);
		}
	}
	return RUNNING;
}

