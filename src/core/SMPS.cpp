/*
    cheali-charger - open source firmware for a variety of LiPo chargers
    Copyright (C) 2013  Paweł Stawicki. All right reserved.

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
#include "Hardware.h"
#include "SMPS.h"

//TODO_NJ temp for test
#include "Buzzer.h"
#include "LcdPrint.h"
#include "Screen.h"

namespace SMPS {
    STATE state_;
    uint16_t value_;

    STATE getState()    { return state_; }
    bool isPowerOn()    { return getState() == CHARGING; }
    bool isWorking()    { return value_ != 0; }


    uint16_t getValue() { return value_; }

}

void SMPS::initialize()
{
    setValue(0);
    powerOff(CHARGING_COMPLETE);
}


void SMPS::setValue(uint16_t value)
{
    if(value > SMPS_UPPERBOUND_VALUE)
        value = SMPS_UPPERBOUND_VALUE;     
    value_ = SMPS::setSmoothI(value);
    hardware::setChargerValue(value_);
    AnalogInputs::resetMeasurement();
}


uint16_t SMPS::setSmoothI(uint16_t value)
{
#ifdef MAX_CURRENT_RISING

  uint16_t oldI, newI, oldValue,stepValue;
   
  oldI = calibrateValue(AnalogInputs::Ismps, AnalogInputs::getADCValue(AnalogInputs::Ismps));
  oldValue = AnalogInputs::getADCValue(AnalogInputs::Ismps);
  stepValue = AnalogInputs::reverseCalibrateValue(AnalogInputs::IsmpsValue, MAX_CURRENT_RISING);
  newI = calibrateValue(AnalogInputs::Ismps, value); //??? good?

  if ((newI > oldI) && ((newI-oldI) > MAX_CURRENT_RISING))
  {
    lcdClear();
    lcdSetCursor0_0();
    Screen::displayStrings(PSTR("SMPS"), PSTR("busy")); 
     
    for(uint16_t i=oldValue; i <= value; i=i+stepValue){
         if (i> value) i=value;
         hardware::setChargerValue(i);
         Buzzer::soundKeyboard();
         hardware::delay(MAX_CURRENT_RISING/2);
    }
    AnalogInputs::isOutStable();     
  }
#endif
  return value;
}



void SMPS::setRealValue(uint16_t I)
{
    uint16_t value = AnalogInputs::reverseCalibrateValue(AnalogInputs::IsmpsValue, I);
    setValue(value);
}

void SMPS::powerOn()
{
    if(isPowerOn())
        return;

    setValue(0);
    hardware::setChargerOutput(true);
    state_ = CHARGING;
}


void SMPS::powerOff(STATE reason)
{
    if(!isPowerOn() || reason == CHARGING)
        return;

    setValue(0);
    hardware::setChargerOutput(false);
    state_ = reason;
}
