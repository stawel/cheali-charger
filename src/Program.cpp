#include "Program.h"
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


char programString[] PROGMEM = "ChCBBlDiFCStSBENEB";
void Program::printProgram2chars(ProgramType prog)
{
	//TODO: ??
	lcdPrint_P(programString+prog*2, 2);
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

void notImplemented()
{
	lcd.clear();
	screens.displayStrings(PSTR("Function not"), PSTR("implemented yet"));
	do { } while(keyboard.getPressedWithSpeed() == BUTTON_NONE);
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

void Program::run(ProgramType prog)
{
	if(startInfo(prog)) {
		switch(prog) {
		case Program::Charge:
				charge(thevenin, theveninScreens, sizeOfArray(theveninScreens));
				break;
		case Program::Balance:
				charge(balancer, balanceScreens, sizeOfArray(balanceScreens));
				break;
		default:
		case Program::FastCharge:
				//TODO:
				notImplemented();
				break;
		}
	}
}
