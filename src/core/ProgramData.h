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


#define MAX_PROGRAMS 32
#define PROGRAM_DATA_MAX_NAME 14
#define PROGRAM_DATA_MAX_CHARGE ANALOG_CHARGE(65.000)

struct ProgramData {

    enum BatteryType {Unknown, NiCd, NiMH, Pb, Life, Lilo, Lipo, Li430, Li435, NiZn,    LAST_BATTERY_TYPE};
    enum VoltageType {VIdle,VCharge,VDischarge,VStorage, VUpperLimit, ValidEmpty,       LAST_VOLTAGE_TYPE};


    struct BatteryData {
        uint8_t type;

        uint16_t C,Ic,Id,cells;
#ifdef ENABLE_TIME_LIMIT  
        uint16_t Time;
#endif
    } __attribute__((packed));


    BatteryData battery;
    char name[PROGRAM_DATA_MAX_NAME];

    uint16_t getVoltagePerCell(VoltageType type = VIdle) const;
    uint16_t getVoltage(VoltageType type = VIdle) const;
    uint16_t getCapacityLimit() const;
#ifdef ENABLE_TIME_LIMIT    
    uint16_t getTimeLimit() const;
    uint8_t printTimeString() const;
#endif
    int16_t getDeltaVLimit() const;
    int16_t getDeltaTLimit() const;

    void edit(int index);
 
    void createName(int index);

    void resetName(int index);

    uint8_t printBatteryString(int n) const;
    uint8_t printBatteryString() const;

    uint8_t printVoltageString() const;
    uint8_t printIcString() const;
    uint8_t printIdString() const;
    uint8_t printChargeString() const;

    void changeBattery(int direction);
    void changeVoltage(int direction);
    void changeCharge(int direction);
    void changeIc(int direction);
    void changeId(int direction);
#ifdef ENABLE_TIME_LIMIT  
    void changeTime(int direction);
#endif

    uint16_t getMaxCells() const;
    uint16_t getMaxIc() const;
    uint16_t getMaxId() const;

    void check();
    void loadDefault();

    bool isLiXX() const { return battery.type == Life || battery.type == Lilo || battery.type == Lipo || battery.type == Li430 ||battery.type == Li435 || battery.type == NiZn; };
    bool isNiXX() const { return battery.type == NiCd || battery.type == NiMH; };
    bool isPb() const { return battery.type == Pb; };

    static void loadProgramData(int index);
    static void saveProgramData(int index);
    static char * getName_E(int index);
    static void restoreDefault();

    static ProgramData currentProgramData;

private:
    static void printIndex(char * &buf, uint8_t &maxSize, uint8_t index);
};

#endif /* PROGRAMDATA_H_ */
