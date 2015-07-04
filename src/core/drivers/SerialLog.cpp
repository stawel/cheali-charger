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
#include "LcdPrint.h"
#include "Program.h"
#include "Settings.h"
#include "memory.h"
#include "Version.h"
#include "TheveninMethod.h"
#include "StackInfo.h"
#include "IO.h"
#include "SerialLog.h"
#include "AnalogInputsPrivate.h"
#include "Balancer.h"

#ifdef ENABLE_SERIAL_LOG
#include "Serial.h"
#endif //ENABLE_SERIAL_LOG

#include "Monitor.h"

namespace AnalogInputsADC {
    void debug();
}

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
            BALANCER_PORTS_GT_6(AnalogInputs::Vb7,AnalogInputs::Vb8,)
    };



void sendTime();

#ifdef ENABLE_SERIAL_LOG

void serialBegin()
{
    Serial::begin(settings.getUARTspeed());
}
void serialEnd()
{
    Serial::flush();
    Serial::end();
}

void printChar(char c)
{
    Serial::write(c);
    CRC^=c;
}

void powerOn()
{
    if(state != Off)
        return;
    if(settings.UART == Settings::Disabled)
        return;

    serialBegin();

    state = Starting;
}

void powerOff()
{
    if(state == Off)
        return;

    serialEnd();
    state = Off;
}

void send()
{
    if(state == Off)
        return;

    currentTime = Time::getMiliseconds();

    if(state == Starting) {
        startTime = currentTime;
        state = On;
    }

    currentTime -= startTime;
    sendTime();
}

void flush()
{
    if(state == Off)
        return;
    Serial::flush();
}


void doIdle()
{
    static uint16_t analogCount;
    if(!AnalogInputs::isPowerOn()) {
        analogCount = 0;
    } else {
        uint16_t c = AnalogInputs::getFullMeasurementCount();
        if(analogCount != c) {
            analogCount = c;
            send();
        }
    }
    //TODO: remove
    //AnalogInputsADC::debug();
}

#else //ENABLE_SERIAL_LOG

void serialBegin(){}
void serialEnd(){}

void printChar(char c){}
void powerOn(){}
void powerOff(){}
void send(){}
void doIdle(){}
void flush(){}


#endif


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

void printString_P(const char *s)
{
    char c;
    while(1) {
        c = pgm::read(s);
        if(!c)
            return;

        printChar(c);
        s++;
    }
}



void printLong(int32_t x)
{
    char buf[15];
    ::printLong(x, buf);
    printString(buf);
}



void sendHeader(uint16_t channel)
{
    CRC = 0;
    printChar('$');
    printUInt(channel);
    printD();
    printUInt(Program::programType+1);
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
    for(uint8_t i=0;i < sizeOfArray(channel1);i++) {
        AnalogInputs::Name name = pgm::read(&channel1[i]);
        uint16_t v = AnalogInputs::getRealValue(name);
        printUInt(v);
        printD();
    }

    for(uint8_t i=0;i<MAX_BANANCE_CELLS;i++) {
        printUInt(TheveninMethod::getReadableRthCell(i));
        printD();
    }

    printUInt(TheveninMethod::getReadableBattRth());
    printD();

    printUInt(TheveninMethod::getReadableWiresRth());
    printD();

    printUInt(Monitor::getChargeProcent());
    printD();
    printUInt(Monitor::getETATime());
    printD();

    sendEnd();
}

void sendChannel2(bool adc)
{
    sendHeader(2);
    ANALOG_INPUTS_FOR_ALL(it) {
        uint16_t v;
        if(adc) v = AnalogInputs::getAvrADCValue(it);
        else    v = AnalogInputs::getRealValue(it);
        printUInt(v);
        printD();
    }
    printUInt(Balancer::balance);
    printD();

    uint16_t pidV=0;
#ifdef ENABLE_GET_PID_VALUE
    pidV = hardware::getPIDValue();
#endif
    printUInt(pidV);
    printD();
    sendEnd();
}

void sendChannel3()
{
    sendHeader(3);
#ifdef    ENABLE_STACK_INFO //ENABLE_SERIAL_LOG
    printUInt(StackInfo::getNeverUsedStackSize());
    printD();
    printUInt(StackInfo::getFreeStackSize());
    printD();
#endif
    sendEnd();
}


void sendTime()
{
    int uart = settings.UART;
    bool adc = false;

    STATIC_ASSERT(Settings::ExtDebugAdc == 4);

    if(uart > Settings::ExtDebug) {
        adc = true;
    }
    sendChannel1();
    if(uart > Settings::Normal)
        sendChannel2(adc);

    if(uart > Settings::Debug)
        sendChannel3();

}

} //namespace SerialLog
