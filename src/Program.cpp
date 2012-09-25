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

namespace {
bool buzzer = false;

void buzzerOn()
{
	buzzer = true;
	pinMode(BUZZER_PIN, OUTPUT);
	analogWrite(BUZZER_PIN, 20);
}

void buzzerOff(){
	if(buzzer) {
		buzzer = false;
		analogWrite(BUZZER_PIN, 0);
	}
}

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

const Screen::ScreenType startInfoScreens[] PROGMEM =
{ Screen::ScreenStartInfo, Screen::ScreenBalancer0_2, Screen::ScreenBalancer3_5};

void chargingComplete()
{
	lcd.clear();
	screens.displayChargeEnded();
	buzzerOn();
	do { } while(keyboard.getPressedWithSpeed() == BUTTON_NONE);
	buzzerOff();
}
void chargingMonitorError()
{
	lcd.clear();
	screens.displayChargeEnded();
	buzzerOn();
	do { } while(keyboard.getPressedWithSpeed() == BUTTON_NONE);
	buzzerOff();
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
		screens.display(pgm_read<Screen::ScreenType>(&chargeScreens[screen]));
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
	return doStrategy(startInfoStrategy, startInfoScreens, sizeOfArray(startInfoScreens)) == Strategy::COMPLETE_AND_EXIT;
}

void Program::runStorage(bool balance)
{
	storage.setDoBalance(balance);
	storage.setVI(ProgramData::currentProgramData.getVoltage(ProgramData::VStorage), ProgramData::currentProgramData.I);
	doStrategy(storage, storageScreens, sizeOfArray(storageScreens));
}
void Program::runTheveninCharge(int minChargeC)
{
	theveninCharge.setVI(ProgramData::currentProgramData.getVoltage(ProgramData::VCharge), ProgramData::currentProgramData.I);
	theveninCharge.setMinI(ProgramData::currentProgramData.I/minChargeC);
	doStrategy(theveninCharge, theveninScreens, sizeOfArray(theveninScreens));
}
void Program::runDischarge()
{
	//TODO: implement discharge current
	theveninDischarge.setVI(ProgramData::currentProgramData.getVoltage(ProgramData::VDischarge), ProgramData::currentProgramData.I);
//	theveninDischarge.setVI(ANALOG_VOLTS(12), ProgramData::currentProgramData.I);
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
		case Program::Charge:
			runTheveninCharge(10);
			break;
		case Program::Balance:
			runBalance();
			break;
		case Program::Discharge:
			runDischarge();
			break;
		case Program::FastCharge:
			runTheveninCharge(5);
			break;
		case Program::Storage:
			runStorage(false);
		case Program::Storage_Balance:
			runStorage(true);
			break;
		default:
			//TODO:
			Screen::notImplemented();
			break;
		}
	}
}
