#include "Hardware.h"
#include "ProgramData.h"
#include "Screen.h"
#include "TheveninDischarge.h"


TheveninDischarge theveninDischarge;

void TheveninDischarge::powerOff()
{
	discharger.powerOff();
}


void TheveninDischarge::setVI(AnalogInputs::ValueType v, AnalogInputs::ValueType i)
{
	V_ = v;
	maxDischargerValue_ = analogInputs.reverseCalibrateValue(AnalogInputs::IdischargeValue, i);
	setMinI(i/10);
}
void TheveninDischarge::setMinI(AnalogInputs::ValueType i)
{
	minDischargerValue_ = analogInputs.reverseCalibrateValue(AnalogInputs::IdischargeValue, i);
}



void TheveninDischarge::powerOn()
{
	discharger.powerOn();
	analogInputs.resetStable();
	t_.init(discharger.getVout());

	discharger.setValue(minDischargerValue_);

	Ifalling_ = false;
	fullCount_ = 0;

	balancer.powerOn();
}


ChargingStrategy::statusType TheveninDischarge::doStrategy()
{
	bool stable = true;
	double i;
	bool isEndVout;
	screens.Rth_ = t_.Rth_*1000000;
	screens.Vth_ = t_.Vth_;

	isEndVout = isMinVout();

	//test for charge complete
	if(isMinDischargerValue() && isEndVout) {
		if(fullCount_++ >= 10) {
			discharger.powerOff(Discharger::DISCHARGING_COMPLETE);
			return COMPLETE;
		}
	} else fullCount_ = 0;

	if(stable) {
		//test for maximum output voltage reached
		if(isEndVout) {
			Ifalling_ = true;
		}

		//calculate new  output current "i"
		t_.calculateRthVth(discharger.getVout(),-discharger.getValue());
		balancer.calculateRthVth(-discharger.getValue());

		AnalogInputs::ValueType Vc = V_;
		AnalogInputs::ValueType Vc_per_cell = balancer.calculatePerCell(Vc);
		i = min(-t_.calculateI(Vc), -balancer.calculateMaxI(Vc_per_cell));

		setI(i);
	}
	return RUNNING;
}

void TheveninDischarge::setI(double i)
{
	screens.valueTh_ = i;
	if(i > maxDischargerValue_) {
		i = maxDischargerValue_;
	}
	if(i < minDischargerValue_) {
		i = minDischargerValue_;
	}
	AnalogInputs::ValueType value = i;

	if((discharger.getValue() != value && !Ifalling_) ||
			discharger.getValue() > value) {

		t_.storeLast(discharger.getVout(), discharger.getIout());
		balancer.storeLast(discharger.getIout());

		analogInputs.resetStable();
		discharger.setValue(value);
	}
}

bool TheveninDischarge::isMinDischargerValue() const
{
	return discharger.getValue() <= minDischargerValue_;
}

bool TheveninDischarge::isMinVout() const
{
	AnalogInputs::ValueType Vc = V_;
	AnalogInputs::ValueType Vc_per_cell = balancer.calculatePerCell(Vc);

	return discharger.getVout() <= Vc || balancer.isMinVout(Vc_per_cell);
}


