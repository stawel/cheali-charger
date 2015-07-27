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


#define MAX_PROGRAMS 20

namespace ProgramData {

    enum BatteryClass {ClassNiXX, ClassPb, ClassLiXX, ClassNiZn, ClassUnknown, ClassLED, LAST_BATTERY_CLASS};
    enum BatteryType {None, NiCd, NiMH, Pb, Life, Lilo, Lipo, Li430, Li435, NiZn, Unknown, LED, LAST_BATTERY_TYPE};
    enum VoltageType {VIdle, VCharge, VDischarge, VStorage, ValidEmpty, LAST_VOLTAGE_TYPE};


    struct Battery {
        uint16_t type;
        uint16_t capacity;
        uint16_t cells;

        uint16_t Ic;
        uint16_t Id;

        uint16_t Vc_per_cell;
        uint16_t Vd_per_cell;

        uint16_t minIc;
        uint16_t minId;

        uint16_t time;

        uint16_t enable_externT;
        AnalogInputs::ValueType externTCO;

        uint16_t enable_adaptiveDischarge;
        uint16_t DCRestTime;
        uint16_t capCutoff;

        union {
            struct { //LiXX
                uint16_t Vs_per_cell; // storage
                uint16_t balancerError;
            };
            struct { //NiXX
                uint16_t enable_deltaV;
                int16_t deltaV;
                uint16_t deltaVIgnoreTime;
                uint16_t deltaT;
                uint16_t DCcycles;
            };
        };


    } CHEALI_EEPROM_PACKED;

    extern Battery battery;
    extern const char * const batteryString[];
    extern const BatteryClass batteryClassMap[];

    uint16_t getDefaultVoltagePerCell(VoltageType type = VIdle);
    uint16_t getDefaultVoltage(VoltageType type = VIdle);
    uint16_t getVoltage(VoltageType type = VIdle);
    uint16_t getCapacityLimit();
    inline uint16_t getTimeLimit() {return battery.time; }

    int16_t getDeltaVLimit();
    inline int16_t getDeltaTLimit() {return battery.deltaT;}

    uint16_t getMaxCells();
    uint16_t getMaxIc();
    uint16_t getMaxId();

    void check();

    void changedType();
    void changedCapacity();
    void changedIc();
    void changedId();

    BatteryClass getBatteryClass();

    inline bool isPowerSupply() { return getBatteryClass() == ClassLED; };
    inline bool isLiXX() { return getBatteryClass() == ClassLiXX; };
    inline bool isNiXX() { return getBatteryClass() == ClassNiXX; };
    inline bool isPb() { return getBatteryClass() == ClassPb; };

    void loadProgramData(uint8_t index);
    void saveProgramData(uint8_t index);

    //TODO: remove "print" methods
    void printProgramData(uint8_t index);

    void restoreDefault();
};

#endif /* PROGRAMDATA_H_ */
