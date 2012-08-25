#include <Arduino.h>
#include "ProgramData.h"
#include "Balancer.h"
#include "Screen.h"

void Balancer::powerOn()
{
	cells_ = analogInputs.getRealValue(AnalogInputs::VbalanceInfo);
	for(int i = 0; i < cells_; i++) {
		t_[i].init(getV(i));
		Voff_[i] = Von_[i] = 0;
	}
	balance_ = 0;
	on_ = true;
	setBalance(0);
	startSwitchTime_ = startSwitchTime_ = 0;
}

uint8_t Balancer::getMin() const
{
	uint8_t c = 0;
	AnalogInputs::ValueType vmin = 65535;
	for(uint8_t i = 0; i < cells_; i++) {
		AnalogInputs::ValueType v = getV(i);
		if(vmin > v) {
			c = i;
			vmin = v;
		}
	}
	return c;
}


AnalogInputs::ValueType Balancer::getV(uint8_t cell)
{
	return analogInputs.getRealValue(AnalogInputs::Name(AnalogInputs::Vb0+cell));
}



void Balancer::powerOff()
{
	setBalance(0);
	on_ = false;
}

Balancer::Balancer()
{
	pinMode(BALANCER0_LOAD_PIN, OUTPUT);
	pinMode(BALANCER1_LOAD_PIN, OUTPUT);
	pinMode(BALANCER2_LOAD_PIN, OUTPUT);
	pinMode(BALANCER3_LOAD_PIN, OUTPUT);
	pinMode(BALANCER4_LOAD_PIN, OUTPUT);
	pinMode(BALANCER5_LOAD_PIN, OUTPUT);
}

void Balancer::setBalance(uint8_t port, bool v)
{
	if(on_)
		digitalWrite(port, v);
}

void Balancer::setBalance(uint16_t v)
{
//	if(timer.getMiliseconds() % 1024 > 900)
	//	v = 0;
	balance_ = v;
	startSwitchTime_ = timer.getMiliseconds();
	analogInputs.resetStable();

	setBalance(BALANCER0_LOAD_PIN, v&1);
	setBalance(BALANCER1_LOAD_PIN, v&2);
	setBalance(BALANCER2_LOAD_PIN, v&4);
	setBalance(BALANCER3_LOAD_PIN, v&8);
	setBalance(BALANCER4_LOAD_PIN, v&16);
	setBalance(BALANCER5_LOAD_PIN, v&32);
}

void Balancer::startBalacing()
{
	//TODO: should be an assert
	if(balance_ != 0 || !on_)
		return;

	minCell_ = getMin();

	AnalogInputs::ValueType vmin = getV(minCell_);

	bool off = true;
	for(int i = 0; i < cells_; i++) {
		Von_[i] = Voff_[i] = getV(i) - vmin;
		if(Von_[i] > error)
			off = false;
	}

	savedVon_ = false;
	startBalanceTime_ = timer.getMiliseconds();
	if(off) {
		powerOff();
	} else {
		setBalance(calculateBalance());
	}
}

uint16_t Balancer::calculateBalance()
{
	int16_t vmin = getV(minCell_);
	uint16_t retu = 0, b = 1;
	for(uint8_t c = 0; c < cells_; c++) {
		int16_t v = getV(c);
		if(b & balance_) {
			// if ON
			if(v - vmin > Von_[c] - Voff_[c])
				retu |=b;
		} else {
			// if OFF
			if(v - vmin > Von_[c] - Voff_[c] + (error+1)/2)
				retu |=b;
		}
		b<<=1;
	}
	return retu;
}

bool Balancer::isStable() const
{
	for(uint8_t c = 0; c < cells_; c++) {
		if(!analogInputs.isStable(AnalogInputs::Name(AnalogInputs::Vb0+c)))
			return false;
	}
	return true;
}

void Balancer::trySaveVon() {
	if(savedVon_)
		return;
	savedVon_ = true;
	AnalogInputs::ValueType vmin = getV(minCell_);
	for(uint8_t c = 0; c < cells_; c++) {
		Von_[c] = getV(c) - vmin;
	}
}

uint32_t Balancer::balanceTime() const
{
	return timer.getMiliseconds() - startBalanceTime_;
}


ChargingStrategy::statusType Balancer::doStrategy()
{
	if(!on_)
		return COMPLETE;
	if(isStable()) {
		if(balance_ == 0) {
			startBalacing();
		} else {
			trySaveVon();
			uint16_t balance = calculateBalance();
			if(balance_ != balance || balanceTime() > maxBalanceTime) {
				setBalance(0);
			}
		}
	}
	return RUNNING;
}

bool Balancer::isPowerOn() const
{
	return on_;
}

void Balancer::calculateRthVth(AnalogInputs::ValueType i)
{
	calculateRth(i);
	calculateVth(i);
}

void Balancer::calculateRth(AnalogInputs::ValueType i)
{
	for(uint8_t c = 0; c < cells_; c++) {
		t_[c].calculateRth(getV(c),i);
	}
}

void Balancer::storeLast(AnalogInputs::ValueType i)
{
	for(uint8_t c = 0; c < cells_; c++) {
		t_[c].storeLast(getV(c), i);
	}
}


void Balancer::calculateVth(AnalogInputs::ValueType i)
{
	for(uint8_t c = 0; c < cells_; c++) {
		t_[c].calculateVth(getV(c),i);
	}
}

double Balancer::calculateI() const
{
	double i = MAX_CHARGE_I;
	for(uint8_t c = 0; c < cells_; c++) {
		i = min(i, t_[c].calculateI(ProgramData::currentProgramData.getVoltagePerCell(ProgramData::VCharge)));
	}
	return i;
}

bool Balancer::isMaxVout() const
{
	for(uint8_t c = 0; c < cells_; c++) {
		if(getV(c) >= ProgramData::currentProgramData.getVoltagePerCell(ProgramData::VCharge))
			return true;
	}
	return false;
}


