/*
    cheali-charger - open source firmware for a variety of LiPo chargers
    Copyright (C) 2016  Paweł Stawicki. All right reserved.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#define __STDC_LIMIT_MACROS
#include <stdint.h>

#include "LcdPrint.h"
#include "SMPS.h"
#include "Calibration.h"
#include "Utils.h"
#include "Screen.h"
#include "Buzzer.h"
#include "StackInfo.h"
#include "EditMenu.h"
#include "SerialLog.h"
#include "Program.h"
#include "AnalogInputsPrivate.h"
#include "Hardware.h"
#include "eeprom.h"
#include "Balancer.h"
#include "memory.h"
#include "EditMenu.h"

namespace Calibration {

uint16_t calibrationPoint;

static void copyVbalVout()
{
    //info: we assume that Vout_plus_pin and Vout_minus_pin have identically voltage dividers
    AnalogInputs::CalibrationPoint p;
    p.x = AnalogInputs::getAvrADCValue(AnalogInputs::Vout_plus_pin);
    p.x -= AnalogInputs::getAvrADCValue(AnalogInputs::Vout_minus_pin);
    p.y = AnalogInputs::getRealValue(AnalogInputs::Vbalancer);
    AnalogInputs::setCalibrationPoint(AnalogInputs::Vout_plus_pin, calibrationPoint, p);
    AnalogInputs::setCalibrationPoint(AnalogInputs::Vout_minus_pin, calibrationPoint, p);
}

#ifdef ENABLE_SIMPLIFIED_VB0_VB2_CIRCUIT
static void calibrateSimplifiedVb1_pin(AnalogInputs::ValueType real_v)
{
    AnalogInputs::CalibrationPoint p1,p2;
    p1.x = AnalogInputs::getAvrADCValue(AnalogInputs::Vb1_pin);
    p1.y = real_v + AnalogInputs::getRealValue(AnalogInputs::Vb0_pin);
    p2.x = AnalogInputs::getAvrADCValue(AnalogInputs::Vb2_pin);
    p2.y = p1.y + AnalogInputs::getRealValue(AnalogInputs::Vb2);

    //info: we assume that Vb0_pin and Vb1_pin have identically voltage dividers
    AnalogInputs::setCalibrationPoint(AnalogInputs::Vb0_pin, calibrationPoint, p1);
    AnalogInputs::setCalibrationPoint(AnalogInputs::Vb1_pin, calibrationPoint, p1);

    if(AnalogInputs::isConnected(AnalogInputs::Vb2)) {
        AnalogInputs::setCalibrationPoint(AnalogInputs::Vb2_pin, calibrationPoint, p2);
    }
}
static void calibrateSimplifiedVb2_pin(AnalogInputs::ValueType real_v)
{
    AnalogInputs::CalibrationPoint p2;
    p2.x = AnalogInputs::getAvrADCValue(AnalogInputs::Vb2_pin);
    p2.y = real_v + AnalogInputs::getRealValue(AnalogInputs::Vb1_pin);
    AnalogInputs::setCalibrationPoint(AnalogInputs::Vb2_pin, calibrationPoint, p2);
}

#endif


static void saveCalibration(AnalogInputs::Name nameEdited, AnalogInputs::Name nameSaveTo, AnalogInputs::ValueType adc, AnalogInputs::ValueType newValue)
{
    AnalogInputs::CalibrationPoint p;
    p.x = adc;
    p.y = newValue;

#ifdef ENABLE_SIMPLIFIED_VB0_VB2_CIRCUIT
        if(nameEdited == AnalogInputs::Vb1)
            calibrateSimplifiedVb1_pin(p.y);
        else if(nameEdited == AnalogInputs::Vb2)
            calibrateSimplifiedVb2_pin(p.y);
        else
            AnalogInputs::setCalibrationPoint(nameSaveTo, calibrationPoint, p);
#else
        AnalogInputs::setCalibrationPoint(nameSaveTo, calibrationPoint, p);
#endif
}

static void saveCalibration(bool doCopyVbalVout, AnalogInputs::Name nameEdited,  AnalogInputs::Name nameSaveTo)
{
    Buzzer::soundSelect();
    AnalogInputs::ValueType newValue = AnalogInputs::getRealValue(nameEdited);
    SerialLog::flush();
    saveCalibration(nameEdited, nameSaveTo, AnalogInputs::getAvrADCValue(nameSaveTo), newValue);
    if(doCopyVbalVout) {
	    AnalogInputs::on_ = true;
	    AnalogInputs::doFullMeasurement();
        SerialLog::flush();
        copyVbalVout();
    }
    eeprom::restoreCalibrationCRC();
}

// menus

#define COND_COPY_VOUT      128
#define COND_POINT          4
#define COND_EDITABLE       2
#define COND_NOT_EDITABLE   1
#define COND_EDITABLE_COPY  COND_EDITABLE + COND_COPY_VOUT

#define EANALOG_V(name) 	{CP_TYPE_V, 			0, &AnalogInputs::real_[AnalogInputs::name]}
#define EANALOG_T(name) 	{CP_TYPE_TEMPERATURE, 	0, &AnalogInputs::real_[AnalogInputs::name]}
#define EANALOG_ADC(name) 	{CP_TYPE_UNSIGNED, 		0, &AnalogInputs::avrAdc_[AnalogInputs::name]}

void runCalibrationMenu(const EditMenu::StaticEditData * menu,
        const AnalogInputs::Name * nameEdited,
        const AnalogInputs::Name * nameSaveTo,
        uint8_t calibrationPoint = false) {

	EditMenu::initialize(menu);

    uint16_t selector = EDIT_MENU_ALWAYS;
    if(!calibrationPoint && settings.menuType == Settings::MenuSimple) {
        selector  ^= COND_POINT;
    }

    EditMenu::setSelector(selector);
    int8_t item;
    do {
        item = EditMenu::run(true);
        if(item < 0) break;
        uint8_t itemCondition = EditMenu::getEnableCondition(item);
		if(itemCondition & COND_EDITABLE) {
			// voltage or temperature calibration
			AnalogInputs::Name editedName = pgm::read(&nameEdited[item]);
			if(AnalogInputs::isConnected(editedName)) {
				AnalogInputs::doFullMeasurement();
				AnalogInputs::on_ = false;
				AnalogInputs::onTintern_ = false;
				if(EditMenu::runEdit()) {
					bool doCopyVout = itemCondition & COND_COPY_VOUT;
					saveCalibration(doCopyVout, editedName, pgm::read(&nameSaveTo[item]));
				}
				AnalogInputs::on_ = true;
				AnalogInputs::onTintern_ = true;
			}
        } else if (itemCondition & COND_POINT) {
            // calibration point edited
            EditMenu::runEdit();
        }
    } while(true);
}




// voltage calibration menu

const AnalogInputs::Name voltageNameEdited[] PROGMEM = {
       AnalogInputs::Vin,
       AnalogInputs::Vb1,
       AnalogInputs::Vb2,
       AnalogInputs::Vb3,
       AnalogInputs::Vb4,
       AnalogInputs::Vb5,
       AnalogInputs::Vb6,
       BALANCER_PORTS_GT_6(AnalogInputs::Vb7, AnalogInputs::Vb8,)
};

const AnalogInputs::Name voltageNameSaveTo[] PROGMEM = {
       AnalogInputs::Vin,
       AnalogInputs::Vb1_pin,
       AnalogInputs::Vb2_pin,
       AnalogInputs::Vb3_pin,
       AnalogInputs::Vb4_pin,
       AnalogInputs::Vb5_pin,
       AnalogInputs::Vb6_pin,
       BALANCER_PORTS_GT_6(AnalogInputs::Vb7_pin, AnalogInputs::Vb8_pin)
};

const EditMenu::StaticEditData editVoltageData[] PROGMEM = {
{string_v_menu_input,       COND_EDITABLE, 		EANALOG_V(Vin),         {CE_STEP_TYPE_KEY_SPEED, 0, ANALOG_VOLT(30)}},
{string_v_menu_cell1,       COND_EDITABLE_COPY, EANALOG_V(Vb1),         {CE_STEP_TYPE_KEY_SPEED, 0, ANALOG_VOLT(5)}},
{string_v_menu_cell2,       COND_EDITABLE_COPY, EANALOG_V(Vb2),         {CE_STEP_TYPE_KEY_SPEED, 0, ANALOG_VOLT(5)}},
{string_v_menu_cell3,       COND_EDITABLE_COPY, EANALOG_V(Vb3),         {CE_STEP_TYPE_KEY_SPEED, 0, ANALOG_VOLT(5)}},
{string_v_menu_cell4,       COND_EDITABLE_COPY, EANALOG_V(Vb4),         {CE_STEP_TYPE_KEY_SPEED, 0, ANALOG_VOLT(5)}},
{string_v_menu_cell5,       COND_EDITABLE_COPY, EANALOG_V(Vb5),         {CE_STEP_TYPE_KEY_SPEED, 0, ANALOG_VOLT(5)}},
{string_v_menu_cell6,       COND_EDITABLE_COPY, EANALOG_V(Vb6),         {CE_STEP_TYPE_KEY_SPEED, 0, ANALOG_VOLT(5)}},
BALANCER_PORTS_GT_6(
{string_v_menu_cell7,       COND_EDITABLE_COPY, EANALOG_V(Vb7),         {CE_STEP_TYPE_KEY_SPEED, 0, ANALOG_VOLT(5)}},
{string_v_menu_cell8,       COND_EDITABLE_COPY,	EANALOG_V(Vb8),         {CE_STEP_TYPE_KEY_SPEED, 0, ANALOG_VOLT(5)}},
)
{string_v_menu_cellSum,     COND_NOT_EDITABLE,  EANALOG_V(Vbalancer),   {0, 0, 0}},
{string_v_menu_output,      COND_NOT_EDITABLE,  EANALOG_V(Vout),        {0, 0, 0}},
{string_menu_point,         COND_POINT,         {CP_TYPE_UNSIGNED, 0, &calibrationPoint},        {1, 0, 1}},
{NULL,                      EDIT_MENU_LAST}
};

void voltageCalibration()
{
    calibrationPoint = 1;
    Program::dischargeOutputCapacitor();
    AnalogInputs::powerOn();
    if(testVout(true)) {
        runCalibrationMenu(editVoltageData, voltageNameEdited, voltageNameSaveTo);
    }
    AnalogInputs::powerOff();
}



/* expert voltage calibration menu*/

#ifdef ENABLE_EXPERT_VOLTAGE_CALIBRATION

const EditMenu::StaticEditData editExpertVoltageData[] PROGMEM = {
#ifdef ENABLE_SIMPLIFIED_VB0_VB2_CIRCUIT
{string_ev_menu_cell0pin,           COND_EDITABLE,   EANALOG_V(Vb0_pin),         {CE_STEP_TYPE_KEY_SPEED, 0, ANALOG_VOLT(10)}},
{string_ev_menu_cell1pin,           COND_EDITABLE,   EANALOG_V(Vb1_pin),         {CE_STEP_TYPE_KEY_SPEED, 0, ANALOG_VOLT(10)}},
{string_ev_menu_cell2pin,           COND_EDITABLE,   EANALOG_V(Vb2_pin),         {CE_STEP_TYPE_KEY_SPEED, 0, ANALOG_VOLT(10)}},
#endif //ENABLE_SIMPLIFIED_VB0_VB2_CIRCUIT
{string_ev_menu_plusVoltagePin,     COND_EDITABLE,   EANALOG_V(Vout_plus_pin),   {CE_STEP_TYPE_KEY_SPEED, 0, MAX_CHARGE_V}},
{string_ev_menu_minusVoltagePin,    COND_EDITABLE,   EANALOG_V(Vout_minus_pin),  {CE_STEP_TYPE_KEY_SPEED, 0, MAX_CHARGE_V}},
{string_menu_point,                 COND_POINT,     {CP_TYPE_UNSIGNED, 0, &calibrationPoint},        {1, 0, 1}},
{NULL,                      		EDIT_MENU_LAST}
};

const AnalogInputs::Name expertVoltageName[] PROGMEM = {
#ifdef ENABLE_SIMPLIFIED_VB0_VB2_CIRCUIT
        AnalogInputs::Vb0_pin,
        AnalogInputs::Vb1_pin,
        AnalogInputs::Vb2_pin,
#endif //ENABLE_SIMPLIFIED_VB0_VB2_CIRCUIT
        AnalogInputs::Vout_plus_pin,
        AnalogInputs::Vout_minus_pin
};

void expertVoltageCalibration()
{
    calibrationPoint = 1;
    Program::dischargeOutputCapacitor();
    AnalogInputs::powerOn(false);
    PolarityCheck::checkReversedPolarity_ = false;
    runCalibrationMenu(editExpertVoltageData, expertVoltageName, expertVoltageName);
    PolarityCheck::checkReversedPolarity_ = true;
    AnalogInputs::powerOff();

}
#endif //ENABLE_EXPERT_VOLTAGE_CALIBRATION




/* external temperature calibration menu */

const EditMenu::StaticEditData editExternTData[] PROGMEM = {
{string_t_menu_temperature,     COND_EDITABLE,  	EANALOG_T(Textern),             {CE_STEP_TYPE_KEY_SPEED, 0, ANALOG_CELCIUS(100)}},
{string_t_menu_adc,             COND_NOT_EDITABLE,	EANALOG_ADC(Textern),           {0,0,0}},
{string_menu_point,             COND_POINT,     	{CP_TYPE_UNSIGNED, 0, &calibrationPoint},        {1, 0, 1}},
{NULL,                      	EDIT_MENU_LAST}
};

const AnalogInputs::Name externTName[] PROGMEM = { AnalogInputs::Textern };

void externalTemperatureCalibration()
{
    calibrationPoint = 0;
    SerialLog::powerOff();
    //TODO: rewrite
    ProgramData::battery.enable_externT = 1;

    AnalogInputs::powerOn(false);
    runCalibrationMenu(editExternTData, externTName, externTName, true);
    AnalogInputs::powerOff();

    SerialLog::powerOn();
}



/* internal temperature calibration menu*/

const EditMenu::StaticEditData editInternTData[] PROGMEM = {
{string_t_menu_temperature,     COND_EDITABLE,  	EANALOG_T(Tintern),             {CE_STEP_TYPE_KEY_SPEED, 0, ANALOG_CELCIUS(100)}},
{string_t_menu_adc,             COND_NOT_EDITABLE,  EANALOG_ADC(Tintern),           {0,0,0}},
{string_menu_point,             COND_POINT,     	{CP_TYPE_UNSIGNED, 0, &calibrationPoint},        {1, 0, 1}},
{NULL,                      	EDIT_MENU_LAST}
};

const AnalogInputs::Name internTName[] PROGMEM = { AnalogInputs::Tintern };

void internalTemperatureCalibration()
{
    calibrationPoint = 0;
    AnalogInputs::powerOn(false);
    runCalibrationMenu(editInternTData, internTName, internTName, true);
    AnalogInputs::powerOff();
}


} // namespace Calibration
