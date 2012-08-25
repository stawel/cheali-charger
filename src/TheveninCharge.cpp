#include "TheveninCharge.h"
#include "SMPS.h"
#include "Hardware.h"
#include "ProgramData.h"
#include "Screen.h"


void TheveninCharge::powerOff()
{
	smps.powerOff();
}

void TheveninCharge::powerOn()
{
	smps.powerOn();
	analogInputs.resetStable();
	t_.init(smps.getVout());

	AnalogInputs::ValueType Amps10 = 10000;
	smpsVcorretion_ = analogInputs.reverseCalibrateValue(AnalogInputs::IsmpsValue,Amps10);
	smpsVcorretion_ /= Amps10;

	AnalogInputs::ValueType minICharge = ProgramData::currentProgramData.I/10;
	smps.setRealValue(minICharge);
	smallestSmpsValue_ = smps.getValue();

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
		if(isSmallSmpsValue()
			&& analogInputs.getStableCount(smps.Vm_) > 10
			&& ismaxVout) {
				smps.powerOff(SMPS::CHARGING_COMPLETE);
				return COMPLETE;
			}

		//calculate new  output current "i"
		t_.calculateRthVth(smps.getVout(),smps.getIout());
		balancer.calculateRthVth(smps.getIout());

		correctSmpsValue();

		AnalogInputs::ValueType Vc = ProgramData::currentProgramData.getVoltage(ProgramData::VCharge);
		i = min(t_.calculateI(Vc), balancer.calculateI());

		setI(i);
	}
	return RUNNING;
}

void TheveninCharge::correctSmpsValue()
{
	if(Imax_) {
		smpsVcorretion_  = smps.getValue();
		smpsVcorretion_ /= smps.getIout();
	}
}


void TheveninCharge::setI(double i)
{
	screens.I_ = i;
	if(i > ProgramData::currentProgramData.I) {
		i = ProgramData::currentProgramData.I;
		Imax_ = true;
	} else {
		Imax_ = false;
	}

	uint16_t value = i * smpsVcorretion_;
	if(value < smallestSmpsValue_) {
		value = smallestSmpsValue_;
	}

	if((smps.getValue() != value && !Ifalling_) ||
			smps.getValue() > value) {

		t_.storeLast(smps.getVout(), smps.getIout());
		balancer.storeLast(smps.getIout());

		analogInputs.resetStable();
		smps.setValue(value);
	}
}

bool TheveninCharge::isSmallSmpsValue()
{
	return smps.getValue() <= smallestSmpsValue_;
}

bool TheveninCharge::isMaxVout() const
{
	AnalogInputs::ValueType Vc = ProgramData::currentProgramData.getVoltage(ProgramData::VCharge);
	return Vc <= smps.getVout() || balancer.isMaxVout();
}

/*bool TheveninCharge::isBalancer() const
{
	return ProgramData::currentProgramData.balanceType == ProgramData::BEnable;
}*/


