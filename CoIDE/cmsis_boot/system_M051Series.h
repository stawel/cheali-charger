//*****************************************************************************
//
//! \file system_M051Series.h
//! \brief CMSIS CMSIS ARM Cortex-M0 Device Peripheral Access Layer Header File 
//!              for M051Series devices. 
//! \version v1.02.002
//! \date 21/09/2011
//! \author Nuvoton
//! \copy
//!
//! Copyright (c) Nuvoton Technology Corp. All rights reserved.
//
//*****************************************************************************
#ifndef __SYSTEM_M051Series_H
#define __SYSTEM_M051Series_H

#ifdef __cplusplus
 extern "C" {
#endif 

//*****************************************************************************
//
//! Define SYSCLK
//
//*****************************************************************************
#define __XTAL      (12000000UL)
#define __IRC22M    (22118400UL)
#define __IRC10K    (10000UL)

//
//! Factory Default is internal 22MHz
//
#define __HSI       (__IRC22M) 

//
//! System Clock Frequency (Core Clock)
//
extern uint32_t SystemCoreClock; 

//
//! Cycles per micro second
//
extern uint32_t CyclesPerUs;      

//*****************************************************************************
//
//! \brief Setup the microcontroller system Initialise GPIO directions and values
//! 
//! \param none
//!
//! \return none
//
//*****************************************************************************
extern void SystemInit(void);

//*****************************************************************************
//
//! \brief Updates the SystemCoreClock with current core Clock 
//!        retrieved from cpu registers.
//! 
//! \param none
//!
//! \return none
//
//*****************************************************************************
extern void SystemCoreClockUpdate (void);

#ifdef __cplusplus
}
#endif

#endif

