#include "TheveninCharge.h"
#include "SMPS.h"
#include "Hardware.h"
#include "ProgramData.h"
#include "Screen.h"

void TheveninCharge::powerOn(SMPS &s)
{
	analogInputs.resetStable();
	t_.init(s.getVout());

	AnalogInputs::ValueType Amps10 = 10000;
	smpsVcorretion_ = analogInputs.reverseCalibrateValue(AnalogInputs::IsmpsValue,Amps10);
	smpsVcorretion_ /= Amps10;

	minICharge_ = ProgramData::currentProgramData.I/10;
	s.setRealValue(minICharge_);
	smallestSmpsValue_ = s.getValue();

	balancer.powerOn();
}


void TheveninCharge::doStrategy(SMPS &s)
{
	bool stable = isStable(s);
	screens.Rth_ = t_.Rth_*1000000;
	screens.Vth_ = t_.Vth_;

	if(stable) {
		AnalogInputs::ValueType Vc = ProgramData::currentProgramData.getVoltage(ProgramData::VCharge);

		if(s.getIout() <= minICharge_ && analogInputs.getStableCount(s.Vm_) > 10 ) {
			if(Vc <= s.getVout() || balancer.isCharged()) {
				s.powerOff(SMPS::CHARGING_COMPLETE);
				return;
			}
		}

		t_.calculateRthVth(s.getVout(),s.getIout());
		balancer.calculateRthVth(s.getIout());

		correctSmpsValue(s);
		double i;
		i = min(t_.calculateI(Vc), balancer.calculateI());

		setI(s, i);
	}
}

void TheveninCharge::correctSmpsValue(SMPS &s)
{
	if(Imax_) {
		smpsVcorretion_  = s.getValue();
		smpsVcorretion_ /= s.getIout();
	}
}


void TheveninCharge::setI(SMPS &s, double i)
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

	if(s.getValue() != value) {
		t_.storeLast(s.getVout(), s.getIout());
		analogInputs.resetStable();
		s.setValue(value);

		balancer.storeLast(s.getIout());
	}
}

bool TheveninCharge::isSmallSmpsValue(SMPS &s)
{
	return s.getValue() <= smallestSmpsValue_;
}

/*bool TheveninCharge::isBalancer() const
{
	return ProgramData::currentProgramData.balanceType == ProgramData::BEnable;
}*/


