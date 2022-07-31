/**************************************************************************//**
 * @file     timer.h
 * @version  V3.00
 * $Revision: 10 $
 * $Date: 15/05/20 2:07p $
 * @brief    M051 series Timer driver header file
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 *
 * Copyright (C) 2013 Nuvoton Technology Corp. All rights reserved.
 *****************************************************************************/
#ifndef __TIMER_H__
#define __TIMER_H__

#ifdef __cplusplus
extern "C"
{
#endif


/** @addtogroup Standard_Driver Standard Driver
  @{
*/

/** @addtogroup TIMER_Driver TIMER Driver
  @{
*/

/** @addtogroup TIMER_EXPORTED_CONSTANTS TIMER Exported Constants
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

/*@}*/ /* end of group TIMER_EXPORTED_CONSTANTS */


/** @addtogroup TIMER_EXPORTED_FUNCTIONS TIMER Exported Functions
  @{
*/

/**
  * @brief      Set Timer Compared Value
  *
  * @param[in]  timer       The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  * @param[in]  u32Value    Timer compare value. Valid values are between 2 to 0xFFFFFF.
  *
  * @return     None
  *
  * @details    This macro is used to set timer compared value to adjust timer time-out interval.
  * @note       1. Never write 0x0 or 0x1 in this field, or the core will run into unknown state. \n
  *             2. If update timer compared value in continuous counting mode, timer counter value will keep counting continuously. \n
  *                But if timer is operating at other modes, the timer up counter will restart counting and start from 0.
  */
#define TIMER_SET_CMP_VALUE(timer, u32Value)        ((timer)->TCMPR = (u32Value))

/**
  * @brief      Set Timer Prescale Value
  *
  * @param[in]  timer       The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  * @param[in]  u32Value    Timer prescale value. Valid values are between 0 to 0xFF.
  *
  * @return     None
  *
  * @details    This macro is used to set timer prescale value and timer source clock will be divided by (prescale + 1) \n
  *             before it is fed into timer.
  */
#define TIMER_SET_PRESCALE_VALUE(timer, u32Value)   ((timer)->TCSR = ((timer)->TCSR & ~TIMER_TCSR_PRESCALE_Msk) | (u32Value))

/**
  * @brief      Check specify Timer Status
  *
  * @param[in]  timer       The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @retval     0   Timer 24-bit up counter is inactive
  * @retval     1   Timer 24-bit up counter is active
  *
  * @details    This macro is used to check if specify Timer counter is inactive or active.
  */
#define TIMER_IS_ACTIVE(timer)                      ((timer)->TCSR & TIMER_TCSR_CACT_Msk ? 1 : 0)

/**
  * @brief      Select Toggle-output Pin
  *
  * @param[in]  timer       The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  * @param[in]  u32ToutSel  Toggle-output pin selection, valid values are:
  *                         - \ref TIMER_TOUT_PIN_FROM_TX_PIN
  *                         - \ref TIMER_TOUT_PIN_FROM_TXEX_PIN
  *
  * @return     None
  *
  * @details    This macro is used to select timer toggle-output pin is output on Tx or TxEX pin.
  */
#define TIMER_SELECT_TOUT_PIN(timer, u32ToutSel)    ((timer)->TCSR = ((timer)->TCSR & ~TIMER_TCSR_TOUT_SEL_Msk) | (u32ToutSel))

/**
  * @brief      Start Timer Counting
  *
  * @param[in]  timer       The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @return     None
  *
  * @details    This function is used to start Timer counting.
  */
static __INLINE void TIMER_Start(TIMER_T *timer)
{
    timer->TCSR |= TIMER_TCSR_CEN_Msk;
}

/**
  * @brief      Stop Timer Counting
  *
  * @param[in]  timer       The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @return     None
  *
  * @details    This function is used to stop/suspend Timer counting.
  */
static __INLINE void TIMER_Stop(TIMER_T *timer)
{
    timer->TCSR &= ~TIMER_TCSR_CEN_Msk;
}

/**
  * @brief      Enable Timer Interrupt Wake-up Function
  *
  * @param[in]  timer       The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @return     None
  *
  * @details    This function is used to enable the timer interrupt wake-up function and interrupt source could be time-out interrupt, \n
  *             counter event interrupt or capture trigger interrupt.
  * @note       To wake the system from Power-down mode, timer clock source must be selected LIRC.
  */
static __INLINE void TIMER_EnableWakeup(TIMER_T *timer)
{
    timer->TCSR |= TIMER_TCSR_WAKE_EN_Msk;
}

/**
  * @brief      Disable Timer Wake-up Function
  *
  * @param[in]  timer       The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @return     None
  *
  * @details    This function is used to disable the timer interrupt wake-up function.
  */
static __INLINE void TIMER_DisableWakeup(TIMER_T *timer)
{
    timer->TCSR &= ~TIMER_TCSR_WAKE_EN_Msk;
}

/**
  * @brief      Enable Capture Pin De-bounce
  *
  * @param[in]  timer       The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @return     None
  *
  * @details    This function is used to enable the detect de-bounce function of capture pin.
  */
static __INLINE void TIMER_EnableCaptureDebounce(TIMER_T *timer)
{
    timer->TEXCON |= TIMER_TEXCON_TEXDB_Msk;
}

/**
  * @brief      Disable Capture Pin De-bounce
  *
  * @param[in]  timer       The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @return     None
  *
  * @details    This function is used to disable the detect de-bounce function of capture pin.
  */
static __INLINE void TIMER_DisableCaptureDebounce(TIMER_T *timer)
{
    timer->TEXCON &= ~TIMER_TEXCON_TEXDB_Msk;
}

/**
  * @brief      Enable Counter Pin De-bounce
  *
  * @param[in]  timer       The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @return     None
  *
  * @details    This function is used to enable the detect de-bounce function of counter pin.
  */
static __INLINE void TIMER_EnableEventCounterDebounce(TIMER_T *timer)
{
    timer->TEXCON |= TIMER_TEXCON_TCDB_Msk;
}

/**
  * @brief      Disable Counter Pin De-bounce
  *
  * @param[in]  timer       The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @return     None
  *
  * @details    This function is used to disable the detect de-bounce function of counter pin.
  */
static __INLINE void TIMER_DisableEventCounterDebounce(TIMER_T *timer)
{
    timer->TEXCON &= ~TIMER_TEXCON_TCDB_Msk;
}

/**
  * @brief      Enable Timer Time-out Interrupt
  *
  * @param[in]  timer       The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @return     None
  *
  * @details    This function is used to enable the timer time-out interrupt function.
  */
static __INLINE void TIMER_EnableInt(TIMER_T *timer)
{
    timer->TCSR |= TIMER_TCSR_IE_Msk;
}

/**
  * @brief      Disable Timer Time-out Interrupt
  *
  * @param[in]  timer       The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @return     None
  *
  * @details    This function is used to disable the timer time-out interrupt function.
  */
static __INLINE void TIMER_DisableInt(TIMER_T *timer)
{
    timer->TCSR &= ~TIMER_TCSR_IE_Msk;
}

/**
  * @brief      Enable Capture Trigger Interrupt
  *
  * @param[in]  timer       The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @return     None
  *
  * @details    This function is used to enable the timer capture trigger interrupt function.
  */
static __INLINE void TIMER_EnableCaptureInt(TIMER_T *timer)
{
    timer->TEXCON |= TIMER_TEXCON_TEXIEN_Msk;
}

/**
  * @brief      Disable Capture Trigger Interrupt
  *
  * @param[in]  timer       The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @return     None
  *
  * @details    This function is used to disable the timer capture trigger interrupt function.
  */
static __INLINE void TIMER_DisableCaptureInt(TIMER_T *timer)
{
    timer->TEXCON &= ~TIMER_TEXCON_TEXIEN_Msk;
}

/**
  * @brief      Get Timer Time-out Interrupt Flag
  *
  * @param[in]  timer   The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @retval     0   Timer time-out interrupt did not occur
  * @retval     1   Timer time-out interrupt occurred
  *
  * @details    This function indicates timer time-out interrupt occurred or not.
  */
static __INLINE uint32_t TIMER_GetIntFlag(TIMER_T *timer)
{
    return (timer->TISR & TIMER_TISR_TIF_Msk ? 1 : 0);
}

/**
  * @brief      Clear Timer Time-out Interrupt Flag
  *
  * @param[in]  timer       The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @return     None
  *
  * @details    This function clears timer time-out interrupt flag to 0.
  */
static __INLINE void TIMER_ClearIntFlag(TIMER_T *timer)
{
    timer->TISR = TIMER_TISR_TIF_Msk;
}

/**
  * @brief      Get Timer Capture Interrupt Flag
  *
  * @param[in]  timer   The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @retval     0   Timer capture interrupt did not occur
  * @retval     1   Timer capture interrupt occurred
  *
  * @details    This function indicates timer capture trigger interrupt occurred or not.
  */
static __INLINE uint32_t TIMER_GetCaptureIntFlag(TIMER_T *timer)
{
    return timer->TEXISR;
}

/**
  * @brief      Clear Timer Capture Interrupt Flag
  *
  * @param[in]  timer       The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @return     None
  *
  * @details    This function clears timer capture trigger interrupt flag to 0.
  */
static __INLINE void TIMER_ClearCaptureIntFlag(TIMER_T *timer)
{
    timer->TEXISR = TIMER_TEXISR_TEXIF_Msk;
}

/**
  * @brief      Get Timer Wake-up Flag
  *
  * @param[in]  timer   The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @retval     0   Timer does not cause CPU wake-up
  * @retval     1   Timer interrupt event cause CPU wake-up
  *
  * @details    This function indicates timer interrupt event has waked up system or not.
  */
static __INLINE uint32_t TIMER_GetWakeupFlag(TIMER_T *timer)
{
    return (timer->TISR & TIMER_TISR_TWF_Msk ? 1 : 0);
}

/**
  * @brief      Clear Timer Wake-up Flag
  *
  * @param[in]  timer       The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @return     None
  *
  * @details    This function clears the timer wake-up system flag to 0.
  */
static __INLINE void TIMER_ClearWakeupFlag(TIMER_T *timer)
{
    timer->TISR = TIMER_TISR_TWF_Msk;
}

/**
  * @brief      Get Capture value
  *
  * @param[in]  timer       The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @return     24-bit Capture Value
  *
  * @details    This function reports the current 24-bit timer capture value.
  */
static __INLINE uint32_t TIMER_GetCaptureData(TIMER_T *timer)
{
    return timer->TCAP;
}

/**
  * @brief      Get Counter value
  *
  * @param[in]  timer       The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @return     24-bit Counter Value
  *
  * @details    This function reports the current 24-bit timer counter value.
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

/*@}*/ /* end of group TIMER_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group TIMER_Driver */

/*@}*/ /* end of group Standard_Driver */

#ifdef __cplusplus
}
#endif

#endif //__TIMER_H__

/*** (C) COPYRIGHT 2013 Nuvoton Technology Corp. ***/
