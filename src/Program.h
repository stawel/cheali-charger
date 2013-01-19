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
