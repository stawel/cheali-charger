#include "GTPowerA610.h"
#include "TimerOne.h"


uint16_t smpsValue()
{
	return smps.getValue();
}

uint16_t dischargerValue()
{
	return discharger.getValue();
}

const AnalogInputs::DefaultValues inputs_P[AnalogInputs::PHYSICAL_INPUTS] PROGMEM =
{
		{AnalogInputs::analogValue<OUTPUT_VOLATAGE_PIN>,	{0,0}, 	{27516, 12552}},	//Vout
		{AnalogInputs::analogValue<SMPS_CURRENT_PIN>,		{0,0}, 	{1024, 200}},		//Ismps
		{AnalogInputs::analogValue<DISCHARGE_CURRENT_PIN>,	{0,0},	{8960, 1000}},	 	//Idischarge

		{mux.analogRead<MADDR_V_OUTMUX>, 	{0, 0}, 	{24832, 12552}},  	//VoutMux
		{mux.analogRead<MADDR_T_INTERN>, 	{21500, ANALOG_CELCIUS(52)}, {41023, ANALOG_CELCIUS(29)}},	//Tintern
		{mux.analogRead<MADDR_V_IN>,		{0, 0}, 	{48000, 13930}},  	//Vin
		{mux.analogRead<MADDR_T_EXTERN>,	{0, 0}, 	{1, 1}},			//Textern

		{mux.analogRead<MADDR_V_BALANSER0>,	{0, 0}, 	{54592, 4169}},			//Vb0
		{mux.analogRead<MADDR_V_BALANSER1>,	{0, 0}, 	{54592, 4178}},			//Vb1
		{mux.analogRead<MADDR_V_BALANSER2>,	{0, 0}, 	{54913, 4200}},			//Vb2
		{mux.analogRead<MADDR_V_BALANSER3>,	{0, 0}, 	{51834, 3966}},			//Vb3
		{mux.analogRead<MADDR_V_BALANSER4>,	{0, 0}, 	{51834, 3966}},			//Vb4
		{mux.analogRead<MADDR_V_BALANSER5>,	{0, 0}, 	{51834, 3966}},			//Vb5

		{smpsValue,				{0,0}, 	{836, 2250}},			//IsmpsValue
		{dischargerValue,		{0,0},	{653,1000}},	 		//IdischargeValue
};


uint8_t hardware::getKeyPressed()
{
	return    (mux.digitalRead(MADDR_BUTTON_STOP) ? 0 : BUTTON_STOP)
			| (mux.digitalRead(MADDR_BUTTON_DEC)  ? 0 : BUTTON_DEC)
			| (mux.digitalRead(MADDR_BUTTON_INC)  ? 0 : BUTTON_INC)
			| (mux.digitalRead(MADDR_BUTTON_START)? 0 : BUTTON_START);
}

void hardware::init()
{
	analogReference(EXTERNAL);
	pinMode(BACKLIGHT_PIN, OUTPUT);
	pinMode(OUTPUT_DISABLE_PIN, OUTPUT);
	pinMode(FAN_PIN, OUTPUT);
	setBatteryOutput(false);
	setFan(false);

	lcd.begin(LCD_COLUMNS, LCD_LINES);
	mux.init();
	timer.init();
}

void hardware::setLCDBacklight(uint16_t val)
{
	Timer1.pwm(BACKLIGHT_PIN, val);
}

void hardware::setBatteryOutput(bool enable)
{
	digitalWrite(OUTPUT_DISABLE_PIN, !enable);
}

void hardware::setFan(bool enable)
{
	digitalWrite(FAN_PIN, enable);
}


void hardware::delay(uint32_t t)
{
//	timer.delay(t);
	analogInputs.doMeasurement((AnalogInputs::PHYSICAL_INPUTS*t)/4);
}






Multiplexer mux;
Keyboard keyboard;
SMPS smps;
Discharger discharger;
Balancer balancer;
AnalogInputs analogInputs(inputs_P);

LiquidCrystal lcd(LCD_ENABLE_RS, LCD_ENABLE_PIN,
		LCD_D0_PIN, LCD_D1_PIN, LCD_D2_PIN, LCD_D3_PIN);
