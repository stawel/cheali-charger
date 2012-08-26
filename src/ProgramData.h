#ifndef PROGRAMDATA_H_
#define PROGRAMDATA_H_

#include <inttypes.h>
#include "AnalogInputs.h"
#include "Hardware.h"


#define MAX_PROGRAMS 16

struct ProgramData {
	enum BatteryType {Unknown, NiCd, NiMH, Pb, Life, Lilo, Lipo, 				LAST_BATTERY_TYPE};
	enum VoltageType {VIdle,VCharge,VDischarge,VStorage, 						LAST_VOLTAGE_TYPE};

	uint8_t batteryType;
	uint16_t C,I,cells;

	uint16_t getVoltagePerCell(VoltageType type = VIdle) const;
	uint16_t getVoltage(VoltageType type = VIdle) const { return cells * getVoltagePerCell(type); }

	bool edit();

	void printBatteryString(int n = LCD_COLUMNS) const;

	void printVoltageString() const;
	void printCurrentString() const;
	void printChargeString() const;

	void changeBattery(int direction);
	void changeVoltage(int direction);
	void changeCharge(int direction);
	void changeCurrent(int direction);

	uint16_t getMaxCells() const;
	uint16_t getMaxCurrent() const;
	uint32_t getMaxPower() const;

	void check();
	void loadDefault();

	bool isLiXX() const { return batteryType == Life || batteryType == Lilo || batteryType == Lipo;};

	static void loadProgramData(int index);
	static void saveProgramData(int index);
	static void restoreDefault();

	static ProgramData currentProgramData;
};

#endif /* PROGRAMDATA_H_ */
