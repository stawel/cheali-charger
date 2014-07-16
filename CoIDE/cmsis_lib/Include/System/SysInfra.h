//*****************************************************************************
//
//! \file SysInfra.h
//! \brief 
//! \version v1.02.002
//! \date 21/09/2011
//! \author Nuvoton
//! \copy
//!
//! Copyright (c) Nuvoton Technology Corp. All rights reserved.
//
//*****************************************************************************


#ifndef __SYSINFRA_H__
#define __SYSINFRA_H__

//*****************************************************************************
// Includes of system headers                                                  
//*****************************************************************************
#include "ModuleID.h"
#include "stdint.h"
#include "core_cm0.h"

#ifdef __cplusplus
extern "C" {
#endif

//*****************************************************************************
// Define Version number                                                
//*****************************************************************************

#define SYSINFRA_MAJOR_NUM		1
#define SYSINFRA_MINOR_NUM		00
#define SYSINFRA_BUILD_NUM		1

//*****************************************************************************
// Macro, type and constant definitions                                                
//*****************************************************************************

//
// Define an error code composed of error bit, module ID, and error ID.
//
#define _SYSINFRA_ERRCODE(IS_ERROR, MODULE_ID_VALUE, ERROR_ID)      (((IS_ERROR) ? 0xFFFF0000 : 0x00000000) | ((((MODULE_ID_VALUE) & 0xFF) | ((IS_ERROR) ? 0x100 : 0x00)) << 7) | ((ERROR_ID) & 0x7F))
#define _SYSINFRA_ERRCODE_DEF(MODULE_ID, ERROR_NAME, IS_ERROR, MODULE_ID_VALUE, ERROR_ID) enum {E_##MODULE_ID##_##ERROR_NAME = ((IS_ERROR) ? 0xFFFF0000 : 0x00000000) | ((((MODULE_ID_VALUE) & 0xFF) | ((IS_ERROR) ? 0x100 : 0x00)) << 7) | ((ERROR_ID) & 0x7F)};

//
// Define a module version composed of major number, minor number, and build 
// number.
//
#define _SYSINFRA_VERSION(MAJOR_NUM, MINOR_NUM, BUILD_NUM)          (((MAJOR_NUM) << 16) | ((MINOR_NUM) << 8) | (BUILD_NUM))
#define _SYSINFRA_VERSION_DEF(MODULE_ID, MAJOR_NUM, MINOR_NUM, BUILD_NUM)	enum {MODULE_ID##_VERSION_NUM = ((MAJOR_NUM) << 16) | ((MINOR_NUM) << 8) | (BUILD_NUM)};

//
// Test if this error code means an error by seeing its error bit (BIT31). 
//
#define _SYSINFRA_ERRCODE_IS_ERROR(ERROR_CODE)	((ERROR_CODE) < 0)

//
// Extract module ID part of this error code.
//
#define _SYSINFRA_ERRCODE_EXTRACT_MODULE_ID(ERROR_CODE)	(((ERROR_CODE) >> 7) & 0xFF)

//
// Extract error ID part of this error code.
//
#define _SYSINFRA_ERRCODE_EXTRACT_ERROR_ID(ERROR_CODE)	((ERROR_CODE) & 0x7F)

//
// Define module version number.
//
#define SYSINFRA_VERSION_NUM    _SYSINFRA_VERSION(SYSINFRA_MAJOR_NUM, SYSINFRA_MINOR_NUM, SYSINFRA_BUILD_NUM)

#define E_SUCCESS		0
#define S_OK			E_SUCCESS

//*****************************************************************************
// Define Error Code                                                  
//*****************************************************************************

//
// #define Error Code
//
// E_SYSINFRA_NOT_DEFINE					Un-defined error code	
// E_SYSINFRA_NULL_POINTER				A NULL pointer is passed as an argument
// E_SYSINFRA_BUFFER_OVERRUN				Buffer size is not enough
//

#define E_SYSINFRA_NOT_DEFINE       _SYSINFRA_ERRCODE(TRUE, MODULE_ID_SYSINFRA, 0)
#define E_SYSINFRA_NULL_POINTER     _SYSINFRA_ERRCODE(TRUE, MODULE_ID_SYSINFRA, 1)
#define E_SYSINFRA_BUFFER_OVERRUN   _SYSINFRA_ERRCODE(TRUE, MODULE_ID_SYSINFRA, 2)

#ifdef __cplusplus
}
#endif

#endif // __SYSINFRA_H__



