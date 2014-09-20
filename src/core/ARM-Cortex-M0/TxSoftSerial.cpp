/*
    TxSoftSerial - Software serial library (transmit only)
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
#include "atomic.h"
#include "Hardware.h"
#include "TxSoftSerial.h"
#include "irq_priority.h"

#include "IO.h"

namespace TxSoftSerial {
//namespace Serial {

#define Tx_BUFFER_SIZE	256

#define START_BIT 0
#define STOP_BIT 512

uint8_t  pucTxBuffer[Tx_BUFFER_SIZE];
uint16_t usTxBufferLen=Tx_BUFFER_SIZE;
uint16_t usTxData;
uint8_t *pucTxpin;

volatile uint16_t usTxBufferRead;
volatile uint16_t usTxBufferWrite;


void initialize()
{
    CLK_EnableModuleClock(TMR2_MODULE);
    CLK_SetModuleClock(TMR2_MODULE,CLK_CLKSEL1_TMR2_S_HCLK,CLK_CLKDIV_UART(1));
    NVIC_SetPriority(TMR2_IRQn, SOFT_SERIAL_IRQ_PRIORITY);
    NVIC_EnableIRQ(TMR2_IRQn);
    pucTxpin=(uint8_t *)IO::getPinAddress(UART_TX_PIN);

#ifndef ENABLE_EXT_TEMP_AND_UART_COMMON_OUTPUT
    IO::pinMode(UART_TX_PIN, GPIO_PMD_OUTPUT);
#endif

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

    *(pucTxpin) = 1;     // Tx pin high (IDLE)
    usTxBufferRead = 0;
    usTxBufferWrite = 0;
    usTxData = 0;
}


void write(uint8_t ucData)
{
    uint16_t usTemp;

    usTemp = usTxBufferWrite + 1;
    if(usTemp == usTxBufferLen) {
        usTemp = 0;
    }
    while(usTemp == *(volatile uint8_t *)(&(usTxBufferRead))) {
    }
    pucTxBuffer[usTxBufferWrite] = ucData;
    usTxBufferWrite = usTemp;
}


void flush()
{
    while(usTxBufferRead != usTxBufferWrite);
}

void end()
{
    TIMER_Stop(TIMER2);
    TIMER_DisableInt(TIMER2);
}

inline uint16_t getNewData() {
    if(usTxBufferRead == usTxBufferWrite) return 0;
    uint16_t v = START_BIT + ((pucTxBuffer[usTxBufferRead]) << 1) + STOP_BIT;
    if (++usTxBufferRead == usTxBufferLen) {
        usTxBufferRead = 0;
    }
    return v;
}

extern "C"
{
void TMR2_IRQHandler(void) {
    TIMER_ClearIntFlag(TIMER2);
        if(usTxData) {
            *(pucTxpin) = usTxData & 1;
            usTxData >>= 1;
            return;
        }
    usTxData = getNewData();
}
}


} // namespace TxSoftSerial



