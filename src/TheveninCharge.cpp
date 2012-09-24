#include "TheveninCharge.h"
#include "SMPS.h"
#include "Hardware.h"
#include "ProgramData.h"
#include "Screen.h"
#include "TheveninMethod.h"

TheveninCharge theveninCharge;

void TheveninCharge::powerOff()
{
	smps.powerOff();
}


void TheveninCharge::powerOn()
{
	smps.powerOn();
	balancer.powerOn();
	analogInputs.resetStable();
	theveninMethod.init();
}

void TheveninCharge::setVI(AnalogInputs::ValueType v, AnalogInputs::ValueType i)
{
       theveninMethod.setVI(v, analogInputs.reverseCalibrateValue(AnalogInputs::IsmpsValue, i));
       setMinI(i/10);
}
void TheveninCharge::setMinI(AnalogInputs::ValueType i)
{
       theveninMethod.setMinI(analogInputs.reverseCalibrateValue(AnalogInputs::IsmpsValue, i));
}

Strategy::statusType TheveninCharge::doStrategy()
{
	bool stable = isStable();
	bool ismaxVout = isEndVout();
	uint16_t oldValue = smps.getValue();

	//test for charge complete
	if(theveninMethod.isComlete(ismaxVout, oldValue)) {
		smps.powerOff(SMPS::CHARGING_COMPLETE);
		return COMPLETE;
	}

	if(stable) {
		uint16_t value = theveninMethod.calculateNewValue(ismaxVout, oldValue);
		if(value != oldValue)
			smps.setValue(value);
	}
	return RUNNING;
}


bool TheveninCharge::isEndVout() const
{
	AnalogInputs::ValueType Vc = theveninMethod.Vend_;
	AnalogInputs::ValueType Vc_per_cell = balancer.calculatePerCell(Vc);

	return Vc <= smps.getVout() || balancer.isMaxVout(Vc_per_cell);
}


