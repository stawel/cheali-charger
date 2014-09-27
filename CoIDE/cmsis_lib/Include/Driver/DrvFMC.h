//*****************************************************************************
//
//! \file DrvFMC.h
//! \brief Defines and Macros for FMC API.
//! \version v1.02.002
//! \date 09/05/2011
//! \author Nuvoton
//! \copy
//!
//! Copyright (c)  2011, Nuvoton 
//! All rights reserved.
//*****************************************************************************
#ifndef _DRVFMC_H
#define _DRVFMC_H

#include "M051Series.h"


//*****************************************************************************
//
//! Define Version number
//
//*****************************************************************************
#define DRVFMC_MAJOR_NUM  1
#define DRVFMC_MINOR_NUM  01
#define DRVFMC_BUILD_NUM  003

//*****************************************************************************
//
//! Version define with SysInfra
//
//*****************************************************************************
#define DRVFMC_VERSION_NUM     _SYSINFRA_VERSION(DRVFMC_MAJOR_NUM, DRVFMC_MINOR_NUM, DRVFMC_BUILD_NUM)
                 
//*****************************************************************************
//
//! Define Error Code
//
//*****************************************************************************
// E_DRVFMC_ERR_ISP_FAIL      ISP Failed when illegal condition occurs
#define E_DRVFMC_ERR_ISP_FAIL   _SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVFMC, 1)


//*****************************************************************************
//
//! Define parameter
//
//*****************************************************************************
#define CONFIG0         0x00300000

//*****************************************************************************
//
//! Flash Boot Selector
//
//*****************************************************************************
typedef enum 
{
  E_FMC_APROM = 0, 
  E_FMC_LDROM = 1
} E_FMC_BOOTSELECT;


//*****************************************************************************
//
//! Define FMC functions prototype
//
//*****************************************************************************
void   DrvFMC_BootSelect(E_FMC_BOOTSELECT boot);

void   DrvFMC_DisableAPUpdate(void);
void   DrvFMC_DisableConfigUpdate(void);
void   DrvFMC_DisableISP(void);
void   DrvFMC_DisableLDUpdate(void);
void   DrvFMC_DisableLowFreqOptMode(void);
void   DrvFMC_DisablePowerSaving(void);

void   DrvFMC_EnableAPUpdate(void);
void   DrvFMC_EnableConfigUpdate(void);
void   DrvFMC_EnableISP(void);
void   DrvFMC_EnableLDUpdate(void);
void   DrvFMC_EnableLowFreqOptMode(void);
void   DrvFMC_EnablePowerSaving(void);
int32_t  DrvFMC_Erase(uint32_t u32addr);

E_FMC_BOOTSELECT DrvFMC_GetBootSelect(void);
uint32_t DrvFMC_GetVersion(void);

int32_t  DrvFMC_Read(uint32_t u32addr, uint32_t * u32data);
uint32_t DrvFMC_ReadDataFlashBaseAddr(void);
int32_t  DrvFMC_ReadUniqueID(uint32_t * u32data);

int32_t  DrvFMC_Write(uint32_t u32addr, uint32_t u32data);
int32_t  DrvFMC_WriteConfig(uint32_t u32data0);


#endif

