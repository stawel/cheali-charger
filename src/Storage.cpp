#include "TheveninVtLimitCharge.h"
#include "SimpleDischarge.h"
#include "Hardware.h"
#include "Storage.h"
#include "TheveninCharge.h"

Storage storage;

void Storage::powerOff()
{
	theveninCharge.powerOff();
	simpleDischarge.powerOff();
	balancer.powerOff();
}

void Storage::powerOn()
{
	balancer.powerOn();
	if(balancer.isMinVout(balancer.calculatePerCell(V_))) {
		theveninCharge.powerOn();
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
			status = theveninCharge.doStrategy();
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

	if(status == COMPLETE && doBalance_) {
		status = RUNNING;
		state = Balance;
	}

	return status;
}

void Storage::setVI(AnalogInputs::ValueType V, AnalogInputs::ValueType I)
{
	V_ = V;
	theveninCharge.setVI(V, I);
	simpleDischarge.setVI(V,I);
}

void Storage::setDoBalance(bool v)
{
	doBalance_ = v;
}

