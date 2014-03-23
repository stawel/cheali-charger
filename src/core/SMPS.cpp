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

#include "LcdPrint.h"
#include "Screen.h"
#include "Settings.h"

namespace SMPS {
    STATE state_;
    uint16_t value_;
#ifdef MAX_CURRENT_RISING    
    uint16_t oldI, newI,stepValue;
#endif

    STATE getState()    { return state_; }
    bool isPowerOn()    { return getState() == CHARGING; }
    bool isWorking()    { return value_ != 0; }


    uint16_t getValue() { return value_; }

}

void SMPS::initialize()
{
    value_ = 0;
    setValue(0);
    powerOff(CHARGING_COMPLETE);
}


void SMPS::setValue(uint16_t value)
{
    if (settings.calibratedState_ >= 7) //disable limit if uncalibrated.
    {
      if(value > settings.SMPS_Upperbound_Value_) value = settings.SMPS_Upperbound_Value_;     
    }
    
    value_ = SMPS::setSmoothI(value, value_);
    hardware::setChargerValue(value_);
    AnalogInputs::resetMeasurement();
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
    //reset rising value
    value_ = 0;
    setValue(0);
    hardware::setChargerOutput(true);
    state_ = CHARGING;
}


void SMPS::powerOff(STATE reason)
{
    if(!isPowerOn() || reason == CHARGING)
        return;

    setValue(0);
    //reset rising value
    value_ = 0;
    hardware::setChargerOutput(false);
    state_ = reason;
}

uint16_t SMPS::setSmoothI(uint16_t value, uint16_t oldValue)
{
#ifdef MAX_CURRENT_RISING 
 if (settings.calibratedState_ >= 7) //enabled if  calibrated.
    {
            oldI = calibrateValue(AnalogInputs::IsmpsValue, oldValue);
            stepValue = (AnalogInputs::reverseCalibrateValue(AnalogInputs::IsmpsValue, MAX_CURRENT_RISING))/2;
            newI = calibrateValue(AnalogInputs::IsmpsValue, value);
            
          //rising
            if ((newI > oldI) && ((newI-oldI) > MAX_CURRENT_RISING))
            {
              lcdClear();
              lcdSetCursor0_0();
              Screen::displayStrings(PSTR("SMPS up"), NULL);
              for(uint16_t i=oldValue; i <= value; i=i+stepValue){
                   if (i> value) //safety
                   {
                     i=value; 
                   }
                   hardware::setChargerValue(i);
                   Timer::delay(500);
              }
              AnalogInputs::isOutStable();    //resistance measure? 
            }
            
          //falling
            if ((oldI > newI) && ((oldI-newI) > MAX_CURRENT_RISING))
            {
              lcdClear();
              lcdSetCursor0_0();
              Screen::displayStrings(PSTR("SMPS down"), NULL);
              for(uint16_t i=value; i <= oldValue; i=i+stepValue){
                   if (i> oldValue)   //safety
                   {
                     i=oldValue;  
                   } 
                   hardware::setChargerValue(oldValue-i);
                   Timer::delay(500);
              }
              AnalogInputs::isOutStable();    //resistance measure? 
            }
  }          
#endif 
  return value;
}
