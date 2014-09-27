#include <DrvGPIO.h>
#include <DrvFMC.h>
//#include <DrvUART.h>
#include <DrvSYS.h>

#define LDROM_BASE      0x00100000
#define PAGE_SIZE       512
/******************************************************************************//**
 *@brief  A simple FMC example. It Erase the LDROM and Verify ,then Program LDROM
 *        and read out data to compare.
 *@param  None
 *@return None 
**********************************************************************************/
void FMC_LDROM(void)
{
	uint32_t u32Data, i;
	int32_t  i32Err;
//	STR_UART_T sParam;

	/* Unlock the protected registers */
	UNLOCKREG();

	/* Enable the 12MHz oscillator oscillation */
	SYSCLK->PWRCON.XTL12M_EN = 1;

	/* Waiting for 12M Xtal stable */
	while (DrvSYS_GetChipClockSourceStatus(E_SYS_XTL12M) != 1);

	/* Set UART Pin */
	DrvGPIO_InitFunction(E_FUNC_UART0);

	/* UART Setting */
/*	sParam.u32BaudRate 		= 115200;
	sParam.u8cDataBits 		= DRVUART_DATABITS_8;
	sParam.u8cStopBits 		= DRVUART_STOPBITS_1;
	sParam.u8cParity 		= DRVUART_PARITY_NONE;
	sParam.u8cRxTriggerLevel= DRVUART_FIFO_1BYTES;
*/
	/* Select UART Clock Source From 12MHz */
	DrvSYS_SelectIPClockSource(E_SYS_UART_CLKSRC, 0);

	/* Set UART Configuration */
//	DrvUART_Open(UART_PORT0, &sParam);

	/* Enable ISP function */
	DrvFMC_EnableISP();
	/* Enable LDROM update */
	DrvFMC_EnableLDUpdate();

	/* Page Erase LDROM */
	printf("Erase LDROM test...............................");
	for(i=0; i<4096; i+=PAGE_SIZE)
		DrvFMC_Erase(LDROM_BASE + i);
	/* Erase Verify */
	i32Err = 0;
	for(i = LDROM_BASE; i < (LDROM_BASE+4096); i += 4)
	{
		DrvFMC_Read(i, &u32Data);

		if(u32Data != 0xFFFFFFFF)
		{
			i32Err = 1;
		}
	}
    if (i32Err)
        printf("[FAIL]\n\r");
    else
        printf("[OK]\n\r");

	/* Program LD ROM and read out data to compare */
	printf("Program LDROM test.............................");
	for(i = LDROM_BASE; i < (LDROM_BASE+4096); i += 4)
	{
		DrvFMC_Write(i, i);
	}
	i32Err = 0;
	for(i = LDROM_BASE; i < (LDROM_BASE+4096); i += 4)
	{
		DrvFMC_Read(i, &u32Data);
		if(u32Data != i)
		{
			i32Err = 1;
		}
	}
    if (i32Err)
        printf("[FAIL]\n\r");
    else
        printf("[OK]\n\r");

	/* Disable ISP function */
	DrvFMC_DisableISP();

	/* Lock protected registers */
	LOCKREG();

	printf("FMC Sample Code Completed!\n\r");
}
