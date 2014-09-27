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

void  begin(unsigned long baud)
{
#ifdef ENABLE_TX_HW_SERIAL
	if(settings.UARTinput_ == Settings::Hardware) {
		TxHardSerial::begin(baud);
	} else {
		TxSoftSerial::begin(baud);
	}
#else
	TxSoftSerial::begin(baud);
#endif
};

void  write(uint8_t c)
{
#ifdef ENABLE_TX_HW_SERIAL
	if(settings.UARTinput_ == Settings::Hardware) {
		TxHardSerial::write(c);
	} else {
		TxSoftSerial::write(c);
	}
#else
	TxSoftSerial::write(c);
#endif
}

void  flush()
{
#ifdef ENABLE_TX_HW_SERIAL
	if(settings.UARTinput_ == Settings::Hardware) {
		TxHardSerial::flush();
	} else {
		TxSoftSerial::flush();
	}
#else
	TxSoftSerial::flush();
#endif
}
void  end() {
#ifdef ENABLE_TX_HW_SERIAL
	if(settings.UARTinput_ == Settings::Hardware) {
		TxHardSerial::end();
	} else {
		TxSoftSerial::end();
	}
#else
	TxSoftSerial::end();
#endif
}

void  initialize() {
#ifdef ENABLE_TX_HW_SERIAL
	TxHardSerial::initialize();
#endif
	TxSoftSerial::initialize();
}

} // namespace Serial
