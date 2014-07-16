/**************************************************************************//**
 * @file     timer.h
 * @version  V3.00
 * $Revision: 6 $
 * $Date: 14/01/28 10:49a $
 * @brief    M051 series Timer driver header file
 *
 * @note
 * Copyright (C) 2013 Nuvoton Technology Corp. All rights reserved.
 *****************************************************************************/
#ifndef __TIMER_H__
#define __TIMER_H__

#ifdef __cplusplus
extern "C"
{
#endif


/** @addtogroup M051_Device_Driver M051 Device Driver
  @{
*/

/** @addtogroup M051_TIMER_Driver TIMER Driver
  @{
*/

/** @addtogroup M051_TIMER_EXPORTED_CONSTANTS TIMER Exported Constants
  @{
*/

#define TIMER_ONESHOT_MODE                  (0UL << TIMER_TCSR_MODE_Pos) /*!< Timer working in one-shot mode */
#define TIMER_PERIODIC_MODE                 (1UL << TIMER_TCSR_MODE_Pos) /*!< Timer working in periodic mode */
#define TIMER_TOGGLE_MODE                   (2UL << TIMER_TCSR_MODE_Pos) /*!< Timer working in toggle-output mode */
#define TIMER_CONTINUOUS_MODE               (3UL << TIMER_TCSR_MODE_Pos) /*!< Timer working in continuous counting mode */
#define TIMER_CONTINUOUS_MODE               (3UL << TIMER_TCSR_MODE_Pos) /*!< Timer working in continuous counting mode */
#define TIMER_CAPTURE_FROM_TXEX_PIN         (0UL << TIMER_TCSR_CAP_SRC_Pos) /*!< Timer capture source is from TxEX pin */
#define TIMER_CAPTURE_FROM_INTERNAL_ACMP    (1UL << TIMER_TCSR_CAP_SRC_Pos) /*!< Timer capture source is from internal ACMP output signal */
#define TIMER_TOUT_PIN_FROM_TX_PIN          (0UL << TIMER_TCSR_TOUT_SEL_Pos) /*!< Timer toggle-output pin is from Tx pin */
#define TIMER_TOUT_PIN_FROM_TXEX_PIN        (1UL << TIMER_TCSR_TOUT_SEL_Pos) /*!< Timer toggle-output pin is from TxEX pin */
#define TIMER_CAPTURE_FREE_COUNTING_MODE    (0UL << TIMER_TEXCON_RSTCAPSEL_Pos) /*!< Timer capture event to get timer counter value */
#define TIMER_CAPTURE_COUNTER_RESET_MODE    (1UL << TIMER_TEXCON_RSTCAPSEL_Pos) /*!< Timer capture event to reset timer counter */
#define TIMER_CAPTURE_FALLING_EDGE              (0UL << TIMER_TEXCON_TEX_EDGE_Pos) /*!< Falling edge trigger timer capture */
#define TIMER_CAPTURE_RISING_EDGE               (1UL << TIMER_TEXCON_TEX_EDGE_Pos) /*!< Rising edge trigger timer capture */
#define TIMER_CAPTURE_FALLING_AND_RISING_EDGE   (2UL << TIMER_TEXCON_TEX_EDGE_Pos) /*!< Both falling and rising edge trigger timer capture */
#define TIMER_COUNTER_FALLING_EDGE          (0UL << TIMER_TEXCON_TX_PHASE_Pos) /*!< Counter increase on falling edge */
#define TIMER_COUNTER_RISING_EDGE           (1UL << TIMER_TEXCON_TX_PHASE_Pos) /*!< Counter increase on rising edge */

/*@}*/ /* end of group M051_TIMER_EXPORTED_CONSTANTS */


/** @addtogroup M051_TIMER_EXPORTED_FUNCTIONS TIMER Exported Functions
  @{
*/

/**
  * @brief      Set Timer Compare Value
  *
  * @param[in]  timer       The base address of Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  * @param[in]  u32Value    Timer compare value. Valid values are between 2 to 0xFFFFFF.
  *
  * @return     None
  *
  * @details    This macro is used to set new Timer compared value.
  */
#define TIMER_SET_CMP_VALUE(timer, u32Value)        ((timer)->TCMPR = (u32Value))

/**
  * @brief      Set Timer Prescale Value
  *
  * @param[in]  timer       The base address of Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  * @param[in]  u32Value    Timer prescale value. Valid values are between 0 to 0xFF.
  *
  * @return     None
  *
  * @details    This macro is used to set new Timer prescale value.
  * @note       Clock input is divided by (prescale + 1) before it is fed into timer.
  */
#define TIMER_SET_PRESCALE_VALUE(timer, u32Value)   ((timer)->TCSR = ((timer)->TCSR & ~TIMER_TCSR_PRESCALE_Msk) | (u32Value))

/**
  * @brief      Check specify Timer Status
  *
  * @return     Timer counter is activate or inactivate
  * @retval     0   Timer 24-bit up counter is inactive
  * @retval     1   Timer 24-bit up counter is active
  *
  * @details    This macro is used to check if specify Timer is inactive or active.
  */
#define TIMER_IS_ACTIVE(timer)                      ((timer)->TCSR & TIMER_TCSR_CACT_Msk ? 1 : 0)

/**
  * @brief      Select Toggle-output Pin
  *
  * @param[in]  timer       The base address of Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  * @param[in]  u32ToutSel  Toggle-output pin selection, valid values are:
  *                         - \ref TIMER_TOUT_PIN_FROM_TX_PIN
  *                         - \ref TIMER_TOUT_PIN_FROM_TXEX_PIN
  *
  * @return     None
  *
  * @details    This macro is used to select Timer toggle-output pin.
  */
#define TIMER_SELECT_TOUT_PIN(timer, u32ToutSel)    ((timer)->TCSR = ((timer)->TCSR & ~TIMER_TCSR_TOUT_SEL_Msk) | (u32ToutSel))

/**
  * @details    This function is used to start Timer counting.
  */
static __INLINE void TIMER_Start(TIMER_T *timer)
{
    timer->TCSR |= TIMER_TCSR_CEN_Msk;
}

/**
  * @details    This function is used to stop Timer counting.
  */
static __INLINE void TIMER_Stop(TIMER_T *timer)
{
    timer->TCSR &= ~TIMER_TCSR_CEN_Msk;
}

/**
  * @details    This function is used to enable the Timer wake-up function.
  * @note       To wake the system from Power-down mode, timer clock source must be ether LXT or LIRC.
  */
static __INLINE void TIMER_EnableWakeup(TIMER_T *timer)
{
    timer->TCSR |= TIMER_TCSR_WAKE_EN_Msk;
}

/**
  * @details    This function is used to disable the Timer wake-up function.
  */
static __INLINE void TIMER_DisableWakeup(TIMER_T *timer)
{
    timer->TCSR &= ~TIMER_TCSR_WAKE_EN_Msk;
}

/**
  * @details    This function is used to enable the capture pin detection de-bounce function.
  */
static __INLINE void TIMER_EnableCaptureDebounce(TIMER_T *timer)
{
    timer->TEXCON |= TIMER_TEXCON_TEXDB_Msk;
}

/**
  * @details    This function is used to disable the capture pin detection de-bounce function.
  */
static __INLINE void TIMER_DisableCaptureDebounce(TIMER_T *timer)
{
    timer->TEXCON &= ~TIMER_TEXCON_TEXDB_Msk;
}

/**
  * @details    This function is used to enable the counter pin detection de-bounce function.
  */
static __INLINE void TIMER_EnableEventCounterDebounce(TIMER_T *timer)
{
    timer->TEXCON |= TIMER_TEXCON_TCDB_Msk;
}

/**
  * @details    This function is used to disable the counter pin detection de-bounce function.
  */
static __INLINE void TIMER_DisableEventCounterDebounce(TIMER_T *timer)
{
    timer->TEXCON &= ~TIMER_TEXCON_TCDB_Msk;
}

/**
  * @details    This function is used to enable the Timer time-out interrupt function.
  */
static __INLINE void TIMER_EnableInt(TIMER_T *timer)
{
    timer->TCSR |= TIMER_TCSR_IE_Msk;
}

/**
  * @details    This function is used to disable the Timer time-out interrupt function.
  */
static __INLINE void TIMER_DisableInt(TIMER_T *timer)
{
    timer->TCSR &= ~TIMER_TCSR_IE_Msk;
}

/**
  * @details    This function is used to enable the Timer capture trigger interrupt function.
  */
static __INLINE void TIMER_EnableCaptureInt(TIMER_T *timer)
{
    timer->TEXCON |= TIMER_TEXCON_TEXIEN_Msk;
}

/**
  * @details    This function is used to disable the Timer capture trigger interrupt function.
  */
static __INLINE void TIMER_DisableCaptureInt(TIMER_T *timer)
{
    timer->TEXCON &= ~TIMER_TEXCON_TEXIEN_Msk;
}

/**
  * @brief      Get Timer Time-out Interrupt Flag
  *
  * @param[in]  timer   The base address of Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @return     Timer time-out interrupt occurred or not
  * @retval     0   Timer time-out interrupt did not occur
  * @retval     1   Timer time-out interrupt occurred
  *
  * @details    This function indicates Timer time-out interrupt occurred or not.
  */
static __INLINE uint32_t TIMER_GetIntFlag(TIMER_T *timer)
{
    return (timer->TISR & TIMER_TISR_TIF_Msk ? 1 : 0);
}

/**
  * @details    This function clears Timer time-out interrupt flag.
  */
static __INLINE void TIMER_ClearIntFlag(TIMER_T *timer)
{
    timer->TISR = TIMER_TISR_TIF_Msk;
}

/**
  * @brief      Get Timer Capture Interrupt Flag
  *
  * @param[in]  timer   The base address of Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @return     Timer capture interrupt occurred or not
  * @retval     0   Timer capture interrupt did not occur
  * @retval     1   Timer capture interrupt occurred
  *
  * @details    This function indicates Timer capture interrupt occurred or not.
  */
static __INLINE uint32_t TIMER_GetCaptureIntFlag(TIMER_T *timer)
{
    return timer->TEXISR;
}

/**
  * @details    This function clears Timer capture interrupt flag.
  */
static __INLINE void TIMER_ClearCaptureIntFlag(TIMER_T *timer)
{
    timer->TEXISR = TIMER_TEXISR_TEXIF_Msk;
}

/**
  * @brief      Get Timer Wakeup Flag
  *
  * @param[in]  timer   The base address of Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @return     Timer has waked up system or not
  * @retval     0   Timer did not wake up system
  * @retval     1   Timer Timer wake up system
  *
  * @details    This function indicates Timer has waked up system or not.
  */
static __INLINE uint32_t TIMER_GetWakeupFlag(TIMER_T *timer)
{
    return (timer->TISR & TIMER_TISR_TWF_Msk ? 1 : 0);
}

/**
  * @details    This function clears the Timer wake-up system flag.
  */
static __INLINE void TIMER_ClearWakeupFlag(TIMER_T *timer)
{
    timer->TISR = TIMER_TISR_TWF_Msk;
}

/**
  * @details    This function reports the current timer capture data value.
  */
static __INLINE uint32_t TIMER_GetCaptureData(TIMER_T *timer)
{
    return timer->TCAP;
}

/**
  * @details    This function reports the current timer counter value.
  */
static __INLINE uint32_t TIMER_GetCounter(TIMER_T *timer)
{
    return timer->TDR;
}

uint32_t TIMER_Open(TIMER_T *timer, uint32_t u32Mode, uint32_t u32Freq);
void TIMER_Close(TIMER_T *timer);
void TIMER_Delay(TIMER_T *timer, uint32_t u32Usec);
void TIMER_EnableCapture(TIMER_T *timer, uint32_t u32CapMode, uint32_t u32Edge);
void TIMER_DisableCapture(TIMER_T *timer);
void TIMER_EnableEventCounter(TIMER_T *timer, uint32_t u32Edge);
void TIMER_DisableEventCounter(TIMER_T *timer);
uint32_t TIMER_GetModuleClock(TIMER_T *timer);

/*@}*/ /* end of group M051_TIMER_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group M051_TIMER_Driver */

/*@}*/ /* end of group M051_Device_Driver */

#ifdef __cplusplus
}
#endif

#endif //__TIMER_H__

/*** (C) COPYRIGHT 2013 Nuvoton Technology Corp. ***/
