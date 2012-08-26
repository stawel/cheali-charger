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
Screen::ScreenType theveninScreens[] =
{ Screen::Screen1, Screen::ScreenRthVth, Screen::ScreenCIVlimits, Screen::ScreenTime,
  Screen::ScreenBalancer0_2, Screen::ScreenBalancer3_5, Screen::ScreenTemperature };
Screen::ScreenType balanceScreens[] =
{ Screen::ScreenBalancer0_2, Screen::ScreenBalancer0_2M,
  Screen::ScreenBalancer3_5, Screen::ScreenBalancer3_5M /*, Screen::ScreenTemperature */};


void chargingComplete()
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
	ChargingStrategy::statusType status = ChargingStrategy::RUNNING;
	strategy.powerOn();
	lcd.clear();
	uint8_t screen = 0;
	do {
		screens.display(chargeScreens[screen]);
		key = selectIndexWithKeyboard(screen, screen_limit);
		if(run) {
			status = strategy.doStrategy();
			if(status != ChargingStrategy::RUNNING) {
				chargingComplete();
				run = false;
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
				charge(thevenin, theveninScreens, sizeOfArray(theveninScreens));
				break;
		case ProgramData::Balance:
				charge(balancer, balanceScreens, sizeOfArray(balanceScreens));
				break;
		case ProgramData::FastCharge:
				//TODO:
				charge(balancer, balanceScreens, sizeOfArray(balanceScreens));
				break;
		}
	}
}
