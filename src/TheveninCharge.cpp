#include "TheveninCharge.h"
#include "SMPS.h"
#include "Hardware.h"
#include "ProgramData.h"
#include "Screen.h"


TheveninCharge theveninCharge;

void TheveninCharge::powerOff()
{
	smps.powerOff();
}


void TheveninCharge::setVI(AnalogInputs::ValueType v, AnalogInputs::ValueType i)
{
	maxV_ = v;
	maxSmpsValue_ = analogInputs.reverseCalibrateValue(AnalogInputs::IsmpsValue, i);
	setMinI(i/10);
}
void TheveninCharge::setMinI(AnalogInputs::ValueType i)
{
	minSmpsValue_ = analogInputs.reverseCalibrateValue(AnalogInputs::IsmpsValue, i);
}



void TheveninCharge::powerOn()
{
	smps.powerOn();
	analogInputs.resetStable();
	t_.init(smps.getVout());

	smps.setValue(minSmpsValue_);

	Ifalling_ = false;
	fullCount_ = 0;

	balancer.powerOn();
}


ChargingStrategy::statusType TheveninCharge::doStrategy()
{
	bool stable = isStable();
	double i;
	bool ismaxVout;
	screens.Rth_ = t_.Rth_*1000000;
	screens.Vth_ = t_.Vth_;

	ismaxVout = isMaxVout();

	//test for charge complete
	if(isMinSmpsValue() && ismaxVout) {
		if(fullCount_++ >= 10) {
			smps.powerOff(SMPS::CHARGING_COMPLETE);
			return COMPLETE;
		}
	} else fullCount_ = 0;

	if(stable) {
		//test for maximum output voltage reached
		if(ismaxVout) {
			Ifalling_ = true;
		}

		//calculate new  output current "i"
		t_.calculateRthVth(smps.getVout(),smps.getValue());
		balancer.calculateRthVth(smps.getValue());

		AnalogInputs::ValueType Vc = maxV_;
		AnalogInputs::ValueType Vc_per_cell = balancer.calculatePerCell(maxV_);
		i = min(t_.calculateI(Vc), balancer.calculateMinI(Vc_per_cell));

		setI(i);
	}
	return RUNNING;
}

void TheveninCharge::setI(double i)
{
	screens.valueTh_ = i;
	if(i > maxSmpsValue_) {
		i = maxSmpsValue_;
	}
	if(i < minSmpsValue_) {
		i = minSmpsValue_;
	}
	AnalogInputs::ValueType value = i;

	if((smps.getValue() != value && !Ifalling_) ||
			smps.getValue() > value) {

		t_.storeLast(smps.getVout(), smps.getIout());
		balancer.storeLast(smps.getIout());

		analogInputs.resetStable();
		smps.setValue(value);
	}
}

bool TheveninCharge::isMinSmpsValue() const
{
	return smps.getValue() <= minSmpsValue_;
}

bool TheveninCharge::isMaxVout() const
{
	AnalogInputs::ValueType Vc = maxV_;
	AnalogInputs::ValueType Vc_per_cell = balancer.calculatePerCell(maxV_);

	return Vc <= smps.getVout() || balancer.isMaxVout(Vc_per_cell);
}


