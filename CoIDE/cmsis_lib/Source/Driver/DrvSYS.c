//*****************************************************************************
//
//! \file DrvSYS.c
//! \brief Contains all functions support for SYS 
//! \version v1.02.002
//! \date 21/09/2011
//! \author Nuvoton
//! \copy
//!
//! Copyright (c) Nuvoton Technology Corp. All rights reserved.
//
//*****************************************************************************
#include <stdio.h>
#include "DrvSYS.h"


#define FREQ_25MHZ         25000000
#define FREQ_50MHZ         50000000
#define FREQ_100MHZ       100000000
#define FREQ_200MHZ       200000000  

//*****************************************************************************
//
//! Global file scope (static) variables
//
//*****************************************************************************
static BOD_CALLBACK BOD_callbackFn;
static PWRWU_CALLBACK PWRWU_callbackFn;

//*****************************************************************************
//
//! \brief This function is used to get the DrvSYS version number             
//!                                                                            
//! \param None                                                               
//!                                                                           
//! \return The DrvSYS version number     
//
//*****************************************************************************
uint32_t DrvSYS_GetVersion(void)
{
    return DRVSYS_VERSION_NUM;
}

//*****************************************************************************
//
//! \brief Read Product ID             
//!                                                                            
//! \param None                                                               
//!                                                                           
//! \return Product ID      
//
//*****************************************************************************
uint32_t DrvSYS_ReadProductID(void)
{   
    return SYS->PDID;
}

//*****************************************************************************
//
//! \brief Unlock protected register             
//!                                                                            
//! \param None                                                               
//!                                                                           
//! \return - 0 Success
//!         - <0 Failed
//
//*****************************************************************************
int32_t DrvSYS_UnlockProtectedReg(void)
{
    uint32_t lockbit;
    
    SYS->REGWRPROT = 0x59;
    SYS->REGWRPROT = 0x16;
    SYS->REGWRPROT = 0x88;

    lockbit = SYS->REGWRPROT & 0x01;
    return (lockbit)? 0 : E_DRVSYS_ERR_UNLOCK_FAIL;
} 

//*****************************************************************************
//
//! \brief Re-Lock protected registers             
//!                                                                            
//! \param None                                                               
//!                                                                           
//! \return - 0 Success
//!         - <0 Failed
//
//***************************************************************************** 
int32_t DrvSYS_LockProtectedReg(void)
{
    uint32_t lockbit;
    
    SYS->REGWRPROT = 0x00;

    lockbit = SYS->REGWRPROT & 0x01;
    return (lockbit)? E_DRVSYS_ERR_LOCK_FAIL : 0;
}

//*****************************************************************************
//
//! \brief This function is used to check the protected registers are locked or
//!        not             
//!                                                                            
//! \param None                                                               
//!                                                                           
//! \return - 1 The protected Registers are unlocked 
//!         - 0 The protected Registers are locked 
//
//*****************************************************************************
int32_t DrvSYS_IsProtectedRegLocked(void)
{
    return SYS->REGWRPROT & 0x01;
}

//*****************************************************************************
//
//! \brief Get reset source from last operation             
//!                                                                            
//! \param None                                                               
//!                                                                           
//! \return Reset source       
//
//*****************************************************************************
uint32_t DrvSYS_GetResetSource(void)
{
    return *((__IO uint32_t *) &SYS->RSTSRC);
}

//*****************************************************************************
//
//! \brief Clear reset source by write 1              
//!                                                                            
//! \param u32Src The corresponding bit of reset source                                                                
//!                                                                           
//! \return 0       
//
//*****************************************************************************
uint32_t DrvSYS_ClearResetSource(uint32_t u32Src)
{
    *((__IO uint32_t *) &SYS->RSTSRC) = u32Src;
    return 0;
}

//*****************************************************************************
//
//! \brief Reset coresponding IP              
//!                                                                            
//! \param eIpRst The coresponding IP
//!
//! The \e eIpRst can be one of the following values:
//! \b E_SYS_GPIO_RST, \b E_SYS_TMR0_RST, \b E_SYS_TMR1_RST, \b E_SYS_TMR2_RST
//! \b E_SYS_TMR3_RST, \b E_SYS_I2C_RST, \b E_SYS_SPI0_RST, \b E_SYS_SPI1_RST
//! \b E_SYS_UART0_RST, \b E_SYS_UART1_RST, \b E_SYS_PWM03_RST, \b E_SYS_PWM47_RST
//! \b E_SYS_ADC_RST, \b E_SYS_EBI_RST
//!    
//! \note Please make sure that Register Write-Protection Function has been 
//!       disabled before using this function to reset EBI. User can check the 
//!       status of Register Write-Protection Function  with 
//!       DrvSYS_IsProtectedRegLocked().  
//! 
//! \return None       
//
//*****************************************************************************
void DrvSYS_ResetIP(E_SYS_IP_RST eIpRst)
{   
    uint32_t u32Reg;

    if (eIpRst == E_SYS_EBI_RST)
    {
        SYS->IPRSTC1.EBI_RST = 1;
        SYS->IPRSTC1.EBI_RST = 0;
    }
    else
    {
        u32Reg = *((__IO uint32_t *) &SYS->IPRSTC2);
        *((__IO uint32_t *) &SYS->IPRSTC2) = u32Reg | (1<<eIpRst);
        *((__IO uint32_t *) &SYS->IPRSTC2) = u32Reg & ~(1<<eIpRst);
    }
}

//*****************************************************************************
//
//! \brief Reset CPU              
//!                                                                            
//! \param None
//!    
//! \note Please make sure that Register Write-Protection Function has been 
//!       disabled before using this function to reset CPU.  
//!       disabled before using this function to reset EBI. User can check the 
//!       User can check the status of Register Write-Protection Function 
//!       ith DrvSYS_IsProtectedRegLocked().
//! 
//! \return None       
//
//*****************************************************************************
void DrvSYS_ResetCPU(void)
{
    SYS->IPRSTC1.CPU_RST = 1;
}

//*****************************************************************************
//
//! \brief Reset whole chip              
//!                                                                            
//! \param None
//!    
//! \note Please make sure that Register Write-Protection Function has been 
//!       disabled before using this function to reset whole chip . 
//!       User can check the status of Register Write-Protection Function  
//!       with DrvSYS_IsProtectedRegLocked().   
//! 
//! \return None       
//
//*****************************************************************************
void DrvSYS_ResetChip(void)
{
    SYS->IPRSTC1.CHIP_RST = 1;
}

//*****************************************************************************
//
//! \brief Select Brown-Out Detector threshold voltage              
//!                                                                            
//! \param u8Volt BOD voltage value, it can be selected form one of the
//!               following value: - 3:4.5V
//!                                - 2:3.8V
//!                                - 1:2.7V
//!                                - 0:2.2V
//!    
//! \note Please make sure that Register Write-Protection Function has been 
//!       disabled before using this function to select BOD threshold voltage.  
//!       User can check the status of Register Write-Protection Function
//!       with DrvSYS_IsProtectedRegLocked().   
//! 
//! \return None       
//
//*****************************************************************************
void DrvSYS_SelectBODVolt(uint8_t u8Volt)
{
    SYS->BODCR.BOD_VL = u8Volt;
}

//*****************************************************************************
//
//! \brief Enable Brown-Out Detector, and select BOD reset function or interrupt
//!        function. And, install call backfunction if interrupt function is 
//!        selected.              
//!                                                                            
//! \param i32Enable if \b 1 Enable, \b 0 Disable
//!
//! \param i32Flag if \b 1 Enable BOD reset function, \b 0 Enable BOD interrupt 
//!        function
//!    
//! \param bodcallbackFn BOD Call back function
//!
//! \note Please make sure that Register Write-Protection Function has been 
//!       disabled before using this function. 
//!       User can check the status of Register Write-Protection Function 
//!       with DrvSYS_IsProtectedRegLocked(). 
//! 
//! \return None       
//
//*****************************************************************************
void DrvSYS_SetBODFunction(int32_t i32Enable, int32_t i32Mode, BOD_CALLBACK bodcallbackFn)
{
    SYS->BODCR.BOD_EN = i32Enable;

    if (i32Enable)
    {
        SYS->BODCR.BOD_RSTEN = i32Mode;
    
        if (i32Mode)
        {       
            NVIC_DisableIRQ(BOD_IRQn);
        }
        else
        { 
            BOD_callbackFn = bodcallbackFn;
            NVIC_SetPriority(BOD_IRQn, (1<<__NVIC_PRIO_BITS) - 2);
            NVIC_EnableIRQ(BOD_IRQn);
        }
    }   
}

//*****************************************************************************
//
//! \brief Enable Brown-Out Detector low power mode            
//!                                                                            
//! \param None
//!
//! \note Please make sure that Register Write-Protection Function has been
//!       disabled before using this function to enable BOD low power mode.   
//!       User can check the status of Register Write-Protection Function 
//!       with DrvSYS_IsProtectedRegLocked().  
//! 
//! \return None       
//
//*****************************************************************************
void DrvSYS_EnableBODLowPowerMode(void)
{
    SYS->BODCR.BOD_LPM = 1;
}

//*****************************************************************************
//
//! \brief Disable Brown-Out Detector low power mode            
//!                                                                            
//! \param None
//!
//! \note Please make sure that Register Write-Protection Function has been 
//!       disabled before using this function to disable BOD low power mode.   
//!       User can check the status of Register Write-Protection Function 
//!       with DrvSYS_IsProtectedRegLocked().   
//! 
//! \return None       
//
//*****************************************************************************
void DrvSYS_DisableBODLowPowerMode(void)
{
    SYS->BODCR.BOD_LPM = 0;
}

//*****************************************************************************
//
//! \brief Get Brown-Out Detector state            
//!                                                                            
//! \param None
//!
//! \note None 
//! 
//! \return BOD output status - 1: the detected voltage is lower than BOD 
//!                                threshold voltage    
//!                           - 0: the detected voltage is higher than BOD
//!                                threshold voltage
//
//*****************************************************************************
uint32_t DrvSYS_GetBODState(void)
{
    return SYS->BODCR.BOD_OUT;
}

//*****************************************************************************
//
//! \brief Enable low voltage reset function            
//!                                                                            
//! \param None
//!
//! \note Please make sure that Register Write-Protection Function has been
//!       disabled before using this function to enable low voltage reset. 
//!       User can check the status of Register Write-Protection Function  
//!       with DrvSYS_IsProtectedRegLocked().    
//! 
//! \return None       
//
//*****************************************************************************
void DrvSYS_EnableLowVoltReset(void)
{
    SYS->BODCR.LVR_EN = 1;
}

//*****************************************************************************
//
//! \brief Disable low voltage reset function           
//!                                                                            
//! \param None
//!
//! \note Please make sure that Register Write-Protection Function has been
//!       disabled before using this function to disable low voltage reset. 
//!       User can check the status of Register Write-Protection Function  
//!       with DrvSYS_IsProtectedRegLocked().    
//! 
//! \return None       
//
//*****************************************************************************
void DrvSYS_DisableLowVoltReset(void)
{
    SYS->BODCR.LVR_EN = 0;
}

//*****************************************************************************
//
//! \brief Enable power on reset (POR) control          
//!                                                                            
//! \param None
//!
//! \note Please make sure that Register Write-Protection Function has been
//!       disabled before using this function to enable POR control. 
//!       User can check the status of Register Write-Protection Function  
//!       with DrvSYS_IsProtectedRegLocked().    
//! 
//! \return None       
//
//*****************************************************************************
void DrvSYS_EnablePOR(void)
{
    SYS->PORCR = 0;
}

//*****************************************************************************
//
//! \brief Enable power on reset (POR) control          
//!                                                                            
//! \param None
//!
//! \note Please make sure that Register Write-Protection Function has been
//!       disabled before using this function to disable POR control. 
//!       User can check the status of Register Write-Protection Function  
//!       with DrvSYS_IsProtectedRegLocked().    
//! 
//! \return None       
//
//*****************************************************************************
void DrvSYS_DisablePOR(void)
{
    SYS->PORCR = 0x5AA5;
}

//*****************************************************************************
//
//! \brief ISR to handle BOD interrupt event         
//!                                                                            
//! \param None
//!
//! \note None   
//! 
//! \return None       
//
//*****************************************************************************
void BOD_IRQHandler(void)
{
    SYS->BODCR.BOD_INTF = 1;
    if (BOD_callbackFn != NULL)
        BOD_callbackFn();

}

//*****************************************************************************
//
//! \brief Enable/Disable coresponding IP clock              
//!                                                                            
//! \param eIpClk The coresponding IP
//!
//! \param i32Enable When \b 1 Enable the corresponding IP, while \b 0 Disable
//!
//! The \e eIpClk can be one of the following values:
//! \b E_SYS_WDT_CLK, \b E_SYS_TMR0_CLK, \b E_SYS_TMR1_CLK, \b E_SYS_TMR2_CLK
//! \b E_SYS_TMR3_CLK, \b E_SYS_FDIV_CLK, \b E_SYS_I2C_CLK, \b E_SYS_SPI0_CLK
//! \b E_SYS_SPI1_CLK, \b E_SYS_UART0_CLK, \b E_SYS_UART1_CLK, \b E_SYS_PWM01_CLK
//! \b E_SYS_PWM23_CLK, \b E_SYS_PWM45_CLK, \b E_SYS_PWM67_CLK, \b E_SYS_ADC_CLK
//! \b E_SYS_ISP_CLK, \b E_SYS_EBI_CLK
//!    
//! \note Please make sure that Register Write-Protection Function has been 
//!       disabled before using this function to renable/disable WDT clock. 
//!       User can check the status of Register Write-Protection Function 
//!       DrvSYS_IsProtectedRegLocked().  
//! 
//! \return None       
//
//*****************************************************************************
void DrvSYS_SetIPClock(E_SYS_IP_CLK eIpClk, int32_t i32Enable)
{   
    uint32_t u32Reg;

    if (eIpClk == E_SYS_EBI_CLK)
    {
        SYSCLK->AHBCLK.EBI_EN = i32Enable;
    }
    else if (eIpClk == E_SYS_ISP_CLK)
    {
        SYSCLK->AHBCLK.ISP_EN = i32Enable;
    }
    else
    {
        u32Reg = *((__IO uint32_t *) &SYSCLK->APBCLK);
        
        if (i32Enable)
        {
            *((__IO uint32_t *) &SYSCLK->APBCLK) = u32Reg | (1<<eIpClk);
        }
        else
        {
            *((__IO uint32_t *) &SYSCLK->APBCLK) = u32Reg & ~(1<<eIpClk);
        }
    }
}

//*****************************************************************************
//
//! \brief This function is used to select HCLK clock source              
//!                                                                            
//! \param u8ClkSrcSel The clock source can be slected form one of the following
//!                    value:
//!                    - 0:External 12M clock
//!                    - 2:PLL clock
//!                    - 3:Internal 10K clock 
//!                    - 7:Internal 22M clock
//!    
//! \note Please make sure that Register Write-Protection Function has been 
//!       disabled before using this function to select HCLK clock source. 
//!       User can check the status of Register Write-Protection Function 
//!       DrvSYS_IsProtectedRegLocked().  
//! 
//! \return - 0:Success 
//!         - E_DRVSYS_ERR_ARGUMENT:Incorrect argument
//!         - E_DRVSYS_ERR_CLKSWITCH_FAIL:Clock switch failed 
//
//*****************************************************************************
int32_t DrvSYS_SelectHCLKSource(uint8_t u8ClkSrcSel)
{
    if ((u8ClkSrcSel > 7) || ((u8ClkSrcSel > 3) && (u8ClkSrcSel < 7)) || (u8ClkSrcSel == 1) )
    {
        return E_DRVSYS_ERR_ARGUMENT;
    }
    else
    {
        SYSCLK->CLKSEL0.HCLK_S = u8ClkSrcSel;

        if (SYSCLK->CLKSTATUS.CLK_SW_FAIL)
        {
            /* Clear Clock Switch Fail Flag */
            SYSCLK->CLKSTATUS.CLK_SW_FAIL = 1;
            return E_DRVSYS_ERR_CLKSWITCH_FAIL;
        }
    }

    SystemCoreClockUpdate();

    return 0;
}

//*****************************************************************************
//
//! \brief This function is used to select HCLK clock source              
//!                                                                            
//! \param u8ClkSrcSel The clock source can be slected form one of the following
//!                    value:
//!                    - 0:External 12M clock
//!                    - 2:External 12M clock / 2
//!                    - 3:HCLK / 2 
//!                    - 4~7:Internal 22M clock / 2  
//!    
//! \note Please make sure that Register Write-Protection Function has been 
//!       disabled before using this function to select system tick clock source.  
//!       User can check the status of Register Write-Protection Function 
//!       DrvSYS_IsProtectedRegLocked().  
//! 
//! \return - 0:Success 
//!         - <0:Incorrect value
//
//*****************************************************************************
int32_t DrvSYS_SelectSysTickSource(uint8_t u8ClkSrcSel)
{
    if ((u8ClkSrcSel > 7) || (u8ClkSrcSel == 1))
    {
        return E_DRVSYS_ERR_ARGUMENT;
    }
    else
    {
        SYSCLK->CLKSEL0.STCLK_S = u8ClkSrcSel;
    }
    return 0;
}

//*****************************************************************************
//
//! \brief This function is used to select IP clock source             
//!                                                                            
//! \param eIpClkSrc The coresponding IP
//!
//! \param u8ClkSrcSel It can be one of the following value:
//!                    \b 0x00, \b 0x01, \b 0x02, \b 0x03, \b 0x07
//!
//! The \e eIpClkSrc can be one of the following values:
//! \b E_SYS_WDT_CLKSRC, \b E_SYS_ADC_CLKSRC, \b E_SYS_TMR0_CLKSRC, 
//! \b E_SYS_TMR1_CLKSRC, \b E_SYS_TMR2_CLKSRC, \b E_SYS_TMR3_CLKSRC,
//! \b E_SYS_UART_CLKSRC, \b E_SYS_PWM01_CLKSRC, \b E_SYS_PWM23_CLKSRC,
//! \b E_SYS_PWM45_CLKSRC, \b E_SYS_PWM67_CLKSRC, \b E_SYS_FRQDIV_CLKSRC,
//!
//!  --------------------------------------------------------------------------------------------
//!  |  u8ClkSrcSel  |     0x00     |     0x01     |     0x02     |     0x03     |     0x07     |
//!  --------------------------------------------------------------------------------------------
//!  |      WDT      |   Reserved   |   Reserved   |   HCLK/2048  | Internal 10K |      X       |
//!  --------------------------------------------------------------------------------------------
//!  |      ADC      | External 12M |     PLL      |   Reserved   | Internal 22M |      X       |
//!  --------------------------------------------------------------------------------------------
//!  |    TMR 0~3    | External 12M |   Reserved   |     HCLK     |   Reserved   | Internal 22M |
//!  --------------------------------------------------------------------------------------------
//!  |      UART     | External 12M |     PLL      |   Reserved   | Internal 22M |      X       |
//!  --------------------------------------------------------------------------------------------
//!  |      PWM      | External 12M |   Reserved   |     HCLK     | Internal 22M |      X       |
//!  --------------------------------------------------------------------------------------------
//!  |   Frequency   | External 12M |   Reserved   |     HCLK     | Internal 22M |      X       |
//!  | Divider Output|              |              |              |              |              |
//!  -------------------------------------------------------------------------------------------- 
//!    
//! \note Please make sure that Register Write-Protection Function has been 
//!       disabled before using this function to select WDT clock source.
//!       User can check the status of Register Write-Protection Function 
//!       DrvSYS_IsProtectedRegLocked().  
//! 
//! \return - 0:Success 
//!         - 1:Incorrect value
//
//*****************************************************************************
int32_t DrvSYS_SelectIPClockSource(E_SYS_IP_CLKSRC eIpClkSrc, uint8_t u8ClkSrcSel)
{
    
    switch(eIpClkSrc)
    {
        case E_SYS_FRQDIV_CLKSRC:  
            SYSCLK->CLKSEL2.FRQDIV_S = u8ClkSrcSel;
            break;
        case E_SYS_PWM67_CLKSRC:  
            SYSCLK->CLKSEL2.PWM67_S = u8ClkSrcSel;
            break;
        case E_SYS_PWM45_CLKSRC:
            SYSCLK->CLKSEL2.PWM45_S = u8ClkSrcSel;
            break;
        case E_SYS_PWM23_CLKSRC:  
            SYSCLK->CLKSEL1.PWM23_S = u8ClkSrcSel;
            break;
        case E_SYS_PWM01_CLKSRC:
            SYSCLK->CLKSEL1.PWM01_S = u8ClkSrcSel;
            break;
        case E_SYS_UART_CLKSRC:
            SYSCLK->CLKSEL1.UART_S = u8ClkSrcSel;
            break;
        case E_SYS_TMR3_CLKSRC:
            SYSCLK->CLKSEL1.TMR3_S = u8ClkSrcSel;
            break;              
        case E_SYS_TMR2_CLKSRC:
            SYSCLK->CLKSEL1.TMR2_S = u8ClkSrcSel;
            break;              
        case E_SYS_TMR1_CLKSRC:
            SYSCLK->CLKSEL1.TMR1_S = u8ClkSrcSel;
            break;              
        case E_SYS_TMR0_CLKSRC:
            SYSCLK->CLKSEL1.TMR0_S = u8ClkSrcSel;
            break;              
        case E_SYS_ADC_CLKSRC:
            SYSCLK->CLKSEL1.ADC_S = u8ClkSrcSel;
            break;              
        case E_SYS_WDT_CLKSRC:
            SYSCLK->CLKSEL1.WDT_S = u8ClkSrcSel;
            break;              
        default:
            return E_DRVSYS_ERR_IPSRC;
    }
    
    return E_SUCCESS;
}

//*****************************************************************************
//
//! \brief This function is used to set IP divider number from the corresponding
//!        clock source The actual clock divide number is (i32value + 1).             
//!                                                                            
//! \param eIpDiv The IP can be slected form one of the following value:
//!               \b E_SYS_ADC_DIV, \b E_SYS_UART_DIV, \b E_SYS_HCLK_DIV
//!    
//! \note None
//! 
//! \return - 0:Success 
//!         - <0:Incorrect value
//
//*****************************************************************************
int32_t DrvSYS_SetClockDivider(E_SYS_IP_DIV eIpDiv , int32_t i32value)
{
    if (eIpDiv == E_SYS_ADC_DIV)
    {
        SYSCLK->CLKDIV.ADC_N = i32value;
    }
    else if(eIpDiv == E_SYS_UART_DIV)
    {
        SYSCLK->CLKDIV.UART_N = i32value;
    }
    else if(eIpDiv == E_SYS_HCLK_DIV)
    {
        SYSCLK->CLKDIV.HCLK_N = i32value;
    }
    else
        return E_DRVSYS_ERR_IPDIV;

    return E_SUCCESS;   
}

//*****************************************************************************
//
//! \brief This function is used to enable/disable internal oscillator or 
//!        external crystal           
//!                                                                            
//! \param eClkSrc The clock source can be slected form one of the following value:
//!               \b E_SYS_XTL12M, \b E_SYS_OSC22M, \b E_SYS_OSC10K
//!
//! \param i32Enable - 1: Enable
//!                  - 0: Disable
//!    
//! \note Please make sure that Register Write-Protection Function has been 
//!       disabled before using this function to enable/disable internal oscillator
//!       or external crystal.
//!       User can check the status of Register Write-Protection Function 
//!       DrvSYS_IsProtectedRegLocked().  
//! 
//! \return - 0:Success 
//!         - <0:Incorrect value
//
//*****************************************************************************
int32_t DrvSYS_SetOscCtrl(E_SYS_CHIP_CLKSRC eClkSrc, int32_t i32Enable)
{   
    switch(eClkSrc)
    {
        case E_SYS_XTL12M:  
            SYSCLK->PWRCON.XTL12M_EN = i32Enable;
            break;
        case E_SYS_OSC22M:
            SYSCLK->PWRCON.OSC22M_EN = i32Enable;
            break;
        case E_SYS_OSC10K:
            SYSCLK->PWRCON.OSC10K_EN = i32Enable;
            break;              
        default:
            return E_DRVSYS_ERR_ARGUMENT;
    }
    
    return E_SUCCESS;
}

//*****************************************************************************
//
//! \brief ISR to handle Power Down Wake up interrupt event          
//!                                                                            
//! \param None
//!    
//! \note None
//! 
//! \return None
//
//*****************************************************************************
void PWRWU_IRQHandler(void)
{
    SYSCLK->PWRCON.PD_WU_STS = 1;
    if (PWRWU_callbackFn != NULL)
        PWRWU_callbackFn(); 
}

//*****************************************************************************
//
//! \brief Enable or disable Power dowm Wake up interrupt function, and install
//!        call back function. And, enable or disable clock cycles delay to wait
//!        oscillator clock stable.        
//!                                                                            
//! \param i32Enable - 1:Enable Power dowm Wake up interrup 
//!                  - 0:Disable Power dowm Wake up interrup
//!
//! \param pdwucallbackFn Power Down Wake Up Call back function 
//!
//! \param i32enWUDelay - 1:Enable clock cycles delay
//!                     - 0:Disable clock cycles delay
//!   
//! The delayed clock cycle is 4096 clock cycles when chip work at external 
//! 4~24 MHz crystal,or 256 clock cycles when chip work at internal 22.1184 MHz
//! oscillator.
//!
//! \note Please make sure that Register Write-Protection Function has been 
//!       disabled before using this function.
//!       User can check the status of Register Write-Protection Function 
//!       DrvSYS_IsProtectedRegLocked().  
//! 
//! \return None 
//
//*****************************************************************************
void DrvSYS_SetPowerDownWakeUpInt(int32_t i32Enable,\
                             PWRWU_CALLBACK pdwucallbackFn, int32_t i32enWUDelay)
{
    SYSCLK->PWRCON.PD_WU_INT_EN = i32Enable;
    
    if (i32Enable)
    {       
        SYSCLK->PWRCON.PD_WU_DLY = i32enWUDelay;
        PWRWU_callbackFn = pdwucallbackFn;
        NVIC_SetPriority(PWRWU_IRQn, (1<<__NVIC_PRIO_BITS) - 2);
        NVIC_EnableIRQ(PWRWU_IRQn);
    }
    else
    { 
        SYSCLK->PWRCON.PD_WU_DLY = 0;
        PWRWU_callbackFn = NULL;
        NVIC_DisableIRQ(PWRWU_IRQn);
    }   
}

//*****************************************************************************
//
//! \brief Enter power down function         
//!                                                                            
//! \param ePDType The power down type can be selected foem one of the following
//!                value:
//!                \b E_SYS_IMMEDIATE, \b E_SYS_WAIT_FOR_CPU
//!    
//! \note Please make sure that Register Write-Protection Function has been 
//!       disabled before using this function to enter power down. 
//!       User can check the status of Register Write-Protection Function 
//!       DrvSYS_IsProtectedRegLocked().  
//! 
//! \return None
//
//*****************************************************************************
void DrvSYS_EnterPowerDown(E_SYS_PD_TYPE ePDType)
{   
    SYSCLK->PWRCON.PWR_DOWN_EN = 0;
    SYSCLK->PWRCON.PD_WAIT_CPU = ePDType;
    SYSCLK->PWRCON.PWR_DOWN_EN = 1;
}

//*****************************************************************************
//
//! \brief This function is used to select PLL clock source          
//!                                                                            
//! \param ePllSrc The PLL clock source can be selected form the following value:
//!                - E_SYS_EXTERNAL_12M:External Crystal clock (4 ~ 24 MHz)
//!                - E_SYS_INTERNAL_22M: Internal 22M clock 
//!    
//! \note None
//! 
//! \return None
//
//*****************************************************************************
void DrvSYS_SelectPLLSource(E_SYS_PLL_CLKSRC ePllSrc)
{
    SYSCLK->PLLCON.PLL_SRC = ePllSrc;
}

//*****************************************************************************
//
//! \brief Set PLL operates in power down mode or normal mode          
//!                                                                            
//! \param i32Flag - 1:Power down mode 
//!                - 0:Normal mode
//!    
//! \note None
//! 
//! \return None
//
//*****************************************************************************
void DrvSYS_SetPLLMode(int32_t i32Flag)
{
    SYSCLK->PLLCON.OE = i32Flag;
    SYSCLK->PLLCON.PD = i32Flag;
}

//*****************************************************************************
//
//! \brief Get external clock frequency. The clock UNIT is in Hz.          
//!                                                                            
//! \param None
//!    
//! \note None
//! 
//! \return The external clock frequency. The default clock is 12MHz. 
//
//*****************************************************************************
uint32_t DrvSYS_GetExtClockFreq(void)
{   
    return __XTAL;
}

//*****************************************************************************
//
//! \brief This function is used to calculate the nearest PLL frequency to fit 
//!        the target PLL frequency which is defined by u32PllClk and return 
//!        the relative PLL control register setting.          
//!                                                                            
//! \param ePllSrc The PLL clock source can be selected form the following value:
//!                - E_SYS_EXTERNAL_12M:External Crystal clock (4 ~ 24 MHz)
//!                - E_SYS_INTERNAL_22M: Internal 22M clock 
//! \param u32PllClk The target PLL clock frequency (Hz). The value is 25~200 (MHz)
//!    
//! \note The PLL register setting for the target PLL clock frequency. If 
//!       u32PllClk is out of range, the function will return default value 0xC22E
//! 
//! \return None
//
//*****************************************************************************
uint32_t DrvSYS_GetPLLContent(E_SYS_PLL_CLKSRC ePllSrc, uint32_t u32PllClk)
{
    uint32_t u32NR, u32NF, u32NO;
    
    uint32_t u32PllSrcClk = __XTAL, u32Register=0;

    if ( (u32PllClk <= FREQ_200MHZ) && (u32PllClk > FREQ_100MHZ) )
    {
        u32NO = 0;
    }
    else if ( (u32PllClk <= FREQ_100MHZ) && (u32PllClk > FREQ_50MHZ) )
    {
        u32NO = 1;
        u32PllClk = u32PllClk << 1;
    }
    else if ( (u32PllClk <= FREQ_50MHZ) && (u32PllClk >= FREQ_25MHZ) )
    {
        u32NO = 3;
        u32PllClk = u32PllClk << 2;
    }
    else
    {
        return 0xC22E;  
    } 
        
    if (ePllSrc == E_SYS_EXTERNAL_12M)
        u32PllSrcClk = __XTAL;
    else if (ePllSrc == E_SYS_INTERNAL_22M)
        u32PllSrcClk = __IRC22M; 

    u32NF = u32PllClk / 1000000;
    u32NR = u32PllSrcClk / 1000000; 
    
    while(1)
    {
        if ( (u32NR & 0x01) || (u32NF & 0x01) || (u32NR == 2) || (u32NF == 2) )
        {
            break;
        }
        else
        {
            u32NR >>= 1;
            u32NF >>= 1;
        }   
    }    
        
    u32Register = (u32NO<<14) | ((u32NR - 2)<<9) | (u32NF - 2);

    /* Return the nearest value */
    return u32Register;
}

//*****************************************************************************
//
//! \brief This function is used to set PLL settings. 
//!
//! User can use DrvSYS_GetPLLContent() to get proper PLL setting.   
//! And, User can use DrvSYS_GetPLLClockFreq() to get actual PLL clock.
//!                                                                            
//! \param u32PllContent The PLL register setting for the target PLL clock frequency
//!    
//! \note None
//! 
//! \return None 
//
//*****************************************************************************
void DrvSYS_SetPLLContent(uint32_t u32PllContent)
{
    SYSCLK->PLLCON.FB_DV  = (u32PllContent & 0x01FF);
    SYSCLK->PLLCON.IN_DV  = (u32PllContent >> 9) & 0x1F;
    SYSCLK->PLLCON.OUT_DV = (u32PllContent >> 14) & 0x03;
}

//*****************************************************************************
//
//! \brief Get PLL clock frquency. The clock UNIT is in Hz.          
//!                                                                            
//! \param None
//!    
//! \note None
//! 
//! \return The PLL clock.  
//
//*****************************************************************************
uint32_t DrvSYS_GetPLLClockFreq(void)
{
    uint32_t u32Freq =0, u32PLLSrc;
    uint32_t au32Map[4] = {1, 2, 2, 4};

    if (SYSCLK->PLLCON.PLL_SRC == 1)
    {
        u32PLLSrc = __IRC22M;   
    }
    else
    {
        u32PLLSrc = __XTAL;
    }

    if (SYSCLK->PLLCON.PD ==1)  
        u32Freq = 0;    
    else        
        u32Freq = u32PLLSrc * (SYSCLK->PLLCON.FB_DV+2) / (SYSCLK->PLLCON.IN_DV+2) / au32Map[SYSCLK->PLLCON.OUT_DV];

    return u32Freq;
}

//*****************************************************************************
//
//! \brief Get HCLK clock and the UNIT is in Hz.        
//!                                                                            
//! \param None
//!    
//! \note None
//! 
//! \return HCLK clock frequency in Hz.   
//
//*****************************************************************************
uint32_t DrvSYS_GetHCLKFreq(void)
{   
    uint32_t u32Freqout = 0, u32AHBDivider;

    //
    // external 12MHz crystal clock
    //
    if (SYSCLK->CLKSEL0.HCLK_S == 0)     
    {
        u32Freqout = __XTAL;    
    }
	
	//
	//  PLL clock
	//
    else if(SYSCLK->CLKSEL0.HCLK_S == 2) 
    {
        u32Freqout = DrvSYS_GetPLLClockFreq();
    }

	//
	// internal 10KHz oscillator clock
	//
    else if(SYSCLK->CLKSEL0.HCLK_S == 3)  
    {
        u32Freqout = __IRC10K;
    }

	//
	// internal 22MHz oscillator clock
	//
    else                              
    {
        u32Freqout = __IRC22M;
    
    }
    u32AHBDivider = SYSCLK->CLKDIV.HCLK_N + 1 ;
    
    return (u32Freqout/u32AHBDivider);  
}

//*****************************************************************************
//
//! \brief This could configure the PLL according to the PLL source clock and 
//!        target HCLK clock.        
//!                                                                            
//! \param u32Hclk The target HCLK clock frequency in Hz. The range of u32Hclk
//!                is 25MHz~50MHz 
//!    
//! \note Please make sure that Register Write-Protection Function has been 
//!       disabled before using this function to enable/disable internal oscillator
//!       or external crystal.
//!       User can check the status of Register Write-Protection Function 
//!       DrvSYS_IsProtectedRegLocked().  
//! 
//! \note Due to hardware limitation, the actual HCLK clock may be different to
//!       target HCLK clock.The DrvSYS_GetPLLClockFreq() could be used to get
//!       actual PLL clock.The DrvSYS_GetHCLKFreq() could be used to get actual
//!       HCLK clock. The DrvSYS_SetClockDivider() could be used to get lower 
//!       HCLK clock.  
//!
//! \return - E_SUCCESS:Success 
//!         - E_DRVSYS_ERR_OUT_OF_RANGE:The clock setting is out of range 
//!         - E_DRVSYS_ERR_REG_PROTECTED:The Register Write Protection function
//!                                      is enabled
//
//*****************************************************************************
int32_t DrvSYS_Open(uint32_t u32Hclk)
{
    uint32_t u32PllCr;  
    E_SYS_PLL_CLKSRC ePllSrc = E_SYS_EXTERNAL_12M;

    if ( (u32Hclk > FREQ_50MHZ) || (u32Hclk < FREQ_25MHZ) )
    {
        return E_DRVSYS_ERR_OUT_OF_RANGE;
    }

	//
	// Check the status of Register Write-Protection Function
	//
    if (DrvSYS_IsProtectedRegLocked() == 0)
    {
        return E_DRVSYS_ERR_REG_PROTECTED;
    }        

    if (SYSCLK->PLLCON.PLL_SRC == 0)
    {
        SYSCLK->PWRCON.XTL12M_EN = 1;
        
		//
		// Wait for 12M stable
		//
        while (SYSCLK->CLKSTATUS.XTL12M_STB == 0);
        
        ePllSrc = E_SYS_EXTERNAL_12M;
    }
    else
    {
        SYSCLK->PWRCON.OSC22M_EN = 1;
        
		//
		// Wait for 22M stable
		//
        while (SYSCLK->CLKSTATUS.OSC22M_STB == 0);

        ePllSrc = E_SYS_INTERNAL_22M;
    }

    u32PllCr = DrvSYS_GetPLLContent(ePllSrc, u32Hclk);

    DrvSYS_SetPLLContent(u32PllCr);
    SYSCLK->PLLCON.OE     = 0;
    SYSCLK->PLLCON.PD     = 0;
    
	//
	// Wait for PLL stable 
	//
    while (SYSCLK->CLKSTATUS.PLL_STB == 0);

	//
	// Change HCLK clock source to be PLL.
	//
    DrvSYS_SelectHCLKSource(2);
    
    return E_SUCCESS;
}

//*****************************************************************************
//
//! \brief Enable frequency clock output and set its divider value. The output 
//!        frequency = Fin/2^(N+1), Fin is the input clock frequency, N is 4-bit
//!        value       
//!                                                                            
//! \param i32Flag - 1:Enable
//!                - 0:Disable
//!   
//! \param u8Divider The divider of output frequency (0~15)   
//! 
//! \return - 0:Success 
//!         - <0:Incorrect value
//
//*****************************************************************************
int32_t DrvSYS_SetFreqDividerOutput(int32_t i32Flag, uint8_t u8Divider)
{
    if (u8Divider > 15)
        return E_DRVSYS_ERR_ARGUMENT;

    if (i32Flag)
    {
        SYSCLK->FRQDIV.FSEL = u8Divider;
    }
    SYSCLK->FRQDIV.FDIV_EN = i32Flag;       
    DrvSYS_SetIPClock(E_SYS_FDIV_CLK, i32Flag);

    return E_SUCCESS;
}

//*****************************************************************************
//
//! \brief Use the SysTick to generate the delay time and the UNIT is in us 
//!        The SysTick clock source is from HCLK.      
//!                                                                            
//! \param us Delay time. The Max value is 335000
//! 
//! \return  None 
//
//*****************************************************************************
void DrvSYS_Delay(uint32_t us)
{
    SysTick->LOAD = us * CyclesPerUs;
    SysTick->VAL  = (0x00);
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;

	//
	// Waiting for down-count to zero
	//
    while((SysTick->CTRL & (1 << 16)) == 0);
}

//*****************************************************************************
//
//! \brief This function is used to monitor if the chip clock source is stable
//!        or not      
//!                                                                            
//! \param eClkSrc The chip clock source can be selected form the following value:
//!         \b E_SYS_XTL12M, \b E_SYS_OSC22M, \b E_SYS_OSC10K, \b E_SYS_PLL
//! 
//! \return - 0:Clock source is not stable or not enabled 
//!         - 1:Clock source is stable 
//!         - <0:Incorrect value 
//
//*****************************************************************************
int32_t DrvSYS_GetChipClockSourceStatus(E_SYS_CHIP_CLKSRC eClkSrc)
{
    int32_t status;

    switch(eClkSrc)
    {
        case E_SYS_XTL12M:  
            status = SYSCLK->CLKSTATUS.XTL12M_STB;
            break;
        case E_SYS_OSC22M:
            status = SYSCLK->CLKSTATUS.OSC22M_STB;
            break;
        case E_SYS_OSC10K:
            status = SYSCLK->CLKSTATUS.OSC10K_STB;
            break;              
        case E_SYS_PLL:
            status = SYSCLK->CLKSTATUS.PLL_STB;
            break;
        default:
            return E_DRVSYS_ERR_ARGUMENT;
    }

    return status;      
}

//*****************************************************************************
//
//! \brief This function is used to get if switch target clock is successful or
//!        not when software switches system clock source.     
//!                                                                            
//! \param None
//! 
//! \return - 1:Clock switch fail
//!         - 0:Clock switch success 
//
//*****************************************************************************
uint32_t DrvSYS_GetClockSwitchStatus(void)
{   
    return SYSCLK->CLKSTATUS.CLK_SW_FAIL;
}

//*****************************************************************************
//
//! \brief This function is used to clear the Clock Switch Fail Flag 
//!                                                                            
//! \param None
//! 
//! \return None
//
//*****************************************************************************
void DrvSYS_ClearClockSwitchStatus(void)
{
     SYSCLK->CLKSTATUS.CLK_SW_FAIL = 1;
}

