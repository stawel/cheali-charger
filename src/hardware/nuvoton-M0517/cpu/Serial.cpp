/*
    Serial - wrapper for Hardware OR Software serial library (transmit only)
    Copyright (c) 2014 Sasa Mihajlovic.  All right reserved.

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
#include "Settings.h"

#ifdef ENABLE_TX_HW_SERIAL
#include "TxHardSerial.h"
#endif

#include "TxSoftSerial.h"

namespace Serial {
void empty(){}
void emptyUint8(uint8_t c){}

void (*write)(uint8_t c) = emptyUint8;
void (*flush)() = empty;
void (*end)() = empty;

#define Tx_BUFFER_SIZE  256
uint8_t  txBuffer[Tx_BUFFER_SIZE];

void  begin(unsigned long baud)
{
#ifdef ENABLE_TX_HW_SERIAL
    if(settings.UARToutput != Settings::Software) {
        write = &(TxHardSerial::write);
        flush = &(TxHardSerial::flush);
        end = &(TxHardSerial::end);
        TxHardSerial::begin(baud);
    } else {
        write = &(TxSoftSerial::write);
        flush = &(TxSoftSerial::flush);
        end = &(TxSoftSerial::end);
        TxSoftSerial::begin(baud);
    }
#else
    write = &(TxSoftSerial::write);
    flush = &(TxSoftSerial::flush);
    end = &(TxSoftSerial::end);
    TxSoftSerial::begin(baud);
#endif
};


void  initialize() {
#ifdef ENABLE_TX_HW_SERIAL
    TxHardSerial::initialize();
#endif
    TxSoftSerial::initialize();
}

} // namespace Serial
