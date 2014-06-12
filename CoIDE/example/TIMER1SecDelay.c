#include <stdio.h>
#include "DrvTIMER.h"
//#include "DrvUART.h"
#include "DrvGPIO.h"
#include "DrvSYS.h"

/*************************************************************************//**
 * @brief   Delay 1 second exactly with timer.
 * @param   None
 * @return  None
*****************************************************************************/
void Delay1Sec(void)
{
    UNLOCKREG();
    SYSCLK->PWRCON.XTL12M_EN = 1;
    /* Waiting for 12M Xtal stable */
    while (DrvSYS_GetChipClockSourceStatus(E_SYS_XTL12M) != 1);
    
    /*Configure gpio p3.6 as OUTPUT for led*/
    DrvGPIO_Open(E_PORT3, E_PIN6, E_IO_OUTPUT);
    /* Example code */
    DrvTIMER_Init();
    DrvTIMER_Open(E_TMR0, 1000, E_PERIODIC_MODE);                   
    DrvTIMER_EnableInt(E_TMR0);     
    DrvTIMER_Start(E_TMR0);             /* Start counting */

    while (1)
    {
        DrvGPIO_SetBit(E_PORT3, E_PIN6);
        DrvTIMER_Delay(E_TMR0, 1000);       /*Delay 1000/1000 Sec*/
        DrvGPIO_ClrBit(E_PORT3, E_PIN6);
        DrvTIMER_Delay(E_TMR0, 1000);
    }
}
