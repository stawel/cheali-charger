//*****************************************************************************
//
//! \file DrvSYS.h
//! \brief Contains all functions support for SYS 
//! \version v1.02.002
//! \date 21/09/2011
//! \author Nuvoton
//! \copy
//!
//! Copyright (c) Nuvoton Technology Corp. All rights reserved.
//
//*****************************************************************************
#ifndef DRVSYS_H
#define DRVSYS_H

#include "M051Series.h"

//*****************************************************************************
//
//! Macro, type and constant definitions 
//
//*****************************************************************************
#define DRVSYS_MAJOR_NUM 1
#define DRVSYS_MINOR_NUM 01
#define DRVSYS_BUILD_NUM 003

#define DRVSYS_VERSION_NUM    _SYSINFRA_VERSION(DRVSYS_MAJOR_NUM,DRVSYS_MINOR_NUM,DRVSYS_BUILD_NUM)  

//E_DRVSYS_ERR_UNLOCK_FAIL      Unlock protected address failed
//E_DRVSYS_ERR_LOCK_FAIL        Lock protected address failed
//E_DRVSYS_ERR_ARGUMENT         Incorrect Argument
//E_DRVSYS_ERR_IPSRC          Incorrect IP clock selection
//E_DRVSYS_ERR_IPDIV          Incorrect IP divider selection
//E_DRVSYS_ERR_OUT_OF_RANGE     Clock setting is out of range
//E_DRVSYS_ERR_CLKSWITCH_FAIL Switch system clock source failed
//E_DRVSYS_ERR_REG_PROTECTED    The Register Write Protection function is enabled
#define E_DRVSYS_ERR_UNLOCK_FAIL    _SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVSYS, 1)
#define E_DRVSYS_ERR_LOCK_FAIL      _SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVSYS, 2)
#define E_DRVSYS_ERR_ARGUMENT       _SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVSYS, 3)  
#define E_DRVSYS_ERR_IPSRC        _SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVSYS, 4)  
#define E_DRVSYS_ERR_IPDIV        _SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVSYS, 5)
#define E_DRVSYS_ERR_OUT_OF_RANGE   _SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVSYS, 6)
#define E_DRVSYS_ERR_CLKSWITCH_FAIL _SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVSYS, 7)
#define E_DRVSYS_ERR_REG_PROTECTED  _SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVSYS, 8) 

//*****************************************************************************
//
//! PLL Clock Source Selector   
//
//*****************************************************************************
typedef enum 
{
  E_SYS_EXTERNAL_12M = 0,
  E_SYS_INTERNAL_22M = 1, 
}E_SYS_PLL_CLKSRC;

//*****************************************************************************
//
//! IP reset   
//
//*****************************************************************************
typedef enum 
{
  E_SYS_GPIO_RST  = 1,
  E_SYS_TMR0_RST  = 2,
  E_SYS_TMR1_RST  = 3,
  E_SYS_TMR2_RST  = 4,
  E_SYS_TMR3_RST  = 5,
  E_SYS_I2C_RST   = 8,
  E_SYS_SPI0_RST  = 12,
  E_SYS_SPI1_RST  = 13,
  E_SYS_UART0_RST = 16,
  E_SYS_UART1_RST = 17,
  E_SYS_PWM03_RST = 20,
  E_SYS_PWM47_RST = 21,
  E_SYS_ADC_RST   = 28,
  E_SYS_EBI_RST   = 32
}E_SYS_IP_RST;

//*****************************************************************************
//
//! IP Clock Selector  
//
//*****************************************************************************
typedef enum 
{
  E_SYS_WDT_CLK   = 0,
  E_SYS_TMR0_CLK  = 2,
  E_SYS_TMR1_CLK  = 3,
  E_SYS_TMR2_CLK  = 4,
  E_SYS_TMR3_CLK  = 5,
  E_SYS_FDIV_CLK  = 6,
  E_SYS_I2C_CLK   = 8,
  E_SYS_SPI0_CLK  = 12,
  E_SYS_SPI1_CLK  = 13,
  E_SYS_UART0_CLK = 16,
  E_SYS_UART1_CLK = 17,
  E_SYS_PWM01_CLK = 20,
  E_SYS_PWM23_CLK = 21,
  E_SYS_PWM45_CLK = 22,
  E_SYS_PWM67_CLK = 23,
  E_SYS_ADC_CLK   = 28,
  E_SYS_ISP_CLK   = 34,
  E_SYS_EBI_CLK   = 35
}E_SYS_IP_CLK;

//*****************************************************************************
//
//! SYS IP Clcok Divider   
//
//*****************************************************************************
typedef enum 
{
  E_SYS_ADC_DIV,
  E_SYS_UART_DIV,
  E_SYS_HCLK_DIV

}E_SYS_IP_DIV;

//*****************************************************************************
//
//! SYS IP Clcok Source 
//
//*****************************************************************************
typedef enum 
{ 
  E_SYS_WDT_CLKSRC,
  E_SYS_ADC_CLKSRC,
  E_SYS_TMR0_CLKSRC,
  E_SYS_TMR1_CLKSRC,
  E_SYS_TMR2_CLKSRC,
  E_SYS_TMR3_CLKSRC,
  E_SYS_UART_CLKSRC,
  E_SYS_PWM01_CLKSRC,
  E_SYS_PWM23_CLKSRC,
  E_SYS_FRQDIV_CLKSRC,
  E_SYS_PWM45_CLKSRC,
  E_SYS_PWM67_CLKSRC

}E_SYS_IP_CLKSRC;

//*****************************************************************************
//
//! SYS Chip Clock Source   
//
//*****************************************************************************
typedef enum 
{
  E_SYS_XTL12M,
  E_SYS_OSC22M,
  E_SYS_OSC10K,
  E_SYS_PLL,
}E_SYS_CHIP_CLKSRC;

//*****************************************************************************
//
//! Power Down Type  
//
//*****************************************************************************
typedef enum 
{
  E_SYS_IMMEDIATE, 
  E_SYS_WAIT_FOR_CPU
}E_SYS_PD_TYPE;

typedef void (*BOD_CALLBACK)(void);
typedef void (*PWRWU_CALLBACK)(void);

//*****************************************************************************
//
//! Define GCR functions prototype   
//
//*****************************************************************************
void     DrvSYS_ClearClockSwitchStatus(void);
uint32_t DrvSYS_ClearResetSource(uint32_t u32Src);

void     DrvSYS_Delay(uint32_t us);
void     DrvSYS_DisableBODLowPowerMode(void);
void     DrvSYS_DisableLowVoltReset(void);
void     DrvSYS_DisablePOR(void);

void     DrvSYS_EnableBODLowPowerMode(void);
void     DrvSYS_EnableLowVoltReset(void);
void     DrvSYS_EnablePOR(void);
void     DrvSYS_EnterPowerDown(E_SYS_PD_TYPE ePDType);

uint32_t DrvSYS_GetBODState(void);
int32_t  DrvSYS_GetChipClockSourceStatus(E_SYS_CHIP_CLKSRC eClkSrc);
uint32_t DrvSYS_GetClockSwitchStatus(void);
uint32_t DrvSYS_GetExtClockFreq(void);
uint32_t DrvSYS_GetHCLKFreq(void);
uint32_t DrvSYS_GetPLLClockFreq(void);
uint32_t DrvSYS_GetPLLContent(E_SYS_PLL_CLKSRC ePllSrc, uint32_t u32PllClk);
uint32_t DrvSYS_GetResetSource(void);
uint32_t DrvSYS_GetVersion(void);

int32_t  DrvSYS_IsProtectedRegLocked(void);

int32_t  DrvSYS_LockProtectedReg(void);

int32_t  DrvSYS_Open(uint32_t u32Hclk);

uint32_t DrvSYS_ReadProductID(void);
void     DrvSYS_ResetChip(void);
void     DrvSYS_ResetCPU(void);
void     DrvSYS_ResetIP(E_SYS_IP_RST eIpRst);

void     DrvSYS_SelectBODVolt(uint8_t u8Volt);
int32_t  DrvSYS_SelectHCLKSource(uint8_t u8ClkSrcSel);
int32_t  DrvSYS_SelectIPClockSource(E_SYS_IP_CLKSRC eIpClkSrc, uint8_t u8ClkSrcSel);
void     DrvSYS_SelectPLLSource(E_SYS_PLL_CLKSRC ePllSrc);
int32_t  DrvSYS_SelectSysTickSource(uint8_t u8ClkSrcSel);
void     DrvSYS_SetBODFunction(int32_t i32Enable, int32_t i32Mode, BOD_CALLBACK bodcallbackFn);
int32_t  DrvSYS_SetClockDivider(E_SYS_IP_DIV eIpDiv , int32_t i32value);
int32_t  DrvSYS_SetFreqDividerOutput(int32_t i32Flag, uint8_t u8Divider);
void     DrvSYS_SetIPClock(E_SYS_IP_CLK eIpClk, int32_t i32Enable);
int32_t  DrvSYS_SetOscCtrl(E_SYS_CHIP_CLKSRC eClkSrc, int32_t i32Enable);
void     DrvSYS_SetPLLContent(uint32_t u32PllContent);
void     DrvSYS_SetPLLMode(int32_t i32Flag);
void     DrvSYS_SetPowerDownWakeUpInt(int32_t i32Enable, PWRWU_CALLBACK pdwucallbackFn, int32_t i32enWUDelay);

int32_t  DrvSYS_UnlockProtectedReg(void);

#endif

