#ifndef BALANCER_H_
#define BALANCER_H_

#include "Hardware.h"
#include "Thevenin.h"
#include "ChargingStrategy.h"

class Balancer : public ChargingStrategy {
public:
	//TODO: "error" should be changeable
	const static AnalogInputs::ValueType error = 3;
	const static AnalogInputs::ValueType Ibalance = 200; //200mA
	const static uint32_t maxBalanceTime = 30000; //60s
	uint8_t cells_;
	uint8_t minCell_;
	uint16_t balance_;
	bool on_;
	Thevenin t_[MAX_BANANCE_CELLS];
	int16_t Von_[MAX_BANANCE_CELLS], Voff_[MAX_BANANCE_CELLS];
	bool savedVon_;
	uint32_t startBalanceTime_;
	uint32_t startSwitchTime_;

	uint32_t IVtime_;
	AnalogInputs::ValueType V_[MAX_BANANCE_CELLS];
	uint16_t stableCount_;


	Balancer();
	virtual void powerOn();
	virtual void powerOff();
	virtual statusType doStrategy();
	void startBalacing();
	void trySaveVon();
	uint32_t balanceTime() const;

	uint16_t calculateBalance();
	void setBalance(uint16_t v);
	void setBalance(uint8_t port, bool v);
	uint8_t getCellMinV() const;
	static AnalogInputs::ValueType getV(uint8_t cell);
	AnalogInputs::ValueType getPresumedV(uint8_t cell);
	bool isPowerOn() const;
	bool isMaxVout() const;
	bool isStable() const;

	//Thevenin methods
	double calculateI() const;
	void calculateRthVth(AnalogInputs::ValueType i);
	void calculateRth(AnalogInputs::ValueType i);
	void calculateVth(AnalogInputs::ValueType i);
	void storeLast(AnalogInputs::ValueType ILast);
};

#endif /* BALANCER_H_ */
