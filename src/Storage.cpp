#include "TheveninVtLimitCharge.h"
#include "SimpleDischarge.h"
#include "Hardware.h"
#include "Storage.h"

Storage storage;

void Storage::powerOff()
{
	theveninVtLimitCharge.powerOff();
	simpleDischarge.powerOff();
	balancer.powerOff();
}

void Storage::powerOn()
{
	balancer.powerOn();
	if(balancer.isMinVout(balancer.calculatePerCell(V_))) {
		theveninVtLimitCharge.powerOn();
		state = Charge;
	} else {
		simpleDischarge.powerOn();
		state = Discharge;
	}
}


ChargingStrategy::statusType Storage::doStrategy()
{
	ChargingStrategy::statusType status;
	switch(state) {
		case Charge:
			status = theveninVtLimitCharge.doStrategy();
			break;
		case Discharge:
			status = simpleDischarge.doStrategy();
			break;
		case Balance:
			status = balancer.doStrategy();
			if(status != RUNNING) {
				powerOff();
				return status;
			}
			break;
	}

	if(status == COMPLETE) {
		status = RUNNING;
		state = Balance;
	}

	return status;
}

void Storage::setVI(AnalogInputs::ValueType V, AnalogInputs::ValueType I)
{
	V_ = V;
	theveninVtLimitCharge.setVtLimit(V);
	simpleDischarge.setVI(V,I);
}

