/**************************************************************************//**
 * @file     acmp.c
 * @version  V3.00
 * $Revision: 3 $
 * $Date: 14/10/17 5:08p $
 * @brief    NUC029 series Analog Comparator(ACMP) driver source file
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2014 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/

#include "NUC029xAN.h"

#ifdef __cplusplus
extern "C"
{
#endif

/** @addtogroup NUC029_Device_Driver NUC029 Device Driver
  @{
*/

/** @addtogroup NUC029_ACMP_Driver ACMP Driver
  @{
*/


/** @addtogroup NUC029_ACMP_EXPORTED_FUNCTIONS ACMP Exported Functions
  @{
*/


/**
  * @brief  Configure the specified ACMP module
  *
  * @param[in]  Acmp The pointer of the specified ACMP module
  * @param[in]  u32ChNum Comparator number.
  * @param[in]  u32NegSrc Comparator negative input selection.  Including:
  *                  - \ref ACMP_CR_VNEG_PIN
  *                  - \ref ACMP_CR_VNEG_BANDGAP
  * @param[in]  u32HysteresisEn The hysteresis function option. Including:
  *                  - \ref ACMP_CR_HYSTERESIS_ENABLE
  *                  - \ref ACMP_CR_HYSTERESIS_DISABLE
  * @return None
  *
  * @details    Configure hysteresis function, select the source of negative input and enable analog comparator.
  */
void ACMP_Open(ACMP_T *Acmp, uint32_t u32ChNum, uint32_t u32NegSrc, uint32_t u32HysteresisEn)
{
    Acmp->CR[u32ChNum % 2] = (Acmp->CR[u32ChNum % 2] & (~(ACMP_CR_NEGSEL_Msk | ACMP_CR_HYSEN_Msk))) | (u32NegSrc | u32HysteresisEn | ACMP_CR_ACMPEN_Msk);
}

/**
  * @brief  Close analog comparator
  *
  * @param[in]  Acmp The pointer of the specified ACMP module
  * @param[in]  u32ChNum Comparator number.
  *
  * @return None
  *
  * @details  This function will clear ACMPEN bit of ACMP_CR register to disable analog comparator.
  */
void ACMP_Close(ACMP_T *Acmp, uint32_t u32ChNum)
{
    Acmp->CR[u32ChNum % 2] &= (~ACMP_CR_ACMPEN_Msk);
}



/*@}*/ /* end of group NUC029_ACMP_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group NUC029_ACMP_Driver */

/*@}*/ /* end of group NUC029_Device_Driver */

#ifdef __cplusplus
}
#endif

/*** (C) COPYRIGHT 2014 Nuvoton Technology Corp. ***/

