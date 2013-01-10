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


uint16_t reversePolarityValue()
{
    uint16_t vr = AnalogInputs::analogValue<REVERSE_POLARITY_PIN>();
//    return vr;
    uint16_t vo = AnalogInputs::analogValue<OUTPUT_VOLATAGE_PIN>();
    if(vr > vo) return vr - vo;
    return 0;
}

const AnalogInputs::DefaultValues inputs_P[AnalogInputs::PHYSICAL_INPUTS] PROGMEM = {
    {AnalogInputs::analogValue<OUTPUT_VOLATAGE_PIN>,    {0,0},      {27516, ANALOG_VOLT(12.552)}},  //Vout
    {reversePolarityValue,                              {0,0},      {27516, ANALOG_VOLT(12.552)}},  //VreversePolarity
    {AnalogInputs::analogValue<SMPS_CURRENT_PIN>,       {0,0},      {1024,  ANALOG_AMP(0.200)}},    //Ismps
    {AnalogInputs::analogValue<DISCHARGE_CURRENT_PIN>,  {0,0},      {8960,  ANALOG_AMP(1.000)}},    //Idischarge

    {mux.analogRead<MADDR_V_OUTMUX>,    {0, 0},                     {24832, ANALOG_VOLT(12.552)}},  //VoutMux
    {mux.analogRead<MADDR_T_INTERN>,    {21500, ANALOG_CELCIUS(52)},{41023, ANALOG_CELCIUS(29)}},   //Tintern
    {mux.analogRead<MADDR_V_IN>,        {0, 0},                     {48000, ANALOG_VOLT(13.930)}},  //Vin
    {mux.analogRead<MADDR_T_EXTERN>,    {6272,  ANALOG_CELCIUS(24)},{14300, ANALOG_CELCIUS(60)}},   //Textern

    {mux.analogRead<MADDR_V_BALANSER0>, {0, 0},                     {54592, ANALOG_VOLT(4.169)}},   //Vb0
    {mux.analogRead<MADDR_V_BALANSER1>, {0, 0},                     {54592, ANALOG_VOLT(4.178)}},   //Vb1
    {mux.analogRead<MADDR_V_BALANSER2>, {0, 0},                     {54913, ANALOG_VOLT(4.200)}},   //Vb2
    {mux.analogRead<MADDR_V_BALANSER3>, {0, 0},                     {51834, ANALOG_VOLT(3.966)}},   //Vb3
    {mux.analogRead<MADDR_V_BALANSER4>, {0, 0},                     {51834, ANALOG_VOLT(3.966)}},   //Vb4
    {mux.analogRead<MADDR_V_BALANSER5>, {0, 0},                     {51834, ANALOG_VOLT(3.966)}},   //Vb5

    {smpsValue,                         {0,0},                      {836, ANALOG_AMP(2.250)}},      //IsmpsValue
    {dischargerValue,                   {0,0},                      {653, ANALOG_AMP(1.000)}},      //IdischargeValue
#ifdef ANALOG_INPUTS_V_UNKNOWN
    {mux.analogRead<MADDR_V_UNKNOWN0>,  {0,0},                      {1, 1}},                        //UNKNOWN0
    {mux.analogRead<MADDR_V_UNKNOWN1>,  {0,0},                      {1, 1}},                        //UNKNOWN1
#endif
};


uint8_t hardware::getKeyPressed()
{
    return   (mux.digitalRead(MADDR_BUTTON_STOP) ? 0 : BUTTON_STOP)
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
    pinMode(BUZZER_PIN, OUTPUT);

    setBatteryOutput(false);
    setFan(false);
    hardware::setBuzzer(0);

    lcd.begin(LCD_COLUMNS, LCD_LINES);
    mux.init();
    timer.init();
}

void hardware::setLCDBacklight(uint8_t val)
{
    uint32_t v1,v2;
    v1  = LCD_BACKLIGHT_MAX;
    v1 *= val;
    v2  = LCD_BACKLIGHT_MIN;
    v2 *= 100 - val;
    v1+=v2;
    v1/=100;
    Timer1.pwm(BACKLIGHT_PIN, v1);
}

void hardware::setBatteryOutput(bool enable)
{
    digitalWrite(OUTPUT_DISABLE_PIN, !enable);
}

void hardware::setFan(bool enable)
{
    digitalWrite(FAN_PIN, enable);
}


void hardware::delay(uint16_t t)
{
    if(analogInputs.on_)
        analogInputs.doMeasurement((AnalogInputs::PHYSICAL_INPUTS*t)/4);
    else
        timer.delay(t);
}

void hardware::setBuzzer(uint16_t val)
{
    analogWrite(BUZZER_PIN, val);
}





Multiplexer mux;
Keyboard keyboard;
SMPS smps;
Discharger discharger;
Balancer balancer;
AnalogInputs analogInputs(inputs_P);

LiquidCrystal lcd(LCD_ENABLE_RS, LCD_ENABLE_PIN,
        LCD_D0_PIN, LCD_D1_PIN, LCD_D2_PIN, LCD_D3_PIN);
