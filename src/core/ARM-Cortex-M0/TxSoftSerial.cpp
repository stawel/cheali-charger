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
#include "Hardware.h"
#include "TxSoftSerial.h"

extern "C" {
#include "M051Series.h"
}

#include "IO.h"

namespace Serial {

#define Tx_BUFFER_SIZE	256
#define Tx_FLAG_ENABLE	0x01

enum TxState {
    TxSTATE_IDLE,
    TxSTATE_DATA_0,
    TxSTATE_DATA_1,
    TxSTATE_DATA_2,
    TxSTATE_DATA_3,
    TxSTATE_DATA_4,
    TxSTATE_DATA_5,
    TxSTATE_DATA_6,
    TxSTATE_DATA_7,
    TxSTATE_START,
    TxSTATE_STOP
};

uint8_t  pucTxBuffer[Tx_BUFFER_SIZE];
uint16_t usTxBufferLen=Tx_BUFFER_SIZE;
uint8_t  ucTxNext;
uint8_t  ucTxData;
uint8_t *pucTxpin=(uint8_t *)IO::getPinAddress(T_EXTERNAL_PIN);  // ???

volatile uint16_t usTxBufferRead;
volatile uint16_t usTxBufferWrite;
volatile uint8_t  ucFlags;
volatile uint8_t  ucTxState;


void initialize()
{
    CLK_EnableModuleClock(TMR2_MODULE);
    CLK_SetModuleClock(TMR2_MODULE,CLK_CLKSEL1_TMR2_S_HCLK,CLK_CLKDIV_UART(1));
    NVIC_EnableIRQ(TMR2_IRQn);

//    IO::pinMode(UART_TX_PIN, GPIO_PMD_OUTPUT);

#ifndef ENABLE_EXT_TEMP_AND_UART_COMMON_OUTPUT
    IO::pinMode(UART_TX_PIN, GPIO_PMD_OUTPUT);
#endif

//    TxSetTxPin(UART_TX_PIN);
}

void begin(unsigned long baud)
{
#ifdef ENABLE_EXT_TEMP_AND_UART_COMMON_OUTPUT
    IO::disableFuncADC(IO::getPinBit(UART_TX_PIN));
    IO::pinMode(UART_TX_PIN, GPIO_PMD_OUTPUT);
#endif

    TIMER_Open(TIMER2, TIMER_PERIODIC_MODE, baud);
    TIMER_EnableInt(TIMER2);
    TIMER_Start(TIMER2);

    // Tx pin high.
    *(pucTxpin) = 1;

    usTxBufferRead = 0;
    usTxBufferWrite = 0;

    ucFlags |= Tx_FLAG_ENABLE;
    ucTxNext = 1;
    ucTxState = TxSTATE_IDLE;
}


void TxStateMachine()
{
    *(pucTxpin) = ucTxNext; // Write to the Tx data line.
    switch (ucTxState) {
    case TxSTATE_IDLE: {
        if (!(ucFlags & Tx_FLAG_ENABLE)) {
            break;
        } else if (usTxBufferRead != usTxBufferWrite) {
            ucTxNext = 0; // start bit
            ucTxState = TxSTATE_START;
        }
        break;
    }
    case TxSTATE_START: {
        ucTxData = pucTxBuffer[usTxBufferRead];
        ucTxNext = (ucTxData & 1) ? 1 : 0;
        ucTxState = TxSTATE_DATA_0;
        break;
    }
    case TxSTATE_DATA_0:
    case TxSTATE_DATA_1:
    case TxSTATE_DATA_2:
    case TxSTATE_DATA_3:
    case TxSTATE_DATA_4:
    case TxSTATE_DATA_5:
    case TxSTATE_DATA_6: {
        ucTxNext = (ucTxData & (1 << ucTxState)) ? 1 : 0;
        ucTxState++;
        break;
    }
    case TxSTATE_DATA_7: {
        ucTxNext = 1;
        ucTxState = TxSTATE_STOP;
        break;
    }
    case TxSTATE_STOP: {
        usTxBufferRead++;
        if (usTxBufferRead == usTxBufferLen) {
            usTxBufferRead = 0;
        }
        if (!(ucFlags & Tx_FLAG_ENABLE)) {
            ucTxState = TxSTATE_IDLE;
        }
        else if (usTxBufferRead != usTxBufferWrite) { // more Data
            ucTxNext = 0;
            ucTxState = TxSTATE_START;
        }
        else {
            ucTxState = TxSTATE_IDLE;
        }
        break;
    }
    }
}

//void TxEnable()
//{
//    ucFlags |= Tx_FLAG_ENABLE;
//}
//
//void TxSetTxPin(uint8_t pinNumber)
//{
//    // Set pointer to Tx pin
//    pucTxpin=(uint8_t *)IO::getPinAddress(pinNumber);
//}
//
//bool TxBafferSpace()
//{
//    uint16_t usTemp;
//
//    usTemp = usTxBufferWrite + 1;
//    if(usTemp == usTxBufferLen)	{
//        usTemp = 0;
//    }
//    return((usTxBufferRead == usTemp) ? false : true);
//}

void write(uint8_t ucData)
{
    uint16_t usTemp;

    usTemp = usTxBufferWrite + 1;
    if(usTemp == usTxBufferLen) {
        usTemp = 0;
    }
    while(usTemp == *(volatile uint8_t *)(&(usTxBufferRead)))	{
    }

    pucTxBuffer[usTxBufferWrite] = ucData;
    usTxBufferWrite = usTemp;
}

bool TxBusy()
{
    return(((ucTxState == TxSTATE_IDLE) &&
            (((ucFlags & Tx_FLAG_ENABLE) == 0) ||
                    (usTxBufferRead == usTxBufferWrite))) ?
                            false : true);
}

void TxEnd()
{
    while(TxBusy()) {
    }
    ucFlags &= ~(Tx_FLAG_ENABLE);
    TIMER_Stop(TIMER2);
    TIMER_DisableInt(TIMER2);
}

void flush()
{
    while(TxBusy());
}

void end()
{
    ucFlags &= ~(Tx_FLAG_ENABLE);
    TIMER_Stop(TIMER2);
    TIMER_DisableInt(TIMER2);
}


} // namespace Serial


extern "C"
{
void TMR2_IRQHandler(void) {
    TIMER_ClearIntFlag(TIMER2);
    Serial::TxStateMachine();
}
}

