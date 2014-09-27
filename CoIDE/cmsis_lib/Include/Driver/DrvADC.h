//*****************************************************************************
//
//! \file DrvADC.h
//! \brief Defines and Macros for ADC API.
//! \version v1.02.002
//! \date 09/05/2011
//! \author Nuvoton
//! \copy
//!
//! Copyright (c)  2011, Nuvoton 
//! All rights reserved.
//*****************************************************************************
#ifndef __DRVADC_H__
#define __DRVADC_H__


//*****************************************************************************
//
//! Include related headers
//
//*****************************************************************************
#include "M051Series.h"
#include "System/SysInfra.h"

//
// Address definition 
//
#define ADC_ADSR             (ADC_BASE       + 0x30)

//*****************************************************************************
//
//! Macro, type and constant definitions
//
//*****************************************************************************
//
// version definition with SysInfra 
//
#define    DRVADC_MAJOR_NUM 1
#define    DRVADC_MINOR_NUM 01
#define    DRVADC_BUILD_NUM 002
#define DRVADC_VERSION_NUM    _SYSINFRA_VERSION(DRVADC_MAJOR_NUM, DRVADC_MINOR_NUM, DRVADC_BUILD_NUM)

//
// error code definition 
//
#define E_DRVADC_ARGUMENT     _SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVADC, 1)

typedef enum {ADC_SINGLE_END, ADC_DIFFERENTIAL} E_ADC_INPUT_MODE;
typedef enum {ADC_SINGLE_OP, ADC_BURST_OP, ADC_SINGLE_CYCLE_OP, ADC_CONTINUOUS_OP} E_ADC_OPERATION_MODE;
typedef enum {EXTERNAL_12MHZ=0, INTERNAL_PLL=1, INTERNAL_RC22MHZ=2} E_ADC_CLK_SRC;
typedef enum {LOW_LEVEL=0, HIGH_LEVEL=1, FALLING_EDGE=2, RISING_EDGE=3} E_ADC_EXT_TRI_COND;
typedef enum {EXTERNAL_INPUT_SIGNAL, INTERNAL_BANDGAP} E_ADC_CH7_SRC;
typedef enum {LESS_THAN, GREATER_OR_EQUAL} E_ADC_CMP_CONDITION;

typedef void (DRVADC_ADC_CALLBACK)(uint32_t u32UserData);
typedef void (DRVADC_ADCMP0_CALLBACK)(uint32_t u32UserData);
typedef void (DRVADC_ADCMP1_CALLBACK)(uint32_t u32UserData);

#define _DRVADC_CONV() (ADC->ADCR.ADST=1)
#define _DRVADC_GET_ADC_INT_FLAG() ADC->ADSR.ADF
#define _DRVADC_GET_CMP0_INT_FLAG() ADC->ADSR.CMPF0
#define _DRVADC_GET_CMP1_INT_FLAG() ADC->ADSR.CMPF1
#define _DRVADC_CLEAR_ADC_INT_FLAG() (outpw(ADC_ADSR, (inpw(ADC_ADSR)&(~0x7))|0x1))
#define _DRVADC_CLEAR_CMP0_INT_FLAG() (outpw(ADC_ADSR, (inpw(ADC_ADSR)&(~0x7))|0x2))
#define _DRVADC_CLEAR_CMP1_INT_FLAG() (outpw(ADC_ADSR, (inpw(ADC_ADSR)&(~0x7))|0x4))

//*****************************************************************************
//
//! Define Function Prototypes
//
//*****************************************************************************
void DrvADC_Open(E_ADC_INPUT_MODE InputMode, E_ADC_OPERATION_MODE OpMode, uint8_t u8ChannelSelBitwise, E_ADC_CLK_SRC ClockSrc, uint8_t u8AdcDivisor);
void DrvADC_Close(void);
void DrvADC_SetADCChannel(uint8_t u8ChannelSelBitwise);
void DrvADC_ConfigADCChannel7(E_ADC_CH7_SRC Ch7Src);
void DrvADC_SetADCInputMode(E_ADC_INPUT_MODE InputMode);
void DrvADC_SetADCOperationMode(E_ADC_OPERATION_MODE OpMode);
void DrvADC_SetADCClkSrc(E_ADC_CLK_SRC ClockSrc);
void DrvADC_SetADCDivisor(uint8_t u8AdcDivisor);
void DrvADC_EnableADCInt(DRVADC_ADC_CALLBACK callback, uint32_t u32UserData);
void DrvADC_DisableADCInt(void);
void DrvADC_EnableADCCmp0Int(DRVADC_ADCMP0_CALLBACK callback, uint32_t u32UserData);
void DrvADC_DisableADCCmp0Int(void);
void DrvADC_EnableADCCmp1Int(DRVADC_ADCMP1_CALLBACK callback, uint32_t u32UserData);
void DrvADC_DisableADCCmp1Int(void);
uint32_t DrvADC_GetConversionRate(void);
void DrvADC_EnableExtTrigger(E_ADC_EXT_TRI_COND TriggerCondition);
void DrvADC_DisableExtTrigger(void);
void DrvADC_StartConvert(void);
void DrvADC_StopConvert(void);
uint32_t DrvADC_IsConversionDone(void);
int32_t DrvADC_GetConversionData(uint8_t u8ChannelNum);
uint32_t DrvADC_IsDataValid(uint8_t u8ChannelNum);
uint32_t DrvADC_IsDataOverrun(uint8_t u8ChannelNum);
int32_t DrvADC_EnableADCCmp0(uint8_t u8CmpChannelNum, E_ADC_CMP_CONDITION CmpCondition, uint16_t u16CmpData, uint8_t CmpMatchCount);
int32_t DrvADC_EnableADCCmp1(uint8_t u8CmpChannelNum, E_ADC_CMP_CONDITION CmpCondition, uint16_t u16CmpData, uint8_t CmpMatchCount);
void DrvADC_DisableADCCmp0(void);
void DrvADC_DisableADCCmp1(void);
void DrvADC_EnableSelfCalibration(void);
uint32_t DrvADC_IsCalibrationDone(void);
void DrvADC_DisableSelfCalibration(void);
uint32_t DrvADC_GetVersion (void);

#endif
