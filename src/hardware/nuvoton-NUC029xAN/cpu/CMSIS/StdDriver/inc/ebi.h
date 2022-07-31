/**************************************************************************//**
 * @file     ebi.h
 * @version  V3.00
 * $Revision: 2 $
 * $Date: 14/10/03 5:38p $
 * @brief    NUC029 series EBI driver header file
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2013 Nuvoton Technology Corp. All rights reserved.
 *****************************************************************************/
#ifndef __EBI_H__
#define __EBI_H__

#ifdef __cplusplus
extern "C"
{
#endif


/** @addtogroup NUC029_Device_Driver NUC029 Device Driver
  @{
*/

/** @addtogroup NUC029_EBI_Driver EBI Driver
  @{
*/

/** @addtogroup NUC029_EBI_EXPORTED_CONSTANTS EBI Exported Constants
  @{
*/
/*---------------------------------------------------------------------------------------------------------*/
/*  Miscellaneous Constant Definitions                                                                     */
/*---------------------------------------------------------------------------------------------------------*/
#define EBI_BASE_ADDR           0x60000000UL /*!< EBI base address */
#define EBI_MAX_SIZE            0x00020000UL /*!< Maximum EBI size */

/*---------------------------------------------------------------------------------------------------------*/
/*  Constants for EBI data bus width                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
#define EBI_BUSWIDTH_8BIT       8     /*!< EBI bus width is 8-bit */
#define EBI_BUSWIDTH_16BIT      16    /*!< EBI bus width is 16-bit */

/*---------------------------------------------------------------------------------------------------------*/
/*  Constants for EBI MCLK divider and Timing                                                              */
/*---------------------------------------------------------------------------------------------------------*/
#define EBI_MCLKDIV_1           0x0UL /*!< EBI output clock(MCLK) is HCLK/1 */
#define EBI_MCLKDIV_2           0x1UL /*!< EBI output clock(MCLK) is HCLK/2 */
#define EBI_MCLKDIV_4           0x2UL /*!< EBI output clock(MCLK) is HCLK/4 */
#define EBI_MCLKDIV_8           0x3UL /*!< EBI output clock(MCLK) is HCLK/8 */
#define EBI_MCLKDIV_16          0x4UL /*!< EBI output clock(MCLK) is HCLK/16 */
#define EBI_MCLKDIV_32          0x5UL /*!< EBI output clock(MCLK) is HCLK/32 */

#define EBI_TIMING_FASTEST      0x0UL /*!< EBI timing is the fastest */
#define EBI_TIMING_VERYFAST     0x1UL /*!< EBI timing is very fast */
#define EBI_TIMING_FAST         0x2UL /*!< EBI timing is fast */
#define EBI_TIMING_NORMAL       0x3UL /*!< EBI timing is normal  */
#define EBI_TIMING_SLOW         0x4UL /*!< EBI timing is slow */
#define EBI_TIMING_VERYSLOW     0x5UL /*!< EBI timing is very slow */
#define EBI_TIMING_SLOWEST      0x6UL /*!< EBI timing is the slowest */

/*@}*/ /* end of group NUC029_EBI_EXPORTED_CONSTANTS */


/** @addtogroup NUC029_EBI_EXPORTED_FUNCTIONS EBI Exported Functions
  @{
*/

/**
  * @brief      Read 8-bit data
  *
  * @param[in]  u32Addr     The data address on EBI bus.
  *
  * @return     8-bit Data
  *
  * @details    This macro read 8-bit data from specify EBI address.
  */
#define EBI_READ_DATA8(u32Addr)             (*((volatile unsigned char *)(EBI_BASE_ADDR+(u32Addr))))

/**
  * @brief      Write 8-bit data
  *
  * @param[in]  u32Addr     The data address on EBI bus.
  * @param[in]  u32Data     Specify data to be written.
  *
  * @return     None
  *
  * @details    This macro write 8-bit data to specify EBI address.
  */
#define EBI_WRITE_DATA8(u32Addr, u32Data)   (*((volatile unsigned char *)(EBI_BASE_ADDR+(u32Addr))) = (u32Data))

/**
  * @brief      Read 16-bit data
  *
  * @param[in]  u32Addr     The data address on EBI bus.
  *
  * @return     16-bit Data
  *
  * @details    This macro read 16-bit data from specify EBI address.
  */
#define EBI_READ_DATA16(u32Addr)            (*((volatile unsigned short *)(EBI_BASE_ADDR+(u32Addr))))

/**
  * @brief      Write 16-bit data
  *
  * @param[in]  u32Addr     The data address on EBI bus.
  * @param[in]  u32Data     Specify data to be written.
  *
  * @return     None
  *
  * @details    This macro write 16-bit data to specify EBI address.
  */
#define EBI_WRITE_DATA16(u32Addr, u32Data)  (*((volatile unsigned short *)(EBI_BASE_ADDR+(u32Addr))) = (u32Data))

/**
  * @brief      Read 32-bit data
  *
  * @param[in]  u32Addr     The data address on EBI bus.
  *
  * @return     32-bit Data
  *
  * @details    This macro read 32-bit data from specify EBI address.
  */
#define EBI_READ_DATA32(u32Addr)            (*((volatile unsigned int *)(EBI_BASE_ADDR+(u32Addr))))

/**
  * @brief      Write 32-bit data
  *
  * @param[in]  u32Addr     The data address on EBI bus.
  * @param[in]  u32Data     Specify data to be written.
  *
  * @return     None
  *
  * @details    This macro write 32-bit data to specify EBI address.
  */
#define EBI_WRITE_DATA32(u32Addr, u32Data)  (*((volatile unsigned int *)(EBI_BASE_ADDR+(u32Addr))) = (u32Data))

void EBI_Open(uint32_t u32Bank, uint32_t u32DataWidth, uint32_t u32TimingClass, uint32_t u32BusMode, uint32_t u32CSActiveLevel);
void EBI_Close(uint32_t u32Bank);
void EBI_SetBusTiming(uint32_t u32Bank, uint32_t u32TimingConfig, uint32_t u32MclkDiv);

/*@}*/ /* end of group NUC029_EBI_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group NUC029_EBI_Driver */

/*@}*/ /* end of group NUC029_Device_Driver */

#ifdef __cplusplus
}
#endif

#endif //__EBI_H__

/*** (C) COPYRIGHT 2013 Nuvoton Technology Corp. ***/
