/**************************************************************************//**
 * @file     I2C.h
 * @version  V3.0
 * $Revision: 9 $
 * $Date: 14/02/10 3:04p $
 * @brief    M051 Series I2C Driver Header File
 *
 * @note
 * Copyright (C) 2014 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#ifndef __I2C_H__
#define __I2C_H__

#include "M051Series.h"

#ifdef __cplusplus
extern "C"
{
#endif


/** @addtogroup M051_Device_Driver M051 Device Driver
  @{
*/

/** @addtogroup M051_I2C_Driver I2C Driver
  @{
*/

/** @addtogroup M051_I2C_EXPORTED_CONSTANTS I2C Exported Constants
  @{
*/

/*---------------------------------------------------------------------------------------------------------*/
/*  I2CON constant definitions.                                                                            */
/*---------------------------------------------------------------------------------------------------------*/
#define I2C_I2CON_STA_STO_SI        0x38UL /*!< I2CON setting for I2C control bits. It would set STA, STO and SI bits     */
#define I2C_I2CON_STA_STO_SI_AA     0x3CUL /*!< I2CON setting for I2C control bits. It would set STA, STO, SI and AA bits */
#define I2C_I2CON_STA_SI            0x28UL /*!< I2CON setting for I2C control bits. It would set STA and SI bits          */
#define I2C_I2CON_STA_SI_AA         0x2CUL /*!< I2CON setting for I2C control bits. It would set STA, SI and AA bits      */
#define I2C_I2CON_STO_SI            0x18UL /*!< I2CON setting for I2C control bits. It would set STO and SI bits          */
#define I2C_I2CON_STO_SI_AA         0x1CUL /*!< I2CON setting for I2C control bits. It would set STO, SI and AA bits      */
#define I2C_I2CON_SI                0x08UL /*!< I2CON setting for I2C control bits. It would set SI bit                   */
#define I2C_I2CON_SI_AA             0x0CUL /*!< I2CON setting for I2C control bits. It would set SI and AA bits           */
#define I2C_I2CON_STA               0x20UL /*!< I2CON setting for I2C control bits. It would set STA bit                  */
#define I2C_I2CON_STO               0x10UL /*!< I2CON setting for I2C control bits. It would set STO bit                                    */
#define I2C_I2CON_AA                0x04UL /*!< I2CON setting for I2C control bits. It would set AA bit                   */

/*@}*/ /* end of group M051_I2C_EXPORTED_CONSTANTS */

/** @addtogroup M051_I2C_EXPORTED_FUNCTIONS I2C Exported Functions
  @{
*/
/**
 *    @brief        The macro is used to set I2C bus condition at One Time
 *
 *    @param[in]    i2c        Specify I2C port
 *    @return       None
 *
 *    @details      To control I2C bus conditions of START, STOP, SI, ACK.
 */
#define I2C_SET_CONTROL_REG(i2c, u8Ctrl) ((i2c)->I2CON = ((i2c)->I2CON & ~0x3c) | u8Ctrl)

/**
 *    @brief        The macro is used to set START condition of I2C Bus
 *
 *    @param[in]    i2c        Specify I2C port
 *    @return       None
 *
 *    @details      To set the I2C bus START condition
 */
#define I2C_START(i2c)  ((i2c)->I2CON = ((i2c)->I2CON & ~I2C_I2CON_SI_Msk) | I2C_I2CON_STA_Msk)

/**
 *    @brief        The macro is used to set STOP condition of I2C Bus
 *
 *    @param[in]    i2c        Specify I2C port
 *    @return       None
 *
 *    @details      To set the I2C bus STOP condition
 */
#define I2C_STOP(i2c)   ((i2c)->I2CON = ((i2c)->I2CON & ~I2C_I2CON_SI_Msk) | I2C_I2CON_STO_Msk)

/**
 *    @brief        The macro is used to wait I2C bus status get ready
 *
 *    @param[in]    i2c        Specify I2C port
 *    @return       None
 *
 *    @details      When a new status is presented of I2C bus, SI flag will be set.
 */
#define I2C_WAIT_READY(i2c)     while(!((i2c)->I2CON & I2C_I2CON_SI_Msk))

/**
 *    @brief        The macro is used to Read I2C Bus Data Register
 *
 *    @param[in]    i2c        Specify I2C port
 *    @return       A byte of I2C data register
 *
 *    @details      I2C controller read data from bus and save it in I2CDAT
 */
#define I2C_GET_DATA(i2c)   ((i2c)->I2CDAT)

/**
 *    @brief        The macro is used to write a Data to I2C Data Register
 *
 *    @param[in]    i2c         Specify I2C port
 *    @return       None
 *
 *    @details      I2C controller send a byte data to bus.
 */
#define I2C_SET_DATA(i2c, u8Data) ((i2c)->I2CDAT = u8Data)

/**
 *    @brief        The macro is used to get I2C Bus Status Code
 *
 *    @param[in]    i2c        Specify I2C port
 *    @return       I2C status code
 *
 *    @details       To get ths status code of I2C Bus.
 */
#define I2C_GET_STATUS(i2c) ((i2c)->I2CSTATUS)

/**
 *    @brief        The macro is used to get time-out flag
 *
 *    @param[in]    i2c     Specify I2C port
 *    @return       Time-out flag status
 *    @retval       0       Bus time-out is not happened
 *    @retval       1       Bus time-out is happened
 *
 *    @details      To get time-out flag of I2C bus
 */
#define I2C_GET_TIMEOUT_FLAG(i2c)   ( (i2c->I2CTOC & I2C_I2CTOC_TIF_Msk) == I2C_I2CTOC_TIF_Msk ? 1:0 )

/**
 *    @brief        The macro is used to get wake-up flag
 *
 *    @param[in]    i2c     Specify I2C port
 *    @return       Wake-up flag status
 *    @retval       0       Chip is not woken-up from power-down mode
 *    @retval       1       Chip is woken-up from power-down mode
 *
 *    @detail       To get wake-up status of I2C bus
 */
#define I2C_GET_WAKEUP_FLAG(i2c) ( ((i2c)->I2CWKUPSTS & I2C_I2CWKUPSTS_WKUPIF_Msk) == I2C_I2CWKUPSTS_WKUPIF_Msk ? 1:0  )

/**
 *    @brief        The macro is used to clear wake-up flag
 *
 *    @param[in]    i2c     Specify I2C port
 *    @return       None
 *
 *    @detail       To clear wake-up flag of I2C bus when wake-up flag is set.
 */
#define I2C_CLEAR_WAKEUP_FLAG(i2c)  ((i2c)->I2CWKUPSTS |= I2C_I2CWKUPSTS_WKUPIF_Msk)

void I2C_ClearTimeoutFlag(I2C_T *i2c);
void I2C_Close(I2C_T *i2c);
void I2C_Trigger(I2C_T *i2c, uint8_t u8Start, uint8_t u8Stop, uint8_t u8Si, uint8_t u8Ack);
void I2C_DisableInt(I2C_T *i2c);
void I2C_EnableInt(I2C_T *i2c);
uint32_t I2C_GetBusClockFreq(I2C_T *i2c);
uint32_t I2C_GetIntFlag(I2C_T *i2c);
uint32_t I2C_GetStatus(I2C_T *i2c);
uint32_t I2C_Open(I2C_T *i2c, uint32_t u32BusClock);
uint8_t I2C_GetData(I2C_T *i2c);
void I2C_SetSlaveAddr(I2C_T *i2c, uint8_t u8SlaveNo, uint8_t u8SlaveAddr, uint8_t u8GCMode);
void I2C_SetSlaveAddrMask(I2C_T *i2c, uint8_t u8SlaveNo, uint8_t u8SlaveAddrMask);
uint32_t I2C_SetClockBusFreq(I2C_T *i2c, uint32_t u32BusClock);
void I2C_EnableTimeout(I2C_T *i2c, uint8_t u8LongTimeout);
void I2C_DisableTimeout(I2C_T *i2c);
void I2C_EnableWakeup(I2C_T *i2c);
void I2C_DisableWakeup(I2C_T *i2c);
void I2C_SetData(I2C_T *i2c, uint8_t u8Data);

/*@}*/ /* end of group M051_I2C_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group M051_I2C_Driver */

/*@}*/ /* end of group M051_Device_Driver */

#ifdef __cplusplus
}
#endif
#endif //__I2C_H__
