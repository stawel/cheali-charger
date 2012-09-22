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

void TheveninCharge::powerOn()
{
	smps.powerOn();
	analogInputs.resetStable();
	t_.init(smps.getVout());

	maxSmpsValue_ = analogInputs.reverseCalibrateValue(AnalogInputs::IsmpsValue, ProgramData::currentProgramData.I);
	minSmpsValue_ = analogInputs.reverseCalibrateValue(AnalogInputs::IsmpsValue, ProgramData::currentProgramData.I / minChargeC_);
	smps.setValue(minSmpsValue_);

	Ifalling_ = false;

	balancer.powerOn();
}


ChargingStrategy::statusType TheveninCharge::doStrategy()
{
	bool stable = isStable();
	double i;
	bool ismaxVout;
	screens.Rth_ = t_.Rth_*1000000;
	screens.Vth_ = t_.Vth_;

	if(stable) {
		//test for maximum output voltage reached
		ismaxVout = isMaxVout();
		if(ismaxVout) {
			Ifalling_ = true;
		}

		//test for charge complete
		if(isMinSmpsValue()
			&& analogInputs.getStableCount(smps.VName) > 10
			&& ismaxVout) {
				smps.powerOff(SMPS::CHARGING_COMPLETE);
				return COMPLETE;
			}

		//calculate new  output current "i"
		t_.calculateRthVth(smps.getVout(),smps.getValue());
		balancer.calculateRthVth(smps.getValue());

		AnalogInputs::ValueType Vc 			= ProgramData::currentProgramData.getVoltage(ProgramData::VCharge);
		AnalogInputs::ValueType Vc_per_cell = ProgramData::currentProgramData.getVoltagePerCell(ProgramData::VCharge);
		i = min(t_.calculateI(Vc), balancer.calculateI(Vc_per_cell));

		setI(i);
	}
	return RUNNING;
}

void TheveninCharge::setI(double i)
{
	screens.I_ = i;
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
	AnalogInputs::ValueType Vc = ProgramData::currentProgramData.getVoltage(ProgramData::VCharge);
	AnalogInputs::ValueType Vc_per_cell = ProgramData::currentProgramData.getVoltagePerCell(ProgramData::VCharge);

	return Vc <= smps.getVout() || balancer.isMaxVout(Vc_per_cell);
}

void TheveninCharge::setMinChargeC(uint16_t v)
{
	minChargeC_ = v;
}


