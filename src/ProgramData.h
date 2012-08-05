#ifndef PROGRAMDATA_H_
#define PROGRAMDATA_H_

#include <inttypes.h>
#include "AnalogInputs.h"
#include "Hardware.h"


#define MAX_PROGRAMS 16

struct ProgramData {
	enum BatteryType {Unknown, NiCd, NiMH, Pb, Life, Lilo, Lipo, 				LAST_BATTERY_TYPE};
	enum ProgramType { Idle, Charge, Discharge, Storage, Balance, FastCharge, 	LAST_PROGRAM_TYPE};
	enum VoltageType {VIdle,VCharge,VDischarge,VStorage, 						LAST_VOLTAGE_TYPE};
	enum BalanceType {BDisable, BAuto, BEnable, 								LAST_BALANCE_TYPE};
	enum VoltageInput {Vout, VAuto, Vbalacer, VoutMux,							LAST_VOLTAGE_INPUT};
	enum CurrentInput {Ismps, IsmpsValue,										LAST_CURRENT_INPUT};

	uint8_t batteryType;
	uint8_t programType;
	uint8_t balanceType;
	uint8_t voltageInput;
	uint8_t currentInput;

	uint16_t C,I,cells;
	uint16_t getVoltagePerCell(VoltageType type = VIdle) const;
	uint16_t getVoltage(VoltageType type = VIdle) const { return cells * getVoltagePerCell(type); }

	bool edit();

	void printProgramString() const;
	void printBatteryString(int n = LCD_COLUMNS) const;
	void printBalanceString() const;
	void printVoltageInputString() const;
	void printCurrentInputString() const;

	void printVoltageString() const;
	void printCurrentString() const;
	void printChargeString() const;

	void changeProgram(int direction);
	void changeBattery(int direction);
	void changeBalance(int direction);
	void changeVoltageInput(int direction);
	void changeCurrentInput(int direction);
	void changeVoltage(int direction);
	void changeCharge(int direction);
	void changeCurrent(int direction);

	uint16_t getMaxCells() const;
	uint16_t getMaxCurrent() const;
	uint32_t getMaxPower() const;

	void check();
	void loadDefault();

	bool isLiXX() const { return batteryType == Life || batteryType == Lilo || batteryType == Lipo;};
	ProgramType getProgramType() const {return ProgramType(programType); }

	static void loadProgramData(int index);
	static void saveProgramData(int index);
	static void restoreDefault();

	void initSmps() const;
	AnalogInputs::Name getVm() const;
	AnalogInputs::Name getIm() const;

	static ProgramData currentProgramData;
};

#endif /* PROGRAMDATA_H_ */
