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
#include "Serial.h"
#include "SerialLog.h"
#include "Hardware.h"
#include "Timer.h"
#include "LcdPrint.h"
#include "Program.h"
#include "Settings.h"
#include "memory.h"
#include "Version.h"
#include "TheveninMethod.h"
#include "StackInfo.h"

namespace SerialLog {
    enum State { On, Off, Starting };
    uint32_t startTime;
    uint32_t currentTime;

    State state = Off;
    uint8_t CRC;
    const AnalogInputs::Name channel1[] PROGMEM = {
            AnalogInputs::VoutBalancer,
            AnalogInputs::Iout,
            AnalogInputs::Cout,
            AnalogInputs::Pout,
            AnalogInputs::Eout,
            AnalogInputs::Textern,
            AnalogInputs::Tintern,
            AnalogInputs::Vin,
            AnalogInputs::Vb1,
            AnalogInputs::Vb2,
            AnalogInputs::Vb3,
            AnalogInputs::Vb4,
            AnalogInputs::Vb5,
            AnalogInputs::Vb6,
    };



void sendTime();

void serialBegin()
{
    Serial.begin(settings.getUARTspeed());
}
void serialEnd()
{
    Serial.flush();
    Serial.end();
    pinMode(10, INPUT);
}

void powerOn()
{

#ifdef ENABLE_SERIAL_LOG
    if(state != Off)
        return;
    if(settings.UART_ == Settings::Disabled)
        return;

#ifndef ENABLE_SERIAL_LOG_WAIT
    serialBegin();
#endif

    state = Starting;
#endif //ENABLE_SERIAL_LOG
}

void powerOff()
{
#ifdef ENABLE_SERIAL_LOG

    if(state == Off)
        return;

#ifndef ENABLE_SERIAL_LOG_WAIT
    serialEnd();
#endif

    state = Off;
#endif //ENABLE_SERIAL_LOG
}

void send()
{
#ifdef ENABLE_SERIAL_LOG

    if(state == Off)
        return;

#ifdef ENABLE_SERIAL_LOG_WAIT
    serialBegin();
#endif

    currentTime = Timer::getMiliseconds();

    if(state == Starting) {
        startTime = currentTime;
        state = On;
    }

    currentTime -= startTime;
    sendTime();

#ifdef ENABLE_SERIAL_LOG_WAIT
    serialEnd();
#endif

#endif //ENABLE_SERIAL_LOG
}


void printChar(char c)
{
    Serial.print(c);
    CRC^=c;
}
void printD()
{
    printChar(';');
}

void printString(const char *s)
{
    while(*s) {
        printChar(*s);
        s++;
    }
}

void printUInt(uint16_t x)
{
    char buf[8];
    char *str = buf;
    uint8_t maxSize = 7;
    ::printUInt(str, maxSize, x);
    printString(buf);
}

void printULong(uint32_t x)
{
    char buf[8];
    char *str = buf;
    uint8_t maxSize = 7;
    ::printULong(str, maxSize, x);
    printString(buf);
}



void sendHeader(uint16_t channel)
{
    CRC = 0;
    printChar('$');
    printUInt(channel);
    printD();
    printUInt(Program::programState_);   //state
    printD();

    printUInt(currentTime/1000);   //timestamp
    printChar('.');
    printUInt((currentTime/100)%10);   //timestamp
    printD();
}


void printNL()
{
    printChar('\r');
    printChar('\n');
}

void sendEnd()
{
    //checksum
    printUInt(CRC);
    printNL();
}

void sendChannel1()
{
    sendHeader(1);
    //analog inputs
    for(int8_t i=0;i < sizeOfArray(channel1);i++) {
        printUInt(AnalogInputs::getRealValue(pgm::read(&channel1[i])));
        printD();
    }

    for(int8_t i=0;i<MAX_BANANCE_CELLS;i++) {
        printUInt(TheveninMethod::getReadableRthCell(i));
        printD();
    }

    printUInt(TheveninMethod::getReadableBattRth());
    printD();

    printUInt(TheveninMethod::getReadableWiresRth());
    printD();

    sendEnd();
}

void sendChannel2()
{
    sendHeader(2);
    FOR_ALL_INPUTS(it) {
        printUInt(AnalogInputs::getRealValue(it));
        printD();
    }
    printUInt(Balancer::balance_);
    printD();

    uint16_t pidV=0;
#ifdef ENABLE_GET_PID_VALUE
    pidV = hardware::getPIDValue();
#endif
    printUInt(TheveninMethod::idebug_);
    printD();
/*    printUInt(Screen::calculateBattRth());
    printD();

    printUInt(Screen::calculateWiresRth());
    printD();

    for(int8_t i=0;i<MAX_BANANCE_CELLS;i++) {
        printUInt(Screen::calculateRthCell(i));
        printD();
    }
*/
    sendEnd();
}

void sendChannel3()
{
    sendHeader(3);
    FOR_ALL_PHY_INPUTS(it) {
        printUInt(AnalogInputs::getAvrADCValue(it));
        printD();
    }
    printUInt(StackInfo::getNeverUsedStackSize());
    printD();
    printUInt(StackInfo::getFreeStackSize());
    printD();
    sendEnd();
}


void sendTime()
{
    sendChannel1();
    if(settings.UART_>1)
        sendChannel2();

    if(settings.UART_>2)
        sendChannel3();

}

void printV(char c,uint8_t nr, AnalogInputs::ValueType value)
{
    printChar(c);
    printUInt(nr);
    printChar('=');
    printUInt(value);
    printNL();
}

void dumpCalibration()
{
    printString("V=" CHEALI_CHARGER_VERSION_STRING);
    printNL();
    printV('E',0,CHEALI_CHARGER_EEPROM_VERSION);

    AnalogInputs::CalibrationPoint p;
    FOR_ALL_PHY_INPUTS(it) {
        AnalogInputs::getCalibrationPoint(p,it,0);
        printV('a',it, p.x);
        printV('r',it, p.y);
        AnalogInputs::getCalibrationPoint(p,it,1);
        printV('A',it, p.x);
        printV('R',it, p.y);
    }
}
void sendCalibration()
{
    serialBegin();
    dumpCalibration();
    printNL();
    dumpCalibration();
    printNL();
    serialEnd();
}


void debugSerial(uint16_t a, uint16_t b)
{
    //dont working. Why??


    //temporary for debugging
    SerialLog::powerOn();
    serialBegin();
    printUInt(a);
    printChar('-');
    printUInt(b);
    printNL();
    serialEnd();
    SerialLog::powerOff();
}




} //namespace SerialLog
