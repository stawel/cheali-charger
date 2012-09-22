#include "Hardware.h"
#include "TheveninCharge.h"
#include "TheveninVtLimitCharge.h"

//TheveninVtLimitCharge theveninVtLimitCharge;

void TheveninVtLimitCharge::powerOff()
{
	theveninCharge.powerOff();
}

void TheveninVtLimitCharge::powerOn()
{
	theveninCharge.powerOn();
}


ChargingStrategy::statusType TheveninVtLimitCharge::doStrategy()
{
	ChargingStrategy::statusType status = theveninCharge.doStrategy();
	if(status == ChargingStrategy::RUNNING) {
		uint8_t cells = balancer.getCells();
		bool reached = true;
		if(cells > 0) {
			AnalogInputs::ValueType VtLimitPerCell = VtLimit_ / cells;
			for(uint8_t i = 0;i < cells; i++) {
				if(balancer.t_[i].Vth_ < VtLimitPerCell)
					reached = false;
			}
		} else {
			if(theveninCharge.t_.Vth_ < VtLimit_)
				reached = false;
		}
		if(reached) {
			powerOff();
			return ChargingStrategy::COMPLETE;
		}
	}
	return status;
}


