//*****************************************************************************
//
//! \file system_M051Series.c
//! \brief CMSIS ARM Cortex-M0 Device Peripheral Access Layer Source File for
//!              M051Series devices. 
//! \version v1.02.002
//! \date 21/09/2011
//! \author Nuvoton
//! \copy
//!
//! Copyright (c) Nuvoton Technology Corp. All rights reserved.
//
//*****************************************************************************
#include <stdint.h>
#include "M051Series.h"

//*****************************************************************************
//
//! Clock Variable definitions
//
//*****************************************************************************

//
//! System Clock Frequency (Core Clock)
//
uint32_t SystemCoreClock  = __HSI; 

//
//! Cycles per micro second 
//
uint32_t CyclesPerUs      = (__HSI / 1000000);
uint32_t gau32ClkSrcTbl[] = {__XTAL, NULL,__HSI, __IRC10K, __IRC22M};

//*****************************************************************************
//
//! Clock functions
//! This function is used to update the variable SystemCoreClock 
//! and must be called whenever the core clock is changed.
//
//*****************************************************************************

//
//! Get Core Clock Frequency 
//
void SystemCoreClockUpdate (void)   
{
    uint32_t u32CoreFreq, u32ClkSrc, u32Shift;
 
    u32ClkSrc = SYSCLK->CLKSEL0.HCLK_S;
    if(u32ClkSrc > sizeof(gau32ClkSrcTbl)/sizeof(gau32ClkSrcTbl[0]))
        u32ClkSrc = sizeof(gau32ClkSrcTbl)/sizeof(gau32ClkSrcTbl[0]) - 1; 
    
    if(u32ClkSrc != 2)
    {
        //
        // Use the clock sources directly
        //
        u32CoreFreq = gau32ClkSrcTbl[u32ClkSrc];
    }
    else
    {
        //
        // Use PLL
        //

		//
		// OUT_DV :DEF: {1, 2, 2, 4}
		//
        u32Shift = SYSCLK->PLLCON.OUT_DV; 
        if(u32Shift > 1) u32Shift--;
        u32CoreFreq = ((SYSCLK->PLLCON.PLL_SRC)?__IRC22M:__XTAL) * (SYSCLK->PLLCON.FB_DV+2) / (SYSCLK->PLLCON.IN_DV+2) >> u32Shift;
    }
 
    SystemCoreClock = (u32CoreFreq/(SYSCLK->CLKDIV.HCLK_N + 1));
    CyclesPerUs = (SystemCoreClock + 500000) / 1000000;
}

//*****************************************************************************
//
//! Function SystemInit                                                        
//!                                                                            
//! \Param None                                                                  
//!                                                                            
//! \Returns None                                                                  
//!                                                                           
//!  The necessary initializaiton of systerm.                                                           
//                                                                          
//*****************************************************************************
void SystemInit (void)
{

}


