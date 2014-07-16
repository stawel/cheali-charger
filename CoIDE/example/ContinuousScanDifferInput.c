#include <stdio.h>
#include "M051Series.h"
#include "DrvADC.h"
//#include "DrvUART.h"
#include "DrvGPIO.h"
#include "DrvSYS.h"

/**************************************************************************//**
 * @brief      ADC continuous scan mode with differential input pair 0,1,2,3.
 * @param      None
 * @return     None
******************************************************************************/
void ContinuousScanDifferConvert(void)
{   
/*    STR_UART_T  param;
    uint32_t    DelayCounter;
    uint32_t    ConvertCounter=10;  /*Close the ADC after scanning 10 times*/

    /* Unlock the locked registers before access */
    UNLOCKREG(x);
    /* Enable the 12MHz oscillator oscillation */
    DrvSYS_SetOscCtrl(E_SYS_XTL12M, 1);
    /* Waiting for 12M Xtal stable */
    while (DrvSYS_GetChipClockSourceStatus(E_SYS_XTL12M)  != 1);

    /* HCLK clock source. 0: external 12MHz; 4:internal 22MHz RC oscillator */
/*    DrvSYS_SelectHCLKSource(0);
    /* HCLK clock frequency = HCLK clock source / (HCLK_N + 1) */
    /*    DrvSYS_SetClockDivider(E_SYS_HCLK_DIV, 0);

    DrvGPIO_InitFunction(E_FUNC_UART0);
    /* Select UART Clock Source From 12MHz */
    /*    DrvSYS_SelectIPClockSource(E_SYS_UART_CLKSRC,0);

    param.u32BaudRate        = 115200;
    param.u8cDataBits        = DRVUART_DATABITS_8;
    param.u8cStopBits        = DRVUART_STOPBITS_1;
    param.u8cParity          = DRVUART_PARITY_NONE;
    param.u8cRxTriggerLevel  = DRVUART_FIFO_1BYTES;
    param.u8TimeOut          = 0;
    DrvUART_Open(UART_PORT0, &param);

    printf("\n\r");
    printf("Driver version: 0x%X\n\r", DrvADC_GetVersion());

    DrvADC_Open(ADC_DIFFERENTIAL, ADC_CONTINUOUS_OP, 0, EXTERNAL_12MHZ, 3);
    printf("Conversion rate: %u samples/second\n\r", DrvADC_GetConversionRate());
     /*Enable differential input pair 0,1,2,3*/
    /*    DrvADC_SetADCChannel(0x02|0x08|0x20|0x80);
    DrvADC_StartConvert();

    printf("Conversion result\n\r" );
    while (ConvertCounter--)
    {
        while (DrvADC_IsConversionDone() == FALSE);
        printf("pair0:0x%X;  pair1:0x%X;  pair2:0x%X;  pair3:0x%X;\n\r", \
                (uint16_t)DrvADC_GetConversionData(1), (uint16_t)DrvADC_GetConversionData(3), \
                (uint16_t)DrvADC_GetConversionData(5), (uint16_t)DrvADC_GetConversionData(7));
        DelayCounter = 1000000;
        while (DelayCounter --);
    }

    DrvADC_Close();
    */
}
