#include "StartProgram.h"
#include "Hardware.h"
#include "ProgramData.h"
#include "LcdPrint.h"
#include "Utils.h"
#include "Screen.h"
#include "SimpleCharge.h"
#include "TheveninCharge.h"

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
TheveninCharge thevenin;
ChargingStrategy &getChargeStrategy()
{
	return thevenin;
}


void chargingComplete()
{
	lcd.clear();
	screens.displayChargeEnded();
	buzzerOn();
	do { } while(keyboard.getPressedWithSpeed() == BUTTON_NONE);
	buzzerOff();
}


void charge() {
	Screen::ScreenType chargeScreens[] =
	{ Screen::Screen1, Screen::ScreenRthVth, Screen::ScreenCIVlimits, Screen::ScreenTime,
	  Screen::ScreenBalancer0_2, Screen::ScreenBalancer3_5, Screen::ScreenTemperature };

	uint8_t key;
	bool run = true;
	ChargingStrategy::statusType status = ChargingStrategy::RUNNING;
	getChargeStrategy().powerOn();
	lcd.clear();
	uint8_t screen = 0, screen_limit = sizeOfArray(chargeScreens);
	do {
		screens.display(chargeScreens[screen]);
		key = selectIndexWithKeyboard(screen, screen_limit);
		if(run) {
			status = getChargeStrategy().doStrategy();
			if(status != ChargingStrategy::RUNNING) {
				chargingComplete();
				run = false;
			}
		}
	} while(key != BUTTON_STOP);

	getChargeStrategy().powerOff();
}

void balance() {

	Screen::ScreenType chargeScreens[] =
	{ //Screen::Screen1,
	  Screen::ScreenBalancer0_2, Screen::ScreenBalancer3_5/*, Screen::ScreenTemperature */};

	uint8_t key;
	bool run = true;
	balancer.powerOn();
	lcd.clear();
	int ut=0;
	int bal=0;
	uint8_t screen = 0, screen_limit = sizeOfArray(chargeScreens);
	do {
		screens.display(chargeScreens[screen]);
		key = selectIndexWithKeyboard(screen, screen_limit);
		if(run)
			balancer.doStrategy();
		if(!balancer.isPowerOn() && run) {
			chargingComplete();
			run = false;
		}
	} while(key != BUTTON_STOP);
	balancer.powerOff();
}


void balance_test() {
	Screen::ScreenType chargeScreens[] =
	{ //Screen::Screen1,
	  Screen::ScreenBalancer0_2, Screen::ScreenBalancer3_5/*, Screen::ScreenTemperature */};

	uint8_t key;
	bool run = true;
	balancer.powerOn();
	lcd.clear();
	int ut=0;
	int bal=0;
	uint8_t screen = 0, screen_limit = sizeOfArray(chargeScreens);
	do {
		screens.display(chargeScreens[screen]);
		key = selectIndexWithKeyboard(screen, screen_limit);
		if(!balancer.isPowerOn() && run) {
		//	chargingComplete();
			run = false;
		}
		if(key & BUTTON_INC & BUTTON_START) bal ++;
		if(key & BUTTON_DEC & BUTTON_START) bal --;
		balancer.setBalance(bal);
	} while(key != BUTTON_STOP);
	balancer.powerOff();
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

void printStartInfo()
{
	lcd.setCursor(0,0);
	ProgramData::currentProgramData.printBatteryString(4);
	lcd.print(' ');
	ProgramData::currentProgramData.printVoltageString();
	lcd.print(' ');
	ProgramData::currentProgramData.printProgramString();
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

bool startInfo()
{
	uint8_t key;
	bool ok = false;
	lcd.clear();
	hardware::setBatteryOutput(true);
	do {
		printStartInfo();
		key = keyboard.getPressedWithSpeed();
		if(key == BUTTON_START) {
			ok = true;
			break;
		}
	} while(key != BUTTON_STOP);

	hardware::setBatteryOutput(false);
	return ok;
}


void startProgram(ProgramData::ProgramType prog)
{

	ProgramData::currentProgramData.initSmps();
	if(startInfo()) {
		switch(prog) {
		case ProgramData::Charge:
				charge();
				break;
		case ProgramData::Balance:
				balance();
				break;
		case ProgramData::FastCharge:
				//TODO:
				balance_test();
				break;
		}
	}
}
