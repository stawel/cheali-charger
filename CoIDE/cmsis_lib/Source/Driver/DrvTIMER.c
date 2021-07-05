//*****************************************************************************
//
//! \file DrvTIMER.c
//! \brief Driver for the TIMER Controller.
//! \version v1.02.002
//! \date 09/05/2011
//! \author Nuvoton
//! \copy
//!
//! Copyright (c)  2011, Nuvoton 
//! All rights reserved.
//
//*****************************************************************************
 
//*****************************************************************************
//
//! Includes of system headers
//
//*****************************************************************************

//*****************************************************************************
//
//! Includes of local headers
//
//*****************************************************************************
#include "DrvTIMER.h"
#include "DrvSYS.h"
#include "DrvGPIO.h"

//*****************************************************************************
//
//! Macro, type and constant definitions
//
//*****************************************************************************
#define TIMER_EVENT_COUNT   1
const uint32_t CH_OFFSET[] = {0x0, 0x20, 0x100000, 0x100020};


//*****************************************************************************
//
//! Global file scope (static) variables
//
//*****************************************************************************
static TIMER_EVENT_T tTime0Event[TIMER_EVENT_COUNT],
                     tTime1Event[TIMER_EVENT_COUNT],
                     tTime2Event[TIMER_EVENT_COUNT],
                     tTime3Event[TIMER_EVENT_COUNT];

int32_t volatile bIsTimer0Initial     = FALSE,
                 bIsTimer1Initial     = FALSE,
                 bIsTimer2Initial     = FALSE,
                 bIsTimer3Initial     = FALSE,
                 bIsTimer0Used        = FALSE,
                 bIsTimer1Used        = FALSE,
                 bIsTimer2Used        = FALSE,
                 bIsTimer3Used        = FALSE,
                 bIsSetTime0Event     = FALSE,
                 bIsSetTime1Event     = FALSE,
                 bIsSetTime2Event     = FALSE,
                 bIsSetTime3Event     = FALSE;

static uint32_t volatile uTimer0Tick = 0,
                         uTimer1Tick = 0,
                         uTimer2Tick = 0,
                         uTimer3Tick = 0,
                         uTime0EventCount = 0,
                         uTime1EventCount = 0,
                         uTime2EventCount = 0,
                         uTime3EventCount = 0;

static uint32_t volatile _sys_uTimer0TickPerSecond,
                         _sys_uTimer1TickPerSecond,
                         _sys_uTimer2TickPerSecond, 
                         _sys_uTimer3TickPerSecond;

uint32_t volatile u32EXTClockFreq = 12000000;

static WDT_CALLBACK fnCallBack_WDT;

//*****************************************************************************
//
//! \brief Get the timer clock from the specified timer channel. 
//!
//! \param ch  E_TIMER_CHANNEL, it could be E_TMR0/E_TMR1/E_TMR2/E_TMR3
//!
//! \return u32clk The timer clock (Hz)
//!         E_DRVTIMER_CHANNEL Invalid Timer channel
//!         E_DRVTIMER_CLOCK_RATE Invalid Timer clock source 
//
//*****************************************************************************
uint32_t GetTimerClock(E_TIMER_CHANNEL ch)
{
    volatile uint8_t u8ClockSrc;
    volatile uint32_t u32clk = 0;
        
    if ((ch == E_TMR0) || (ch == E_TMR1) || (ch == E_TMR2) || (ch == E_TMR3))
    {
        u8ClockSrc = (inpw(&SYSCLK->CLKSEL1) >> (8+(ch*4))) & 0x7;

        if (u8ClockSrc == 0)           
        {
            //
            // Option 0: Get External Clock From DrvSYS Setting
            //
            u32clk = DrvSYS_GetExtClockFreq() ;     
        }
        else if(u8ClockSrc == 2)
        {
            //
            // Option 2: HCLK
            //
            u32clk = DrvSYS_GetHCLKFreq();          
        }
        else if(u8ClockSrc == 7)
        {
            //
            // Option 7: 22MHz
            //
            u32clk = __IRC22M;                      
        }else
        {
            return E_DRVTIMER_CLOCK_RATE;
        }
    }
    else 
        return E_DRVTIMER_CHANNEL;
            
    return u32clk;
}

//*****************************************************************************
//
//! \brief Calculate the Timer Compare Value and pre-scale value for Timer 
//!
//! \param u32ClockValue Input the clock value of Timer
//!
//! \param u32TicksPerSec Specify the ticks per second of Timer
//!
//! \return Return 32 bits unsigned integer where bits [ 0:23] - The Timer
//!  Compare Value(TCMPR) for Timer bits [24:31] - The pre-scale value for Timer
//!  (uint32_t)-1  Out of range 
//
//*****************************************************************************
uint32_t CalTimerInitValue(uint32_t u32ClockValue, uint32_t u32TicksPerSec)
{
    uint32_t u32PreScale;
    uint32_t u32TCMPRValue;

    if ((u32ClockValue < 2) || (u32TicksPerSec == 0))
        return (uint32_t)-1;

    for (u32PreScale=1; u32PreScale<256; u32PreScale++)
    {
        u32TCMPRValue = u32ClockValue / (u32TicksPerSec * u32PreScale);

        //
        // The TCMPR value must > 1 
        //
        if ((u32TCMPRValue > 1) && (u32TCMPRValue < 0x1000000))
            return (((u32PreScale-1) << 24) | u32TCMPRValue);
    }
    
    return (uint32_t)-1;
}

//*****************************************************************************
//
//! \brief The TIMER0 default IRQ, declared in startup_NUC1xx.s 
//!
//! \param None 
//!
//! \return None 
//
//*****************************************************************************
/*
void TMR0_IRQHandler(void)
{
    int32_t i;
   
    TIMER0->TISR.TIF = 1;

    uTimer0Tick++;
    //
    // Timer Event Handle
    //
    if (bIsSetTime0Event)                       
    {
        for (i=0; i<TIMER_EVENT_COUNT; i++)
        {
            if (tTime0Event[i].active)
            {
                tTime0Event[i].curTick--;
                if (tTime0Event[i].curTick == 0)
                {
                    (*tTime0Event[i].funPtr)(tTime0Event[i].transParam);
                    tTime0Event[i].curTick = tTime0Event[i].initTick;
                }
            }
        }
    }
}
*/
//*****************************************************************************
//
//! \brief The TIMER1 default IRQ, declared in startup_NUC1xx.s 
//!
//! \param None 
//!
//! \return None 
//
//*****************************************************************************
void TMR1_IRQHandler(void)
{
    int32_t i;
    
    TIMER1->TISR.TIF = 1;

    uTimer1Tick++;
    //
    // Timer Event Handle
    //
    if (bIsSetTime1Event)                       
    {
        for (i=0; i<TIMER_EVENT_COUNT; i++)
        {
            if (tTime1Event[i].active)
            {
                tTime1Event[i].curTick--;
                if (tTime1Event[i].curTick == 0)
                {
                    (*tTime1Event[i].funPtr)(tTime1Event[i].transParam);
                    tTime1Event[i].curTick = tTime1Event[i].initTick;
                }
            }
        }
    }
}

//*****************************************************************************
//
//! \brief The TIMER2 default IRQ, declared in startup_NUC1xx.s 
//!
//! \param None 
//!
//! \return None 
//
//*****************************************************************************
void TMR2_IRQHandler(void)
{
    int32_t i;
    
    TIMER2->TISR.TIF = 1;

    uTimer2Tick++;
    //
    // Timer Event Handle
    //
    if (bIsSetTime2Event)                       
    {
        for (i=0; i<TIMER_EVENT_COUNT; i++)
        {
            if (tTime2Event[i].active)
            {
                tTime2Event[i].curTick--;
                if (tTime2Event[i].curTick == 0)
                {
                    (*tTime2Event[i].funPtr)(tTime2Event[i].transParam);
                    tTime2Event[i].curTick = tTime2Event[i].initTick;
                }
            }
        }
    }
}

//*****************************************************************************
//
//! \brief The TIMER3 default IRQ, declared in startup_NUC1xx.s 
//!
//! \param None 
//!
//! \return None 
//
//*****************************************************************************
void TMR3_IRQHandler(void)
{
    int32_t i;
    
    TIMER3->TISR.TIF = 1;

    uTimer3Tick++;
    //
    // Timer Event Handle
    //
    if (bIsSetTime3Event)                       
    {
        for (i=0; i<TIMER_EVENT_COUNT; i++)
        {
            if (tTime3Event[i].active)
            {
                tTime3Event[i].curTick--;
                if (tTime3Event[i].curTick == 0)
                {
                    (*tTime3Event[i].funPtr)(tTime3Event[i].transParam);
                    tTime3Event[i].curTick = tTime3Event[i].initTick;
                }
            }
        }
    }
}

//*****************************************************************************
//
//! \brief User must to call this function before any timer operations after
//!  system boot up. 
//!
//! \param None 
//!
//! \return None 
//
//*****************************************************************************
void DrvTIMER_Init(void)
{
    bIsTimer0Initial    = TRUE;
    bIsTimer1Initial    = TRUE;
    bIsTimer2Initial    = TRUE;
    bIsTimer3Initial    = TRUE;
}

//*****************************************************************************
//
//! \brief Open the specified timer channel with specified operation mode. 
//!
//! \param ch E_TIMER_CHANNEL, it could be E_TMR0/E_TMR1/E_TMR2/E_TMR3
//!
//! \param uTicksPerSecond This value means how many timer interrupt ticks in
//!  one second
//!
//! \param op_mode E_TIMER_OPMODE, E_ONESHOT_MODE/E_PERIODIC_MODE/E_TOGGLE_MODE
//!
//! \return E_SUCCESS Operation successful
//!         E_DRVTIMER_CHANNEL Invalid Timer channel
//!         E_DRVTIMER_CLOCK_RATE Calculate initial value fail 
//
//*****************************************************************************
int32_t DrvTIMER_Open(E_TIMER_CHANNEL ch, uint32_t uTicksPerSecond, E_TIMER_OPMODE op_mode)
{
    uint32_t i;
    uint32_t uRegTcmpr, uRegTcr = 0x0;

    switch (ch)
    {
        case E_TMR0:
        {
            if ((bIsTimer0Initial != TRUE) || (bIsTimer0Used != FALSE))
                return E_DRVTIMER_EIO;
            
            bIsTimer0Used = TRUE;
           
            SYSCLK->APBCLK.TMR0_EN = 1;
                                                              
            //
            // disable timer
            //
            outpw((uint32_t)&TIMER0->TCSR, 0);                  
           
            //
            // write 1 to clear for safty
            //
            TIMER0->TISR.TIF = 1;                               
                               
            for (i=0; i<TIMER_EVENT_COUNT; i++)
            {
                tTime0Event[i].active = FALSE;
            }

            uTimer0Tick = 0;            
            _sys_uTimer0TickPerSecond = uTicksPerSecond;
                    
            uRegTcmpr = CalTimerInitValue(GetTimerClock(E_TMR0), uTicksPerSecond); 
            if (uRegTcmpr == (uint32_t)-1)      
            {
                return E_DRVTIMER_CLOCK_RATE;           
            }

            TIMER0->TCMPR = (uRegTcmpr << 8) >> 8;
            outpw((uint32_t)&TIMER0->TCSR,  (uRegTcr|(uRegTcmpr>>24))|(op_mode<<27));               
            break;
        }

        case E_TMR1:
        {
            if ((bIsTimer1Initial != TRUE) || (bIsTimer1Used != FALSE))
                return E_DRVTIMER_EIO;

            bIsTimer1Used = TRUE;

            SYSCLK->APBCLK.TMR1_EN = 1;
            
            //
            // disable timer
            //
            outpw((uint32_t)&TIMER1->TCSR, 0);                  
            
            //
            // write 1 to clear for safty
            //
            TIMER1->TISR.TIF = 1;                               

            for (i=0; i<TIMER_EVENT_COUNT; i++)
            {
                tTime1Event[i].active = FALSE;
            }

            uTimer1Tick = 0;
            _sys_uTimer1TickPerSecond = uTicksPerSecond;

            uRegTcmpr = CalTimerInitValue(GetTimerClock(E_TMR1), uTicksPerSecond); 
            if(uRegTcmpr == (uint32_t)-1)       
            {
                return E_DRVTIMER_CLOCK_RATE;           
            }

            TIMER1->TCMPR = (uRegTcmpr << 8) >> 8;
            outpw((uint32_t)&TIMER1->TCSR,  (uRegTcr|(uRegTcmpr>>24))|(op_mode<<27));   
            break;
        }

        case E_TMR2:
        {
            if ((bIsTimer2Initial != TRUE) || (bIsTimer2Used != FALSE))
                return E_DRVTIMER_EIO;    

            bIsTimer2Used = TRUE;

            SYSCLK->APBCLK.TMR2_EN =1;
            
            //
            // disable timer
            //
            outpw((uint32_t)&TIMER2->TCSR, 0);                  

            //
            // write 1 to clear for safty
            //
            TIMER2->TISR.TIF = 1;                               

            for (i=0; i<TIMER_EVENT_COUNT; i++)
            {
                tTime2Event[i].active = FALSE;
            }

            uTimer2Tick = 0;
            _sys_uTimer2TickPerSecond = uTicksPerSecond;

            uRegTcmpr = CalTimerInitValue(GetTimerClock(E_TMR2), uTicksPerSecond); 
            if(uRegTcmpr == (uint32_t)-1)       
            {
                return E_DRVTIMER_CLOCK_RATE;           
            }

            TIMER2->TCMPR = (uRegTcmpr << 8) >> 8;  
            outpw((uint32_t)&TIMER2->TCSR,  (uRegTcr|(uRegTcmpr>>24))|(op_mode<<27));   
            break;
        }

        case E_TMR3:
        {
            if ((bIsTimer3Initial != TRUE) || (bIsTimer3Used != FALSE))
                return E_DRVTIMER_EIO;

            bIsTimer3Used = TRUE;

            SYSCLK->APBCLK.TMR3_EN = 1;

            //
            // disable timer
            //
            outpw((uint32_t)&TIMER3->TCSR, 0);                  

            //
            // write 1 to clear for safty
            //
            TIMER3->TISR.TIF = 1;                               

            for (i=0; i<TIMER_EVENT_COUNT; i++)
            {
                tTime3Event[i].active = FALSE;
            }

            uTimer3Tick = 0;
            _sys_uTimer3TickPerSecond = uTicksPerSecond;
            
            uRegTcmpr = CalTimerInitValue(GetTimerClock(E_TMR3), uTicksPerSecond); 
            if(uRegTcmpr == (uint32_t)-1)       
            {
                return E_DRVTIMER_CLOCK_RATE;           
            }

            TIMER3->TCMPR = (uRegTcmpr << 8) >> 8;
            outpw((uint32_t)&TIMER3->TCSR,  (uRegTcr|(uRegTcmpr>>24))|(op_mode<<27));    
            break;
        }

        default:
        {
            return E_DRVTIMER_CHANNEL ;
        }
    }

   	if (op_mode == E_TOGGLE_MODE)
    {
        switch (ch)
        {
            case E_TMR0:
            case E_TMR1:
            case E_TMR2:
    		case E_TMR3:
            {
		    	DrvGPIO_InitFunction((E_DRVGPIO_FUNC)((uint32_t)E_FUNC_TMR0 + (uint32_t)ch));	
            }
    
            default:
            {
                return E_DRVTIMER_CHANNEL ; 
            }
        }
    }

    return E_SUCCESS;
}

//*****************************************************************************
//
//! \brief The function is used to close the timer channel. 
//!
//! \param ch E_TIMER_CHANNEL, it could be E_TMR0/E_TMR1/E_TMR2/E_TMR3
//!
//! \return E_SUCCESS Operation successful
//!         E_DRVTIMER_CHANNEL Invalid Timer channel 
//
//*****************************************************************************
int32_t DrvTIMER_Close(E_TIMER_CHANNEL ch)
{
    switch (ch)
    {
        case E_TMR0:
        {
            DrvTIMER_DisableInt(E_TMR0);
            TIMER0->TCSR.CRST   = 1;
            bIsTimer0Used       = FALSE;
            bIsSetTime0Event    = FALSE;
            break;
        }

        case E_TMR1:
        {
            DrvTIMER_DisableInt(E_TMR1);
            TIMER1->TCSR.CRST   = 1;
            bIsTimer1Used       = FALSE;
            bIsSetTime1Event    = FALSE;
            break;
        }

        case E_TMR2:
        {
            DrvTIMER_DisableInt(E_TMR2);
            TIMER2->TCSR.CRST   = 1;
            bIsTimer2Used       = FALSE;
            bIsSetTime2Event    = FALSE;
            break;
        }

        case E_TMR3:
        {
            DrvTIMER_DisableInt(E_TMR3);
            TIMER3->TCSR.CRST   = 1;
            bIsTimer3Used       = FALSE;
            bIsSetTime3Event    = FALSE;
            break;
        }

        default:
        {
            return E_DRVTIMER_CHANNEL ;
        }
    }

    return E_SUCCESS;
}

//*****************************************************************************
//
//! \brief Install the interrupt callback function of the specified timer
//!  channel. And trigger timer callback function when interrupt occur
//!  specified times. 
//!
//! \param ch E_TIMER_CHANNEL, it could be E_TMR0/E_TMR1/E_TMR2/E_TMR3
//! \param uInterruptTicks Number of timer interrupt occurred
//! \param pTimerCallback The function pointer of the interrupt callback
//!  function
//! \param parameter A parameter of the callback function 
//!
//! \return uTimerEventNo The timer event number
//!         E_DRVTIMER_EVENT_FULL The timer event is full 
//
//*****************************************************************************
int32_t DrvTIMER_SetTimerEvent(E_TIMER_CHANNEL ch, uint32_t uInterruptTicks, TIMER_CALLBACK pTimerCallback, uint32_t parameter)
{
    volatile int32_t i;
    int32_t uTimerEventNo = 0;

    switch (ch)
    {
        case E_TMR0:
        {
            if (uTime0EventCount >= TIMER_EVENT_COUNT)
                return E_DRVTIMER_EVENT_FULL;

            bIsSetTime0Event = TRUE;
            uTime0EventCount++;
            for (i=0; i<TIMER_EVENT_COUNT; i++)
            {
                if (tTime0Event[i].active  == FALSE)
                {
                    tTime0Event[i].active       = TRUE;
                    tTime0Event[i].initTick     = uInterruptTicks;
                    tTime0Event[i].curTick      = uInterruptTicks;
                    tTime0Event[i].funPtr       = (TIMER_CALLBACK)pTimerCallback;
                    tTime0Event[i].transParam   = parameter;
                    uTimerEventNo = i;
                    break;
                }
            }
            break;
        }

        case E_TMR1:
        {
            if (uTime1EventCount >= TIMER_EVENT_COUNT)
                return E_DRVTIMER_EVENT_FULL;

            bIsSetTime1Event = TRUE;
            uTime1EventCount++;
            for (i=0; i<TIMER_EVENT_COUNT; i++)
            {
                if (tTime1Event[i].active   == FALSE)
                {
                    tTime1Event[i].active       = TRUE;
                    tTime1Event[i].initTick     = uInterruptTicks;
                    tTime1Event[i].curTick      = uInterruptTicks;
                    tTime1Event[i].funPtr       = (TIMER_CALLBACK)pTimerCallback;
                    tTime1Event[i].transParam   = parameter;
                    uTimerEventNo = i;
                    break;
                }
            }
            break;
        }

        case E_TMR2:
        {
            if (uTime2EventCount >= TIMER_EVENT_COUNT)
                return E_DRVTIMER_EVENT_FULL;

            bIsSetTime2Event = TRUE;
            uTime2EventCount++;
            for (i=0; i<TIMER_EVENT_COUNT; i++)
            {
                if (tTime2Event[i].active   == FALSE)
                {
                    tTime2Event[i].active       = TRUE;
                    tTime2Event[i].initTick     = uInterruptTicks;
                    tTime2Event[i].curTick      = uInterruptTicks;
                    tTime2Event[i].funPtr       = (TIMER_CALLBACK)pTimerCallback;
                    tTime2Event[i].transParam   = parameter;
                    uTimerEventNo = i;
                    break;
                }
            }
            break;
        }

        case E_TMR3:
        {
            if (uTime3EventCount >= TIMER_EVENT_COUNT)
                return E_DRVTIMER_EVENT_FULL;

            bIsSetTime3Event = TRUE;
            uTime3EventCount++;
            for (i=0; i<TIMER_EVENT_COUNT; i++)
            {
                if (tTime3Event[i].active   == FALSE)
                {
                    tTime3Event[i].active       = TRUE;
                    tTime3Event[i].initTick     = uInterruptTicks;
                    tTime3Event[i].curTick      = uInterruptTicks;
                    tTime3Event[i].funPtr       = (TIMER_CALLBACK)pTimerCallback;
                    tTime3Event[i].transParam   = parameter;
                    uTimerEventNo = i;
                    break;
                }
            }
            break;
        }

        default:
        {
            break;
        }
    }

    return uTimerEventNo;
}

//*****************************************************************************
//
//! \brief Clear the timer event of the specified timer channel. 
//!
//! \param ch E_TIMER_CHANNEL, it could be E_TMR0/E_TMR1/E_TMR2/E_TMR3
//! \param uTimerEventNo The timer event number
//!
//! \return None 
//
//*****************************************************************************
void DrvTIMER_ClearTimerEvent(E_TIMER_CHANNEL ch, uint32_t uTimerEventNo)
{
    switch (ch)
    {
        case E_TMR0:
        {
            tTime0Event[uTimerEventNo].active = FALSE;
            uTime0EventCount--;
            if (uTime0EventCount == 0)
            {
                bIsSetTime0Event = FALSE;
            }
            break;
        }

        case E_TMR1:
        {
            tTime1Event[uTimerEventNo].active = FALSE;
            uTime1EventCount--;
            if (uTime1EventCount == 0)
            {
                bIsSetTime1Event = FALSE;
            }
            break;
        }

        case E_TMR2:
        {
            tTime2Event[uTimerEventNo].active = FALSE;
            uTime2EventCount--;
            if (uTime2EventCount == 0)
            {
                bIsSetTime2Event = FALSE;
            }
            break;
        }

        case E_TMR3:
        {
            tTime3Event[uTimerEventNo].active = FALSE;
            uTime3EventCount--;
            if (uTime3EventCount == 0)
            {
                bIsSetTime3Event = FALSE;
            }
            break;
        }

        default:
        {
            break;
        }
    }
}

//*****************************************************************************
//
//! \brief This function is used to enable the specified timer interrupt. 
//!
//! \param ch E_TIMER_CHANNEL, it could be E_TMR0/E_TMR1/E_TMR2/E_TMR3
//!
//! \return E_SUCCESS Operation successful
//!         E_DRVTIMER_CHANNEL Invalid Timer channel 
//
//*****************************************************************************
int32_t DrvTIMER_EnableInt(E_TIMER_CHANNEL ch)
{
	TIMER_T * tTMR;

   	switch (ch)
    {
        case E_TMR0:
        case E_TMR1:
        case E_TMR2:
		case E_TMR3:
        {
        	tTMR = (TIMER_T *)((uint32_t)TIMER0 + CH_OFFSET[ch]);         
            tTMR->TCSR.IE = 1;
            NVIC_SetPriority((IRQn_Type)((uint32_t)TMR0_IRQn + (uint32_t)ch), (1<<__NVIC_PRIO_BITS) - 2);
			NVIC_EnableIRQ((IRQn_Type)((uint32_t)TMR0_IRQn + (uint32_t)ch)); 
            return E_SUCCESS ;
        }

        default:
        {
            return E_DRVTIMER_CHANNEL ; 
        }
    }
}

//*****************************************************************************
//
//! \brief This function is used to disable the specified timer interrupt 
//!
//! \param ch E_TIMER_CHANNEL, it could be E_TMR0/E_TMR1/E_TMR2/E_TMR3
//!
//! \return E_SUCCESS Operation successful
//!         E_DRVTIMER_CHANNEL Invalid Timer channel 
//
//*****************************************************************************
int32_t DrvTIMER_DisableInt(E_TIMER_CHANNEL ch)
{
	TIMER_T * tTMR;

   	switch (ch)
    {
        case E_TMR0:
        case E_TMR1:
        case E_TMR2:
		case E_TMR3:
        {
        	tTMR = (TIMER_T *)((uint32_t)TIMER0 + CH_OFFSET[ch]);         
            tTMR->TCSR.IE = 0;
			NVIC_DisableIRQ((IRQn_Type)((uint32_t)TMR0_IRQn + (uint32_t)ch)); 
            return E_SUCCESS ;
        }

        default:
        {
            return E_DRVTIMER_CHANNEL ; 
        }
    }
}

//*****************************************************************************
//
//! \brief Get the interrupt flag status from the specified timer channel. 
//!
//! \param ch E_TIMER_CHANNEL, it could be E_TMR0/E_TMR1/E_TMR2/E_TMR3
//!
//! \return iIntStatus 0:No interrupt / 1:Interrupt
//!         occurred E_DRVTIMER_CHANNEL Invalid Timer channel 
//
//*****************************************************************************
int32_t DrvTIMER_GetIntFlag(E_TIMER_CHANNEL ch)
{
	if (ch == E_TMR0 )
		return TIMER0->TISR.TIF;
	else if(ch == E_TMR1 )									 
		return TIMER1->TISR.TIF;
	else if(ch == E_TMR2 )
		return TIMER2->TISR.TIF;
	else if(ch == E_TMR3 )
		return TIMER3->TISR.TIF;
	else 
    	return E_DRVTIMER_CHANNEL;
}

//*****************************************************************************
//
//! \brief Clear the interrupt flag of the specified timer channel. 
//!
//! \param ch E_TIMER_CHANNEL, it could be E_TMR0/E_TMR1/E_TMR2/E_TMR3
//!
//! \return E_SUCCESS Operation successful
//!         E_DRVTIMER_CHANNEL Invalid Timer channel 
//
//*****************************************************************************
int32_t DrvTIMER_ClearIntFlag(E_TIMER_CHANNEL ch)
{
    if (ch == E_TMR0 )
        TIMER0->TISR.TIF = 1;
    else if(ch == E_TMR1 )                                   
        TIMER1->TISR.TIF = 1;
    else if(ch == E_TMR2 )
        TIMER2->TISR.TIF = 1;
    else if(ch == E_TMR3 )
        TIMER3->TISR.TIF = 1;
    else 
        return E_DRVTIMER_CHANNEL;

    return E_SUCCESS;
}

//*****************************************************************************
//
//! \brief Start to count the specified timer channel. 
//!
//! \param ch E_TIMER_CHANNEL, it could be E_TMR0/E_TMR1/E_TMR2/E_TMR3
//!
//! \return E_SUCCESS Operation successful
//!         E_DRVTIMER_CHANNEL Invalid Timer channel 
//
//*****************************************************************************
int32_t DrvTIMER_Start(E_TIMER_CHANNEL ch)
{
	TIMER_T * tTMR;

   	switch (ch)
    {
        case E_TMR0:
        case E_TMR1:
        case E_TMR2:
		case E_TMR3:
        {
        	tTMR = (TIMER_T *)((uint32_t)TIMER0 + CH_OFFSET[ch]);         
            tTMR->TCSR.CEN = 1;
            return E_SUCCESS ;
        }

        default:
        {
            return E_DRVTIMER_CHANNEL ; 
        }
    }
}

//*****************************************************************************
//
//! \brief This function is used to get the number of interrupt occurred after
//!  the timer interrupt function is enabled .Thus DrvTIMER_EnableInt(ch) must 
//!  been called in advance. 
//!
//! \param ch E_TIMER_CHANNEL, it could be E_TMR0/E_TMR1/E_TMR2/E_TMR3
//!
//! \return uTimerTick Return the interrupt ticks
//!         E_DRVTIMER_CHANNEL Invalid Timer channel 
//
//*****************************************************************************
uint32_t DrvTIMER_GetIntTicks(E_TIMER_CHANNEL ch)
{
    switch (ch)
    {
        case E_TMR0:
        {
            return uTimer0Tick;  
        }

        case E_TMR1:
        {
            return uTimer1Tick;
        }   

        case E_TMR2:
        {
            return uTimer2Tick;
        }

        case E_TMR3:
        {
            return uTimer3Tick;
        }

        default:
        {
            return E_DRVTIMER_CHANNEL;
        }
    }
}

//*****************************************************************************
//
//! \brief This function is used to clear interrupt ticks to 0. 
//!
//! \param ch E_TIMER_CHANNEL, it could be E_TMR0/E_TMR1/E_TMR2/E_TMR3
//!
//! \return E_SUCCESS Operation successful
//!         E_DRVTIMER_CHANNEL Invalid Timer channel 
//
//*****************************************************************************
int32_t DrvTIMER_ResetIntTicks(E_TIMER_CHANNEL ch)
{
    switch (ch)
    {
        case E_TMR0:
        {
            uTimer0Tick = 0;
            break;
        }

        case E_TMR1:
        {
            uTimer1Tick = 0;
            break;
        }

        case E_TMR2:
        {
            uTimer2Tick = 0;
            break;
        }

        case E_TMR3:
        {
            uTimer3Tick = 0;
            break;
        }

        default:
        {
            return E_DRVTIMER_CHANNEL;
        }
    }

    return E_SUCCESS;
}

//*****************************************************************************
//
//! \brief This function is used to add a delay loop by specified interrupt
//!  ticks of the timer channel. 
//!
//! \param ch E_TIMER_CHANNEL, it could be E_TMR0/E_TMR1/E_TMR2/E_TMR3
//!
//! \param uIntTicks The delay ticks
//!
//! \return None 
//
//*****************************************************************************
void DrvTIMER_Delay(E_TIMER_CHANNEL ch, uint32_t uIntTicks)
{
    uint32_t volatile btime;

    btime = DrvTIMER_GetIntTicks(ch);
    
    while (1)
    {
        if ((DrvTIMER_GetIntTicks(ch) - btime) > uIntTicks)
        {
            break;
        }
    }
}

//*****************************************************************************
//
//! \brief Get the version number of Timer/WDT driver. 
//!
//! \param None 
//!
//! \return DRVTIMER_VERSION_NUM Version number of Timer/WDT driver 
//
//*****************************************************************************
uint32_t DrvTIMER_GetVersion(void)
{
    return DRVTIMER_VERSION_NUM;
}

//*****************************************************************************
//
//! \brief The WatchDog Timer(WDT) default IRQ, declared in startup_NUC1xx.s All
//!  bits in WDT register are write-protected. User must to check the REGWRPROT
//!  bit is enabled or disabled if write the specified WDT bit fail. 
//!
//! \param None 
//!
//! \return None 
//
//*****************************************************************************
void WDT_IRQHandler(void)
{
    volatile uint32_t uWakeupFlag;

    //
    // Stored WDT wakeup flag
    //
    uWakeupFlag = WDT->WTCR.WTWKF;  

    //
    // Clear the WDT INT Flag
    //
    WDT->WTCR.WTIF = 1;             

    if (fnCallBack_WDT)        
    {
        fnCallBack_WDT(uWakeupFlag);
    }
}

//*****************************************************************************
//
//! \brief Enable WDT engine clock and set WDT time-out interval. All bits in
//!  WDT register are write-protected. User must to check the REGWRPROT bit is
//!  enabled or disabled if write the specified WDT bit fail. 
//!
//! \param WDTlevel  E_WDT_INTERVAL, enumerate the WDT time-out interval. Refer
//!  to WDT_INTERVAL enumeration for detail time-out value.
//!
//! \return E_SUCCESS Operation successful 
//!         E_DRVWDT_OPEN WDT open fail 
//
//*****************************************************************************
int32_t DrvWDT_Open(E_WDT_INTERVAL WDTlevel)
{
    if ((SYS->REGWRPROT & 0x01) == 0)
    {
        //
        // The protected Registers are locked 
        //
        return E_DRVWDT_OPEN;
    }

    //
    // Enable WatchDog Timer Clock
    //
    SYSCLK->APBCLK.WDT_EN = 1;              
     
    //
    // Stop WDT first
    //
    DrvWDT_Ioctl(E_WDT_IOC_STOP_TIMER, 0);  

    //
    // Select WatchDog Timer Interval
    //
    WDT->WTCR.WTIS = WDTlevel;              

    return E_SUCCESS;
}

//*****************************************************************************
//
//! \brief The function is used to stop/disable WDT relative functions. All bits
//!  in WDT register are write-protected. User must to check the REGWRPROT bit
//!  is enabled or disabled if write the specified WDT bit fail. 
//!
//! \param None 
//!
//! \return None 
//
//*****************************************************************************
void DrvWDT_Close(void)
{
    //
    // Stop WDT
    //
    DrvWDT_Ioctl(E_WDT_IOC_STOP_TIMER, 0); 
   
    //
    // Disable WDT Interrupt
    //
    DrvWDT_Ioctl(E_WDT_IOC_DISABLE_INT, 0); 
    
    //
    // Disable WDT wakeup fuction
    //
    DrvWDT_Ioctl(E_WDT_IOC_DISABLE_WAKEUP, 0);  
    
    //
    // Add delay loop to make sure the internal WDT status is stable then disable
    //  WDT engine clock 
    //
    if (DrvSYS_GetHCLKFreq() != __IRC10K)
    {        
        volatile uint32_t i=0x2000;
        while (i--);
    }

    SYSCLK->APBCLK.WDT_EN = 0;
}

//*****************************************************************************
//
//! \brief The function is used to install WDT interrupt service routine. All
//!  bits in WDT register are write-protected. User must to check the REGWRPROT
//!  bit is enabled or disabled if write the specified WDT bit fail. 
//!
//! \param pvWDTISR  The function pointer of the interrupt service routine
//!
//! \return None 
//
//*****************************************************************************
void DrvWDT_InstallISR(WDT_CALLBACK pvWDTISR)
{
    WDT->WTCR.WTIE = 1;
    fnCallBack_WDT = (WDT_CALLBACK)pvWDTISR;    
}

//*****************************************************************************
//
//! \brief The function is used to operate more WDT applications, it could be
//!  the start/stop the WDT, enable/disable WDT interrupt function,
//!  enable/disable WDT time-out wake up function, enable/disable system reset
//!  when WDT time-out and set the WDT time-out interval. All bits in WDT
//!  register are write-protected. User must to check the REGWRPROT bit is
//!  enabled or disabled if write the specified WDT bit fail. 
//!
//! \param uWDTCmd  E_WDT_CMD commands, it could be the one of the follow
//!  commands E_WDT_IOC_START_TIMER, E_WDT_IOC_STOP_TIMER, E_WDT_IOC_ENABLE_INT,
//!  E_WDT_IOC_DISABLE_INT, E_WDT_IOC_ENABLE_WAKEUP, E_WDT_IOC_DISABLE_WAKEUP,
//!  E_WDT_IOC_RESET_TIMER, E_WDT_IOC_ENABLE_RESET_FUNC,
//!  E_WDT_IOC_DISABLE_RESET_FUNC, E_WDT_IOC_SET_INTERVAL
//!
//! \param uArgument  Set the argument for the specified WDT command
//!
//! \return E_SUCCESS Operation successful
//!         E_DRVWDT_CMD Invalid WDT command 
//
//*****************************************************************************
int32_t DrvWDT_Ioctl(E_WDT_CMD uWDTCmd, uint32_t uArgument)
{
    switch (uWDTCmd)
    {
        case E_WDT_IOC_START_TIMER  :
        {
            WDT->WTCR.WTE = 1;
            break ;
        }

        case E_WDT_IOC_STOP_TIMER :
        {
            WDT->WTCR.WTE = 0;
            break ;
        }

        case E_WDT_IOC_ENABLE_INT :
        {
            WDT->WTCR.WTIE = 1;
            NVIC_SetPriority(WDT_IRQn, (1<<__NVIC_PRIO_BITS) - 2);
            NVIC_EnableIRQ(WDT_IRQn); 
            break ;
        }

        case E_WDT_IOC_DISABLE_INT :
        {
            WDT->WTCR.WTIE = 0;
            NVIC_DisableIRQ(WDT_IRQn); 
            break ;
        }

        case E_WDT_IOC_ENABLE_WAKEUP :
        {
            WDT->WTCR.WTWKE = 1;            
            break ;
        }

        case E_WDT_IOC_DISABLE_WAKEUP :
        {
            WDT->WTCR.WTWKE = 0;            
            break ;
        }

        case E_WDT_IOC_RESET_TIMER:
        {
            WDT->WTCR.WTR = 1;
            break ;
        }

        case E_WDT_IOC_ENABLE_RESET_FUNC :
        {
            WDT->WTCR.WTRE = 1;
            break ;
        }

        case E_WDT_IOC_DISABLE_RESET_FUNC:
        {
            WDT->WTCR.WTRE = 0;
            break ;
        }

        case E_WDT_IOC_SET_INTERVAL:
        {
            WDT->WTCR.WTIS = (E_WDT_INTERVAL)uArgument;

            break ;
        }
       
        default :
        {
            return E_DRVWDT_CMD;
        }
    }

    return E_SUCCESS;
}

