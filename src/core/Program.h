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
#ifndef PROGRAM_H_
#define PROGRAM_H_

#include "ProgramData.h"
#include "Strategy.h"

namespace Program {

    enum ProgramType {
        ChargeLiXX, ChargeLiXX_Balance, Balance, DischargeLiXX, FastChargeLiXX, StorageLiXX, StorageLiXX_Balance,
        ChargeNiXX, DischargeNiXX,  DCcycleLiXX,  DCcycleNiXX,
        ChargePb, DischargePb, FastChargePb, DCcyclePb,
        EditBattery, LAST_PROGRAM_TYPE};

    enum ProgramState {
        None, Info, Calibration, Done, Error,
        Charging, ChargingBalancing, Discharging,DischargingCharging, Balancing, Storage,
    };

    extern ProgramType programType_;
    extern ProgramState programState_;
    extern const char * stopReason_;

    void selectProgram(int index);
    void run(ProgramType prog);

    void printStartInfo(ProgramType prog);
    bool startInfo();

    Strategy::statusType runStorage(bool balance);
    Strategy::statusType runTheveninCharge(int minChargeC);
    Strategy::statusType runDischarge();
    Strategy::statusType runNiXXDischarge();
    Strategy::statusType runTheveninChargeBalance();
    Strategy::statusType runBalance();
    Strategy::statusType runDeltaCharge();

    Strategy::statusType runDCRestTime();
};


#endif /* PROGRAM_H_ */
