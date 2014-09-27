//*****************************************************************************
//
//! \file DrvFMC.c
//! \brief Driver for the FMC Controller.
//! \version v1.02.002
//! \date 09/05/2011
//! \author Nuvoton
//! \copy
//!
//! Copyright (c)  2011, Nuvoton 
//! All rights reserved.
//
//*****************************************************************************
#include <stdio.h>
#include "DrvFMC.h"


//*****************************************************************************
//
//! \brief Write data into Flash include APROM, LDROM, Data Flash, and Config
//!  The corresponding functions in Config0 are listed in FMC section of data
//!  sheet.   
//!
//! \param u32addr Flash address include APROM, LDROM, Data Flash, and Config
//!
//! \param u32data Woed Data 
//!
//! \return 0 Success
//!         <0 Failed when illegal condition occurs  
//!
//! \note Please make sure that Register Write-Protection Function has
//!  been disabled before using this function. User can check the status of
//!  Register Write-Protection Function with DrvSYS_IsProtectedRegLocked().
//
//*****************************************************************************
int32_t DrvFMC_Write(uint32_t u32addr, uint32_t u32data)
{
    FMC->ISPCMD.FCTRL = 1;
    FMC->ISPCMD.FCEN = 0;
    FMC->ISPCMD.FOEN = 1;
    FMC->ISPADR = u32addr;
    FMC->ISPDAT = u32data;
    FMC->ISPTRG.ISPGO = 1;  
    __ISB();
    while (FMC->ISPTRG.ISPGO);
    
    if (FMC->ISPCON.ISPFF == 1)
    {
        FMC->ISPCON.ISPFF = 1;
        return E_DRVFMC_ERR_ISP_FAIL;
    }
    
    return 0;
}

//*****************************************************************************
//
//! \brief Read data from Flash include APROM, LDROM, Data Flash, and Config. 
//!
//! \param u32addr Flash address include APROM, LDROM, Data Flash, and Config
//! \param u32data Word Data  
//!
//! \return 0 Success, 
//!         <0 Failed when illegal condition occurs  
//!
//! \note Please make sure that Register Write-Protection Function has been
//!  disabled before using this function. User can check the status of Register
//!  Write-Protection Function with DrvSYS_IsProtectedRegLocked().
//
//*****************************************************************************
int32_t DrvFMC_Read(uint32_t u32addr, uint32_t * u32data)
{ 
    FMC->ISPCMD.FCTRL = 0;
    FMC->ISPCMD.FCEN = 0;
    FMC->ISPCMD.FOEN = 0;
    FMC->ISPADR = u32addr;
    FMC->ISPDAT = 0;
    FMC->ISPTRG.ISPGO = 1;    
    __ISB();
    while (FMC->ISPTRG.ISPGO);

    if (FMC->ISPCON.ISPFF == 1)
    {
        FMC->ISPCON.ISPFF = 1;
        return E_DRVFMC_ERR_ISP_FAIL;
    }
    
    *u32data = FMC->ISPDAT;
    return 0;
}

//*****************************************************************************
//
//! \brief Page Erase Flash include APROM, LDROM, Data Flash, and Config. The
//!  erase unit is 512 bytes.  
//!
//! \param u32addr Flash page base address or Config0 address  
//!
//! \return 0 Success, 
//!         <0 Failed when illegal condition occurs  
//!
//! \note Please make sure that Register Write -Protection Function has been 
//!  disabled before using this function. User can check the status of Register 
//!  Write-Protection Function with DrvSYS_IsProtectedRegLocked(). 
//
//*****************************************************************************
int32_t DrvFMC_Erase(uint32_t u32addr)
{
    FMC->ISPCMD.FCTRL = 2;
    FMC->ISPCMD.FCEN = 0;
    FMC->ISPCMD.FOEN = 1;
    FMC->ISPADR = u32addr;
    FMC->ISPTRG.ISPGO = 1;  
    __ISB();
    while (FMC->ISPTRG.ISPGO);
    
    if (FMC->ISPCON.ISPFF == 1)
    {
        FMC->ISPCON.ISPFF = 1;
        return E_DRVFMC_ERR_ISP_FAIL;
    }
    
    return 0;
}


//*****************************************************************************
//
//! \brief Enable ISP function. This function will check if internal oscillator 
//!  22M is enabled or not. If not, this function will enable 22M oscillator 
//!  automatically.  
//!
//! \param None 
//!
//! \return None  
//!
//! \note Please make sure that Register Write-Protection Function has been 
//! disabled before using this function to enable ISP function. User can check 
//! the status of Register Write-Protection Function with DrvSYS_IsProtectedRegLocked().
//
//*****************************************************************************
void DrvFMC_EnableISP(void)
{
    SYSCLK->AHBCLK.ISP_EN = 1;
    FMC->ISPCON.ISPEN = 1;
}

//*****************************************************************************
//
//! \brief Disable ISP function.   
//!
//! \param None 
//!
//! \return None  
//!
//! \note Please make sure that Register Write-Protection Function has been 
//!  disabled before using this function to disable
//!  ISP function. User can check the status of Register Write-Protection
//!  Function with DrvSYS_IsProtectedRegLocked().
//
//*****************************************************************************
void DrvFMC_DisableISP(void)
{
    FMC->ISPCON.ISPEN = 0;
}

//*****************************************************************************
//
//! \brief Select next boot from APROM or LDROM.   
//!
//! \param boot E_FMC_APROM / E_FMC_LDROM  
//!
//! \return None  
//
//*****************************************************************************
void DrvFMC_BootSelect(E_FMC_BOOTSELECT boot)
{
    FMC->ISPCON.BS = (boot)? 1: 0;
}

//*****************************************************************************
//
//! \brief To return current boot select setting. 
//!
//! \param None 
//!
//! \return E_FMC_APROM      The current boot select setting is in APROM
//!         E_FMC_LDROM      The current boot select setting is in LDROM  
//
//*****************************************************************************
E_FMC_BOOTSELECT DrvFMC_GetBootSelect(void)
{
    return (E_FMC_BOOTSELECT)FMC->ISPCON.BS;
}

//*****************************************************************************
//
//! \brief Enable LDROM update function.  
//!
//! \param None 
//!
//! \return None
//!
//! \note Please make sure that Register
//!  Write-Protection Function has been disabled before using this function to
//!  enable LD update function. User can check the status of Register Write
//!  -Protection Function with DrvSYS_IsProtectedRegLocked().  
//
//*****************************************************************************
void DrvFMC_EnableLDUpdate(void)
{   
    FMC->ISPCON.LDUEN = 1;
}

//*****************************************************************************
//
//! \brief Disable LDROM update function.   
//!
//! \param None 
//!
//! \return None  
//!
//! \note Please make sure that Register
//!  Write-Protection Function has been disabled before using this function to
//!  disable LD update function. User can check the status of Register Write
//!  -Protection Function with DrvSYS_IsProtectedRegLocked().
//
//*****************************************************************************
void DrvFMC_DisableLDUpdate(void)
{   
    FMC->ISPCON.LDUEN = 0;
}

//*****************************************************************************
//
//! \brief Enable Config update function when MCU is running in APROM or LDROM. 
//!   
//! \param None 
//!
//! \return None 
//!
//! \note Please make sure that Register Write-Protection Function has been
//!  disabled before using this function to enable Config update function. User
//!  can check the status of Register Write-Protection Function with
//!  DrvSYS_IsProtectedRegLocked(). 
//
//*****************************************************************************
void DrvFMC_EnableConfigUpdate(void)
{   
    FMC->ISPCON.CFGUEN = 1;
}

//*****************************************************************************
//
//! \brief Disable Config update function when MCU is running in APROM or LDROM. 
//!   
//! \param None 
//!
//! \return None 
//!
//! \note Please make sure that Register Write-Protection Function has been
//!  disabled before using this function to disable Config update function. User
//!  can check the status of Register Write-Protection Function with
//!  DrvSYS_IsProtectedRegLocked(). 
//
//*****************************************************************************
void DrvFMC_DisableConfigUpdate(void)
{   
    FMC->ISPCON.CFGUEN = 0;
}

//*****************************************************************************
//
//! \brief Enable flash power saving function.   
//!
//! \param None 
//!
//! \return None  
//!
//! \note Please make sure that Register Write-Protection Function has been 
//!  disabled before using this function to enable flash power saving function. 
//!  User can check the status of Register Write-Protection Function with 
//!  DrvSYS_IsProtectedRegLocked().
//
//*****************************************************************************
void DrvFMC_EnablePowerSaving(void)
{   
    FMC->FATCON.FPSEN = 1;
}

//*****************************************************************************
//
//! \brief Disable flash power saving function.   
//!
//! \param None 
//!
//! \return None  
//! 
//! \note Please make sure that Register Write-Protection Function has been disabled 
//!  before using this function to disable flash power saving function. User can 
//!  check the status of Register Write-Protection Function with 
//!  DrvSYS_IsProtectedRegLocked().
//
//*****************************************************************************
void DrvFMC_DisablePowerSaving(void)
{   
    FMC->FATCON.FPSEN = 0;
}

//*****************************************************************************
//
//! \brief Enable flash access low frequency optimization mode. It can improve
//!  flash access performance when CPU runs at low frequency.   
//!
//! \param None 
//!
//! \return None 
//! 
//! \note Set this bit only when HCLK <= 25MHz. If HCLK > 25MHz, CPU will fetch 
//!  wrong code and cause fail result. Note2: Please make sure that Register Write
//!  -Protection Function has been disabled before using this function to enable
//!  flash access low frequency optimization mode. User can check the status of
//!  Register Write-Protection Function with DrvSYS_IsProtectedRegLocked(). 
//
//*****************************************************************************
void DrvFMC_EnableLowFreqOptMode(void)
{   
    FMC->FATCON.LFOM = 1;
}

//*****************************************************************************
//
//! \brief Disable flash access low frequency optimization mode.  
//!
//! \param None 
//!
//! \return None
//!
//! \note Please make sure that Register Write-Protection Function has been 
//!  disabled before using this function to disable flash access low frequency 
//!  optimization mode. User can check the status of Register Write-Protection 
//!  Function with DrvSYS_IsProtectedRegLocked().   
//
//*****************************************************************************
void DrvFMC_DisableLowFreqOptMode(void)
{   
    FMC->FATCON.LFOM = 0;
}


//*****************************************************************************
//
//! \brief Read Data Flash base address. The base address is fixed 0x1F000 for
//!  M051 series. 
//!
//! \param None 
//!
//! \return Data Flash base address  
//
//*****************************************************************************
uint32_t DrvFMC_ReadDataFlashBaseAddr(void)
{   
    return FMC->DFBADR;
}

//*****************************************************************************
//
//! \brief Erase Config and write data into Config0 The corresponding functions
//!  in Config0 are listed in FMC section of data sheet.  
//!
//! \param u32data0 Config0 data  
//!
//! \return 0 Success, 
//!         <0 Failed when illegal condition occurs  
//!
//! \Note: Please make sure that Register Write-Protection Function has been 
//!  disabled before using this function. User can check the status of Register 
//!  Write-Protection Function with DrvSYS_IsProtectedRegLocked().   
//
//*****************************************************************************
int32_t DrvFMC_WriteConfig(uint32_t u32data0)
{       
    if ( DrvFMC_Erase(CONFIG0) != E_SUCCESS )
        return E_DRVFMC_ERR_ISP_FAIL;
    
    return DrvFMC_Write(CONFIG0, u32data0);
}

//*****************************************************************************
//
//! \brief This function is used to get the DrvFMC version number. 
//!
//! \param None 
//!
//! \return The DrvFMC version number  
//
//*****************************************************************************
uint32_t DrvFMC_GetVersion(void)
{
    return DRVFMC_VERSION_NUM;
}

