//*****************************************************************************
//
//! \file DrvGPIO.h
//! \brief Defines and Macros for GPIO API.
//! \version v1.02.002
//! \date 09/28/2011
//! \author Nuvoton
//! \copy
//!
//! Copyright (c)  2011, Nuvoton 
//! All rights reserved.
//
//*****************************************************************************
#ifndef DRVGPIO_H
#define DRVGPIO_H

#include "M051Series.h"

//*****************************************************************************
//
//! Define Version number
//
//*****************************************************************************
#define DRVGPIO_MAJOR_NUM        1
#define DRVGPIO_MINOR_NUM        01
#define DRVGPIO_BUILD_NUM        005

//*****************************************************************************
//
//! Version define with SysInfra
//
//*****************************************************************************
#define DRVGPIO_VERSION_NUM     _SYSINFRA_VERSION(DRVGPIO_MAJOR_NUM, DRVGPIO_MINOR_NUM, DRVGPIO_BUILD_NUM)
                               
//*****************************************************************************
//
//! Define Error Code
//
//*****************************************************************************
//
// Error code 
//
#define E_DRVGPIO_ARGUMENT      _SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVGPIO, 1)
#define E_DRVGPIO_GROUP_PIN     _SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVGPIO, 2)
#define E_DRVGPIO_BUSY          _SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVGPIO, 3)   

//
// Define GPIO[n] Pin I/O Bit Output Control. 
//
#define _PORT_DOUT(PortNum, PinNum)      (*((volatile uint32_t *) ((PORT_BIT_DOUT+(0x20*PortNum)) + (0x4*PinNum))))    
#define P00_DOUT                _PORT_DOUT(0, 0)
#define P01_DOUT                _PORT_DOUT(0, 1)
#define P02_DOUT                _PORT_DOUT(0, 2)
#define P03_DOUT                _PORT_DOUT(0, 3)
#define P04_DOUT                _PORT_DOUT(0, 4)
#define P05_DOUT                _PORT_DOUT(0, 5)
#define P06_DOUT                _PORT_DOUT(0, 6)
#define P07_DOUT                _PORT_DOUT(0, 7)
#define P10_DOUT                _PORT_DOUT(1, 0)
#define P11_DOUT                _PORT_DOUT(1, 1)
#define P12_DOUT                _PORT_DOUT(1, 2)
#define P13_DOUT                _PORT_DOUT(1, 3)
#define P14_DOUT                _PORT_DOUT(1, 4)
#define P15_DOUT                _PORT_DOUT(1, 5)
#define P16_DOUT                _PORT_DOUT(1, 6)
#define P17_DOUT                _PORT_DOUT(1, 7)
#define P20_DOUT                _PORT_DOUT(2, 0)
#define P21_DOUT                _PORT_DOUT(2, 1)
#define P22_DOUT                _PORT_DOUT(2, 2)
#define P23_DOUT                _PORT_DOUT(2, 3)
#define P24_DOUT                _PORT_DOUT(2, 4)
#define P25_DOUT                _PORT_DOUT(2, 5)
#define P26_DOUT                _PORT_DOUT(2, 6)
#define P27_DOUT                _PORT_DOUT(2, 7)
#define P30_DOUT                _PORT_DOUT(3, 0)
#define P31_DOUT                _PORT_DOUT(3, 1)
#define P32_DOUT                _PORT_DOUT(3, 2)
#define P33_DOUT                _PORT_DOUT(3, 3)
#define P34_DOUT                _PORT_DOUT(3, 4)
#define P35_DOUT                _PORT_DOUT(3, 5)
#define P36_DOUT                _PORT_DOUT(3, 6)
#define P37_DOUT                _PORT_DOUT(3, 7)
#define P40_DOUT                _PORT_DOUT(4, 0)
#define P41_DOUT                _PORT_DOUT(4, 1)
#define P42_DOUT                _PORT_DOUT(4, 2)
#define P43_DOUT                _PORT_DOUT(4, 3)
#define P44_DOUT                _PORT_DOUT(4, 4)
#define P45_DOUT                _PORT_DOUT(4, 5)
#define P46_DOUT                _PORT_DOUT(4, 6)
#define P47_DOUT                _PORT_DOUT(4, 7)

typedef void (*P0P1_CALLBACK)(uint32_t u32P0Status, uint32_t u32P1Status);
typedef void (*P2P3P4_CALLBACK)(uint32_t u32P2Status, uint32_t u32P3Status, uint32_t u32P4Status);
typedef void (*EINT_CALLBACK)(void);

typedef enum 
{
    E_PORT0    = 0,
    E_PORT1, 
    E_PORT2, 
    E_PORT3, 
    E_PORT4
} E_DRVGPIO_PORT;

typedef enum 
{
    E_PIN0     = 0,
    E_PIN1,
    E_PIN2,
    E_PIN3,
    E_PIN4,
    E_PIN5,
    E_PIN6,
    E_PIN7
} E_DRVGPIO_PIN;

typedef enum 
{
    E_EINT0_PIN = 2,
    E_EINT1_PIN = 3
} E_DRVGPIO_EXT_INT_PIN;       

typedef enum 
{
    E_IO_INPUT,
    E_IO_OUTPUT,
    E_IO_OPENDRAIN,
    E_IO_QUASI
} E_DRVGPIO_IO;

typedef enum 
{
    E_IO_RISING,
    E_IO_FALLING,
    E_IO_BOTH_EDGE
} E_DRVGPIO_INT_TYPE;

typedef enum
{
    E_MODE_EDGE,
    E_MODE_LEVEL
} E_DRVGPIO_INT_MODE;

typedef enum
{
    E_DBCLKSRC_HCLK = 0, 
    E_DBCLKSRC_10K = 1
} E_DRVGPIO_DBCLKSRC;       

//*****************************************************************************
//
//! SYS IP Clcok Selector
//
//*****************************************************************************
typedef enum
{
    E_FUNC_GPIO,    E_FUNC_CLKO,    E_FUNC_I2C,     E_FUNC_SPI0,    E_FUNC_SPI1,    
    E_FUNC_ADC0,    E_FUNC_ADC1,    E_FUNC_ADC2,    E_FUNC_ADC3,    E_FUNC_ADC4,    
    E_FUNC_ADC5,    E_FUNC_ADC6,    E_FUNC_ADC7,    E_FUNC_EXTINT0, E_FUNC_EXTINT1, 
    E_FUNC_TMR0,    E_FUNC_TMR1,    E_FUNC_TMR2,    E_FUNC_TMR3,    E_FUNC_UART0,   
    E_FUNC_UART1,   E_FUNC_PWM01,   E_FUNC_PWM23,   E_FUNC_PWM45,   E_FUNC_PWM67,   
    E_FUNC_PWM01_P4,E_FUNC_PWM23_P4,E_FUNC_EBI_8B,  E_FUNC_EBI_16B, E_FUNC_ICE     
} E_DRVGPIO_FUNC;
              
//*****************************************************************************
//
//! Define GPIO functions prototype
//
//*****************************************************************************
void DrvGPIO_Open(E_DRVGPIO_PORT port, uint32_t pin, E_DRVGPIO_IO mode);
void DrvGPIO_Close(E_DRVGPIO_PORT port, uint32_t pin);
int32_t DrvGPIO_SetBit(E_DRVGPIO_PORT port, uint32_t pin);
int32_t DrvGPIO_GetBit(E_DRVGPIO_PORT port, uint32_t pin);
int32_t DrvGPIO_ClrBit(E_DRVGPIO_PORT port, uint32_t pin);
int32_t DrvGPIO_SetPortBits(E_DRVGPIO_PORT port, int32_t i32PortValue);
int32_t DrvGPIO_GetPortBits(E_DRVGPIO_PORT port);
int32_t DrvGPIO_GetDoutBit(E_DRVGPIO_PORT port, uint32_t pin);
int32_t DrvGPIO_GetPortDoutBits(E_DRVGPIO_PORT port);
int32_t DrvGPIO_SetBitMask(E_DRVGPIO_PORT port, uint32_t pin);
int32_t DrvGPIO_GetBitMask(E_DRVGPIO_PORT port, uint32_t pin);
int32_t DrvGPIO_ClrBitMask(E_DRVGPIO_PORT port, uint32_t pin);
int32_t DrvGPIO_SetPortMask(E_DRVGPIO_PORT port, int32_t i32PortValue);
int32_t DrvGPIO_GetPortMask(E_DRVGPIO_PORT port);
int32_t DrvGPIO_ClrPortMask(E_DRVGPIO_PORT port, int32_t i32PortValue);
int32_t DrvGPIO_EnableDigitalInputBit(E_DRVGPIO_PORT port, uint32_t pin);
int32_t DrvGPIO_DisableDigitalInputBit(E_DRVGPIO_PORT port, uint32_t pin);
int32_t DrvGPIO_EnableDebounce(E_DRVGPIO_PORT port, uint32_t pin);
int32_t DrvGPIO_DisableDebounce(E_DRVGPIO_PORT port, uint32_t pin);
int32_t DrvGPIO_SetDebounceTime(uint32_t u32CycleSelection, E_DRVGPIO_DBCLKSRC ClockSource);
int32_t DrvGPIO_GetDebounceSampleCycle(void);
int32_t DrvGPIO_EnableInt(E_DRVGPIO_PORT port, uint32_t pin, E_DRVGPIO_INT_TYPE Type, E_DRVGPIO_INT_MODE Mode);
int32_t DrvGPIO_DisableInt(E_DRVGPIO_PORT port, uint32_t pin);
void DrvGPIO_SetIntCallback(P0P1_CALLBACK pfP0P1Callback, P2P3P4_CALLBACK pfP2P3P4Callback);
int32_t DrvGPIO_EnableEINT(E_DRVGPIO_EXT_INT_PIN pin, E_DRVGPIO_INT_TYPE Type, E_DRVGPIO_INT_MODE Mode, EINT_CALLBACK pfEINTCallback);
int32_t DrvGPIO_DisableEINT(E_DRVGPIO_EXT_INT_PIN pin);
int32_t DrvGPIO_GetIntStatus(E_DRVGPIO_PORT port);
int32_t DrvGPIO_InitFunction(E_DRVGPIO_FUNC function);
int32_t DrvGPIO_GetVersion(void);

#endif








