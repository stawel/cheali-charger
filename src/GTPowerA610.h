#ifndef GTPOWERA610_H_
#define GTPOWERA610_H_

#define LCD_LINES 2
#define LCD_COLUMNS 16

#define MAX_CHARGE_V  	25300  //25.3V
#define MAX_CHARGE_I  	10000  //10A
#define MAX_CHARGE_P   200000  //200W

#define MAX_DISCHARGE_P 25000  //25W
#define MAX_DISCHARGE_I  5000  //5A

#define MAX_BANANCE_CELLS	6


// pin configuration
#define MUX_ADR2_PIN	1
#define MUX_ADR1_PIN	2
#define MUX_ADR0_PIN	3

//pin 4 - 8 - reset, Vcc, GND, XTAL2, XTAL1

#define FAN_PIN		9
//pin 10 - rs232 TX

#define DISCHARGE_DISABLE_PIN 	11
#define OUTPUT_DISABLE_PIN		12
#define BACKLIGHT_PIN 			13
// pin 14 - the same pin for discharger and smps
#define DISCHARGE_VALUE_PIN 	14
#define SMPS_VALUE_PIN			14
#define SMPS_DISABLE_PIN		15
#define BUZZER_PIN				16

// pin 17,18 - VCC, GND

#define LCD_D3_PIN		19
#define LCD_D2_PIN		20
#define LCD_D1_PIN		21
#define LCD_D0_PIN		22
#define LCD_ENABLE_PIN	23
#define LCD_ENABLE_RS	24

#define UNKNOWN0_PIN		25
#define UNKNOWN1_PIN		26

// pin 27 - 29 - AVcc, GND, AREF

#define	BALANCER5_LOAD_PIN	30
#define	BALANCER4_LOAD_PIN	31
//pin 32
#define MUX0_Z_A_PIN				A5
#define MUX0_Z_D_PIN				32
//pin 33
#define MUX1_Z_A_PIN				A4
#define MUX1_Z_D_PIN				33
//pin 34
#define DISCHARGE_CURRENT_PIN	A3
//pin 35 - ??
#define SMPS_CURRENT_PIN		A2
//pin 36
#define UNKNOWN2_PIN			A1
//pin 37
#define OUTPUT_VOLATAGE_PIN		A0

// pin 38,39 - Vcc, GND

#define	BALANCER0_LOAD_PIN	40
#define	BALANCER1_LOAD_PIN	41
#define	BALANCER2_LOAD_PIN	42
#define	BALANCER3_LOAD_PIN	43

#define UNKNOWN3_PIN		44




//Multiplexer addresses
#define MUXINPUT0 	0
#define MUXINPUT1 	8

#define MADDR_V_OUTMUX	 	(0 + MUXINPUT0)
#define MADDR_T_INTERN 		(1 + MUXINPUT0)
#define MADDR_V_IN 			(2 + MUXINPUT0)
#define MADDR_T_EXTERN 		(3 + MUXINPUT0)
#define MADDR_BUTTON_DEC 	(4 + MUXINPUT0)
#define MADDR_BUTTON_INC 	(5 + MUXINPUT0)
#define MADDR_BUTTON_STOP 	(6 + MUXINPUT0)
#define MADDR_BUTTON_START 	(7 + MUXINPUT0)

#define MADDR_V_BALANSER0 	(0 + MUXINPUT1)
#define MADDR_V_BALANSER1 	(1 + MUXINPUT1)
#define MADDR_V_BALANSER2 	(2 + MUXINPUT1)
#define MADDR_V_BALANSER3 	(3 + MUXINPUT1)
#define MADDR_V_BALANSER4 	(4 + MUXINPUT1)
#define MADDR_V_BALANSER5 	(5 + MUXINPUT1)
#define MADDR_V_UNKNOWN0 	(6 + MUXINPUT1)
#define MADDR_V_UNKNOWN1   	(7 + MUXINPUT1)




#include <LiquidCrystal.h>
#include "Multiplexer.h"
#include "Keyboard.h"
#include "AnalogInputs.h"
#include "Timer.h"
#include "SMPS.h"
#include "Balancer.h"

extern LiquidCrystal lcd;
extern Multiplexer mux;
extern Keyboard keyboard;
extern AnalogInputs analogInputs;
extern Timer timer;
extern SMPS smps;
extern Balancer balancer;

namespace hardware {
	void init();
	uint8_t getKeyPressed();
	void delay(uint32_t t);
	void setLCDBacklight(uint16_t val);
	void setBatteryOutput(bool enable);
}


#endif /* GTPOWERA610_H_ */
