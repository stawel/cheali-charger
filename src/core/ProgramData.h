/*
    cheali-charger - open source firmware for a variety of LiPo chargers
    Copyright (C) 2013  Paweł Stawicki. All right reserved.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef PROGRAMDATA_H_
#define PROGRAMDATA_H_

#include <inttypes.h>
#include "AnalogInputs.h"
#include "Hardware.h"
#include "Settings.h"


#define MAX_PROGRAMS 30
#define PROGRAM_DATA_MAX_NAME 14
#define PROGRAM_DATA_MAX_CHARGE ANALOG_CHARGE(65.000)
#define PROGRAM_DATA_MAX_TIME 1000 //infinity
#define PROGRAM_DATA_MIN_CHARGE ANALOG_CHARGE(0.100)

struct ProgramData {

    enum BatteryClass {ClassNiXX, ClassPb, ClassLiXX, ClassNiZn};
    enum BatteryType {Unknown, NiCd, NiMH, Pb, Life, Lilo, Lipo, Li430, Li435, NiZn, LAST_BATTERY_TYPE};
    enum VoltageType {VIdle, VCharge, VDischarge, VStorage, ValidEmpty, LAST_VOLTAGE_TYPE};


    struct BatteryData {
        uint16_t type;

        uint16_t C,Ic,Id,cells;

//#ifdef ENABLE_TIME_LIMIT: to ensure the same eeprom layout Time is always enabled
        uint16_t time;

    } CHEALI_EEPROM_PACKED;

    BatteryData battery;
    char name[PROGRAM_DATA_MAX_NAME];

    uint16_t getVoltagePerCell(VoltageType type = VIdle) const;
    uint16_t getVoltage(VoltageType type = VIdle) const;
    uint16_t getCapacityLimit() const;
#ifdef ENABLE_TIME_LIMIT
    uint16_t getTimeLimit() const {return battery.time; }
    void printTimeString() const;
    void changeTime(int direction);
#endif
    int16_t getDeltaVLimit() const;
    int16_t getDeltaTLimit() const {return settings.deltaT;}

    //Info: the print... and change... methods are used in ProgramDataMenu AND Screen
    void printBatteryString() const;

    void printVoltageString() const;
    void printIcString() const;
    void printIdString() const;
    void printChargeString() const;

    //TODO: change to changeBatteryType
    void changeBatteryType(int direction);
    void changeVoltage(int direction);
    void changeCharge(int direction);
    void changeIc(int direction);
    void changeId(int direction);

    uint16_t getMaxCells() const;
    uint16_t getMaxIc() const;
    uint16_t getMaxId() const;

    void check();
    void loadDefault();

    BatteryClass getBatteryClass() const;

    //TODO: remove
    bool isLiXX() const { return getBatteryClass() == ClassLiXX; };
    bool isNiXX() const { return getBatteryClass() == ClassNiXX; };
    bool isPb() const { return getBatteryClass() == ClassPb; };

    void edit(int index);

    void createName(int index);
    void resetName(int index);

    static void loadProgramData(int index);
    static void saveProgramData(int index);
    static char * getName_E(int index);
    static void restoreDefault();

    static ProgramData currentProgramData;

private:
    static void printIndex(char * &buf, uint8_t &maxSize, uint8_t index);
};

#endif /* PROGRAMDATA_H_ */
