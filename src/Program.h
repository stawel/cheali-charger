#ifndef PROGRAM_H_
#define PROGRAM_H_

#include "ProgramData.h"

class Program {
public:
	enum ProgramType { Charge, Charge_Balance, Balance, Discharge, FastCharge, Storage, Storage_Balance, EditName, EditBattery , 	LAST_PROGRAM_TYPE};

	static void run(ProgramType prog);

	static void printStartInfo(ProgramType prog);
	static bool startInfo(ProgramType prog);

	static void runStorage(bool balance);
	static void runTheveninCharge(int minChargeC);
	static void runDischarge();
	static void runBalance();
};


#endif /* PROGRAM_H_ */
