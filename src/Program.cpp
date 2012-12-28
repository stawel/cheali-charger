#include "Program.h"
#include "Hardware.h"
#include "ProgramData.h"
#include "LcdPrint.h"
#include "Utils.h"
#include "Screen.h"
#include "SimpleCharge.h"
#include "TheveninCharge.h"
#include "TheveninDischarge.h"
#include "Monitor.h"
#include "Storage.h"
#include "memory.h"
#include "StartInfoStrategy.h"
#include "Buzzer.h"
#include "StaticMenu.h"


namespace {

	SimpleCharge simple;

	//TODO: program memory
	const Screen::ScreenType theveninScreens[] PROGMEM =
	{ Screen::Screen1, Screen::ScreenRthVth, Screen::ScreenCIVlimits, Screen::ScreenTime,
	  Screen::ScreenBalancer0_2, Screen::ScreenBalancer3_5, Screen::ScreenTemperature };
	const Screen::ScreenType balanceScreens[] PROGMEM =
	{ Screen::ScreenBalancer0_2, Screen::ScreenBalancer0_2M,
	  Screen::ScreenBalancer3_5, Screen::ScreenBalancer3_5M /*, Screen::ScreenTemperature */};
	const Screen::ScreenType dischargeScreens[] PROGMEM =
	{ Screen::Screen1, Screen::ScreenRthVth, Screen::ScreenTime,
	  Screen::ScreenBalancer0_2,
	  Screen::ScreenBalancer0_2RthV,
	  Screen::ScreenBalancer0_2RthI,
	  Screen::ScreenBalancer0_2Rth,
	  Screen::ScreenBalancer3_5, Screen::ScreenTemperature };
	const Screen::ScreenType storageScreens[] PROGMEM =
	{ Screen::Screen1, Screen::ScreenRthVth, Screen::ScreenCIVlimits, Screen::ScreenTime,
	  Screen::ScreenBalancer0_2, Screen::ScreenBalancer3_5, Screen::ScreenTemperature };

	const Screen::ScreenType startInfoBalanceScreens[] PROGMEM =
	{ Screen::ScreenStartInfo, Screen::ScreenBalancer0_2, Screen::ScreenBalancer3_5};

	const Screen::ScreenType startInfoScreens[] PROGMEM =
	{ Screen::ScreenStartInfo };

	void chargingComplete()
	{
		lcd.clear();
		screens.displayChargeEnded();
		buzzer.soundProgramComplete();
		do { } while(keyboard.getPressedWithSpeed() == BUTTON_NONE);
		buzzer.soundOff();
	}
	void chargingMonitorError()
	{
		lcd.clear();
		screens.displayChargeEnded();
		buzzer.soundError();
		do { } while(keyboard.getPressedWithSpeed() == BUTTON_NONE);
		buzzer.soundOff();
	}

	Strategy::statusType doStrategy(Strategy &strategy, const Screen::ScreenType chargeScreens[]
													  , uint8_t screen_limit)
	{
		uint8_t key;
		bool run = true;
		uint16_t newMesurmentData = 0;
		Strategy::statusType status = Strategy::RUNNING;
		Monitor::statusType mstatus;
		strategy.powerOn();
		lcd.clear();
		uint8_t screen = 0;
		do {
			if(!PolarityCheck::runReversedPolarityInfo())
				screens.display(pgm::read<Screen::ScreenType>(&chargeScreens[screen]));

			key = selectIndexWithKeyboard(screen, screen_limit);
			if(run) {
				mstatus = monitor.run();
				if(mstatus != Monitor::OK) {
					strategy.powerOff();
					chargingMonitorError();
					run = false;
					status = Strategy::ERROR;
				}
				if(newMesurmentData != analogInputs.getCalculationCount()) {
					newMesurmentData = analogInputs.getCalculationCount();
					status = strategy.doStrategy();
					if(status == Strategy::COMPLETE_AND_EXIT) {
						strategy.powerOff();
						return status;
					}

					if(status != Strategy::RUNNING) {
						strategy.powerOff();
						chargingComplete();
						run = false;
					}
				}
			}
		} while(key != BUTTON_STOP);

		strategy.powerOff();
		return status;
	}

} //namespace {

bool Program::startInfo(ProgramType prog)
{
	screens.programType_ = prog;
	if(ProgramData::currentProgramData.isLiXX()) {
		//usues balance port
		startInfoStrategy.setBalancePort(true);
		return doStrategy(startInfoStrategy, startInfoBalanceScreens, sizeOfArray(startInfoBalanceScreens)) == Strategy::COMPLETE_AND_EXIT;
	} else {
		startInfoStrategy.setBalancePort(false);
		return doStrategy(startInfoStrategy, startInfoScreens, sizeOfArray(startInfoScreens)) == Strategy::COMPLETE_AND_EXIT;
	}
}

void Program::runStorage(bool balance)
{
	storage.setDoBalance(balance);
	storage.setVII(ProgramData::currentProgramData.getVoltage(ProgramData::VStorage),
			ProgramData::currentProgramData.battery.Ic, ProgramData::currentProgramData.battery.Id);
	doStrategy(storage, storageScreens, sizeOfArray(storageScreens));
}
void Program::runTheveninCharge(int minChargeC)
{
	theveninCharge.setVI(ProgramData::currentProgramData.getVoltage(ProgramData::VCharge), ProgramData::currentProgramData.battery.Ic);
	theveninCharge.setMinI(ProgramData::currentProgramData.battery.Ic/minChargeC);
	doStrategy(theveninCharge, theveninScreens, sizeOfArray(theveninScreens));
}
void Program::runDischarge()
{
	theveninDischarge.setVI(ProgramData::currentProgramData.getVoltage(ProgramData::VDischarge), ProgramData::currentProgramData.battery.Id);
	doStrategy(theveninDischarge, dischargeScreens, sizeOfArray(dischargeScreens));
}

void Program::runBalance()
{
	doStrategy(balancer, balanceScreens, sizeOfArray(balanceScreens));
}

void Program::run(ProgramType prog)
{
	if(startInfo(prog)) {
		switch(prog) {
		case Program::ChargeLiXX:
			runTheveninCharge(10);
			break;
		case Program::Balance:
			runBalance();
			break;
		case Program::DischargeLiXX:
			runDischarge();
			break;
		case Program::FastChargeLiXX:
			runTheveninCharge(5);
			break;
		case Program::StorageLiXX:
			runStorage(false);
		case Program::StorageLiXX_Balance:
			runStorage(true);
			break;
		default:
			//TODO:
			Screen::runNotImplemented();
			break;
		}
	}
}



// Program selection depending on the battery type

namespace {

	const char charge_str[] PROGMEM = "charge";
	const char chaBal_str[] PROGMEM = "charge+balance";
	const char balanc_str[] PROGMEM = "balance";
	const char discha_str[] PROGMEM = "discharge";
	const char fastCh_str[] PROGMEM = "fast charge";
	const char storag_str[] PROGMEM = "storage";
	const char stoBal_str[] PROGMEM = "storage+balanc";
	const char cycle__str[] PROGMEM = "cycle";
	const char edBatt_str[] PROGMEM = "edit battery";

	const char * const programLiXXMenu[] PROGMEM =
	{ charge_str,
	  chaBal_str,
	  balanc_str,
	  discha_str,
	  fastCh_str,
	  storag_str,
	  stoBal_str,
	  edBatt_str
	};

	const Program::ProgramType programLiXXMenuType[] PROGMEM =
	{ Program::ChargeLiXX,
	  Program::ChargeLiXX_Balance,
	  Program::Balance,
	  Program::DischargeLiXX,
	  Program::FastChargeLiXX,
	  Program::StorageLiXX,
	  Program::StorageLiXX_Balance,
	  Program::EditBattery
	};

	const char * const programNiXXMenu[] PROGMEM =
	{ charge_str,
	  discha_str,
	  cycle__str,
	  edBatt_str
	};

	const Program::ProgramType programNiXXMenuType[] PROGMEM =
	{ Program::ChargeNiXX,
	  Program::DischargeNiXX,
	  Program::CycleNiXX,
	  Program::EditBattery
	};

	const char * const programPbMenu[] PROGMEM =
	{ charge_str,
	  discha_str,
	  edBatt_str
	};

	const Program::ProgramType programPbMenuType[] PROGMEM =
	{ Program::ChargePb,
	  Program::DischargePb,
	  Program::EditBattery
	};


	StaticMenu selectLiXXMenu(programLiXXMenu, sizeOfArray(programLiXXMenu));
	StaticMenu selectNiXXMenu(programNiXXMenu, sizeOfArray(programNiXXMenu));
	StaticMenu selectPbMenu(programPbMenu, sizeOfArray(programPbMenu));

	StaticMenu * getSelectProgramMenu() {
		if(ProgramData::currentProgramData.isLiXX())
			return &selectLiXXMenu;
		else if(ProgramData::currentProgramData.isNiXX())
			return &selectNiXXMenu;
		else return &selectPbMenu;
	}
	Program::ProgramType getSelectProgramType(int index) {
		const Program::ProgramType * address;
		if(ProgramData::currentProgramData.isLiXX())
			address = &programLiXXMenuType[index];
		else if(ProgramData::currentProgramData.isNiXX())
			address = &programNiXXMenuType[index];
		else address = &programPbMenuType[index];
		return pgm::read(address);
	}
}

void Program::selectProgram(int index)
{
	uint8_t key;
	bool release = true;
	ProgramData::loadProgramData(index);
	StaticMenu * selectPrograms = getSelectProgramMenu();
	selectPrograms->render();

	do {
		key = selectPrograms->run();

		if(key == BUTTON_NONE)
			release = false;

		if(!release && key == BUTTON_START)  {
			int i = selectPrograms->getIndex();
			Program::ProgramType prog = getSelectProgramType(i);
			switch(prog) {
			case Program::EditBattery:
				if(ProgramData::currentProgramData.edit(index)) {
					buzzer.soundSave();
					ProgramData::saveProgramData(index);
					selectPrograms = getSelectProgramMenu();
				}
				break;
			default:
				Program::run(prog);
				break;
			}
			selectPrograms->render();
			release = true;
		}
	} while(key != BUTTON_STOP || release);
}




