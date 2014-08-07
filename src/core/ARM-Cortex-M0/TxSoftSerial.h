/*
    TxSoftSerial - Software serial library (transmit only)
    Copyright (c) 2014 Saša Mihajloviæ.  All right reserved.

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
#ifndef TxSOFSERIAL_H__
#define TxSOFSERIAL_H__

#include <inttypes.h>

namespace Serial {
    void  begin(unsigned long baud);
    void  write(uint8_t c);
    void  flush();
    void  end();
    void  initialize();
    void  TxStateMachine();
    void  TxEnd();
    bool  TxBusy();
    extern volatile uint16_t usTxBufferRead;
    extern volatile uint16_t usTxBufferWrite;
    extern volatile uint8_t  ucFlags;
    extern volatile uint8_t  ucTxState;
    extern uint8_t  ucTxNext;
    extern uint8_t  ucTxData;

} // namespace Serial

#endif //  TxSOFSERIAL_H__

