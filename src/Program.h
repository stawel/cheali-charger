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

class Program {
public:
    enum ProgramType {
        ChargeLiXX, ChargeLiXX_Balance, Balance, DischargeLiXX, FastChargeLiXX, StorageLiXX, StorageLiXX_Balance,
        ChargeNiXX, DischargeNiXX, CDcycleNiXX,DCcycleNiXX,
        ChargePb, DischargePb,
        EditBattery, LAST_PROGRAM_TYPE};

    static AnalogInputs::Name iName_;
    static ProgramType programType_;
    static const char * stopReason_;

    static void selectProgram(int index);
    static void run(ProgramType prog);


    static void printStartInfo(ProgramType prog);
    static bool startInfo();

    static void runStorage(bool balance);
    static void runTheveninCharge(int minChargeC);
    static void runDischarge();
    static void runNiXXDischarge();

    static void runBalance();
    static void runDeltaCharge();
};


#endif /* PROGRAM_H_ */
