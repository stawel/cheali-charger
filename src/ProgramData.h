#ifndef PROGRAMDATA_H_
#define PROGRAMDATA_H_

#include <inttypes.h>
#include "AnalogInputs.h"
#include "Hardware.h"


#define MAX_PROGRAMS 32

struct ProgramData {
	enum BatteryType {Unknown, NiCd, NiMH, Pb, Life, Lilo, Lipo, 				LAST_BATTERY_TYPE};
	enum VoltageType {VIdle,VCharge,VDischarge,VStorage, 						LAST_VOLTAGE_TYPE};

	uint8_t batteryType;
	uint16_t C,Ic,Id,cells;

	uint16_t getVoltagePerCell(VoltageType type = VIdle) const;
	uint16_t getVoltage(VoltageType type = VIdle) const;

	bool edit();

	uint8_t printBatteryString(int n = LCD_COLUMNS) const;

	uint8_t printVoltageString() const;
	uint8_t printIcString() const;
	uint8_t printIdString() const;
	uint8_t printChargeString() const;

	void changeBattery(int direction);
	void changeVoltage(int direction);
	void changeCharge(int direction);
	void changeIc(int direction);
	void changeId(int direction);

	uint16_t getMaxCells() const;
	uint16_t getMaxIc() const;
	uint16_t getMaxId() const;

	void check();
	void loadDefault();

	bool isLiXX() const { return batteryType == Life || batteryType == Lilo || batteryType == Lipo;};

	static void loadProgramData(int index);
	static void saveProgramData(int index);
	static void restoreDefault();

	static ProgramData currentProgramData;
};

#endif /* PROGRAMDATA_H_ */
