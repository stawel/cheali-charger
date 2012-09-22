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


char programString[] PROGMEM = "ChCBBlDiFCStSBENEB";
void Program::printProgram2chars(ProgramType prog)
{
	//TODO: ??
	lcdPrint_P(programString+prog*2, 2);
}

SimpleCharge simple;

//TODO: program memory
Screen::ScreenType theveninScreens[] =
{ Screen::Screen1, Screen::ScreenRthVth, Screen::ScreenCIVlimits, Screen::ScreenTime,
  Screen::ScreenBalancer0_2, Screen::ScreenBalancer3_5, Screen::ScreenTemperature };
Screen::ScreenType balanceScreens[] =
{ Screen::ScreenBalancer0_2, Screen::ScreenBalancer0_2M,
  Screen::ScreenBalancer3_5, Screen::ScreenBalancer3_5M /*, Screen::ScreenTemperature */};
Screen::ScreenType dischargeScreens[] =
{ Screen::Screen1, Screen::ScreenRthVth, Screen::ScreenTime,
  Screen::ScreenBalancer0_2, Screen::ScreenBalancer3_5, Screen::ScreenTemperature };
Screen::ScreenType storageScreens[] =
{ Screen::Screen1, Screen::ScreenRthVth, Screen::ScreenCIVlimits, Screen::ScreenTime,
  Screen::ScreenBalancer0_2, Screen::ScreenBalancer3_5, Screen::ScreenTemperature };


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

void charge(ChargingStrategy &strategy, Screen::ScreenType chargeScreens[], uint8_t screen_limit)
{
	uint8_t key;
	bool run = true;
	uint16_t newMesurmentData = 0;
	ChargingStrategy::statusType status = ChargingStrategy::RUNNING;
	Monitor::statusType mstatus;
	strategy.powerOn();
	lcd.clear();
	uint8_t screen = 0;
	do {
		screens.display(chargeScreens[screen]);
		key = selectIndexWithKeyboard(screen, screen_limit);
		if(run) {
			mstatus = monitor.run();
			if(mstatus != Monitor::OK) {
				strategy.powerOff();
				chargingMonitorError();
				run = false;
			}
			if(newMesurmentData != analogInputs.getCalculationCount()) {
				newMesurmentData = analogInputs.getCalculationCount();
				status = strategy.doStrategy();
				if(status != ChargingStrategy::RUNNING) {
					strategy.powerOff();
					chargingComplete();
					run = false;
				}
			}
		}
	} while(key != BUTTON_STOP);

	strategy.powerOff();
}

uint16_t getChargeProcent(){
	uint16_t v1,v2, add = 0;
	uint32_t v;
	v2 = ProgramData::currentProgramData.getVoltage(ProgramData::VCharge);
	v1 = ProgramData::currentProgramData.getVoltage(ProgramData::VDischarge);
	v = analogInputs.getRealValue(AnalogInputs::VoutBalancer);

	if(((v*99)/100) >= v2) return 100;
	if(v<=v1) return 0;
	v-=v1;
	v*=1000;
	v=  v/(v2-v1);
	if(v%10 > 5) add = 1;
	return v/10 + add;
}

void Program::printStartInfo(ProgramType prog)
{
	lcd.setCursor(0,0);
	ProgramData::currentProgramData.printBatteryString(4);
	lcd.print(' ');
	ProgramData::currentProgramData.printVoltageString();
	lcd.print(' ');
	printProgram2chars(prog);
	analogInputs.doFullMeasurement();

	lcd.setCursor(0,1);
	uint16_t procent = getChargeProcent();
	if(procent < 10)
		lcd.print(' ');
	lcd.print(procent);
	lcdPrint_P(PSTR("% "));

	analogInputs.printRealValue(AnalogInputs::Vout, 5);
	lcd.print(' ');
	analogInputs.printRealValue(AnalogInputs::Vbalacer, 5);
	lcd.print(analogInputs.getConnectedBalancePorts());
}

bool Program::startInfo(ProgramType prog)
{
	uint8_t key;
	bool ok = false;
	lcd.clear();
	hardware::setBatteryOutput(true);
	do {
		printStartInfo(prog);
		key = keyboard.getPressedWithSpeed();
		if(key == BUTTON_START) {
			ok = true;
			break;
		}
	} while(key != BUTTON_STOP);

	hardware::setBatteryOutput(false);
	return ok;
}

void Program::runStorage(bool balance)
{
	storage.setDoBalance(balance);
	storage.setVI(ProgramData::currentProgramData.getVoltage(ProgramData::VStorage), ProgramData::currentProgramData.I);
	charge(storage, storageScreens, sizeOfArray(storageScreens));
}
void Program::runTheveninCharge(int minChargeC)
{
	theveninCharge.setVI(ProgramData::currentProgramData.getVoltage(ProgramData::VCharge), ProgramData::currentProgramData.I);
	theveninCharge.setMinI(ProgramData::currentProgramData.I/minChargeC);
	charge(theveninCharge, theveninScreens, sizeOfArray(theveninScreens));
}
void Program::runDischarge()
{
	//TODO: implement discharge current
	theveninDischarge.setVI(ANALOG_VOLTS(11.7), ProgramData::currentProgramData.I);
	charge(theveninDischarge, dischargeScreens, sizeOfArray(dischargeScreens));
}

void Program::runBalance()
{
	charge(balancer, balanceScreens, sizeOfArray(balanceScreens));
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
