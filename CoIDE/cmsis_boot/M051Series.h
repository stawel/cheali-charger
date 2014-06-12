//*****************************************************************************
//
//! \file M051Series.h
//! \brief CMSIS Cortex-M0 Core Peripheral Access Layer Header File for Nuvoton M051 Series
//!          Device.
//! \version v1.02.002
//! \date 21/09/2011
//! \author Nuvoton
//! \copy
//!
//! Copyright (c) Nuvoton Technology Corp. All rights reserved.
//
//*****************************************************************************
#ifndef __M051Series_H__
#define __M051Series_H__


//*****************************************************************************
//
//! Interrupt Number Definition
//
//*****************************************************************************
 
typedef enum IRQn
{
  //***************************************************************************
  // 
  //! Cortex-M0 Processor Exceptions Numbers 
  //
  //***************************************************************************

  //
  //! 2 Non Maskable Interrupt 
  //
  NonMaskableInt_IRQn         = -14,  

  //
  //! 3 Cortex-M0 Hard Fault Interrupt 
  //
  HardFault_IRQn		      = -13,   

  //
  //! 11 Cortex-M0 SV Call Interrupt
  //
  SVCall_IRQn                 = -5,     

  //
  //! 14 Cortex-M0 Pend SV Interrupt
  //
  PendSV_IRQn                 = -2,    

  //
  //! 15 Cortex-M0 System Tick Interrupt 
  //
  SysTick_IRQn                = -1,  

  //***************************************************************************
  // 
  //! ARMIKMCU Swift specific Interrupt Numbers 
  //
  //***************************************************************************
  BOD_IRQn                  = 0,
  WDT_IRQn                  = 1,
  EINT0_IRQn                = 2,
  EINT1_IRQn                = 3,
  GPIO_P0P1_IRQn            = 4,
  GPIO_P2P3P4_IRQn          = 5,
  PWMA_IRQn                 = 6,
  PWMB_IRQn                 = 7,
  TMR0_IRQn                 = 8,
  TMR1_IRQn                 = 9,
  TMR2_IRQn                 = 10,
  TMR3_IRQn                 = 11,
  UART0_IRQn                = 12,
  UART1_IRQn                = 13,
  SPI0_IRQn                 = 14,
  SPI1_IRQn                 = 15,
  I2C0_IRQn                 = 18,
  I2C1_IRQn                 = 19,
  CAN0_IRQn                 = 20,
  CAN1_IRQn                 = 21,
  SD_IRQn                   = 22,
  USBD_IRQn                 = 23,
  PS2_IRQn                  = 24,
  ACMP_IRQn                 = 25,
  PDMA_IRQn                 = 26,
  I2S_IRQn                  = 27,
  PWRWU_IRQn                = 28,
  ADC_IRQn                  = 29,
  DAC_IRQn                  = 30,
  RTC_IRQn                  = 31

  //***************************************************************************
  // 
  //! maximum of 32 Interrupts are possible 
  //
  //***************************************************************************  
} IRQn_Type;

//*****************************************************************************
// 
//! Processor and Core Peripheral Section
//
//*****************************************************************************  

//*****************************************************************************
// 
//! Configuration of the Cortex-M0 Processor and Core Peripherals 
//
//*****************************************************************************  

//
//! armikcmu does not provide a MPU present or not
//
#define __MPU_PRESENT           0     

//
//! armikcmu Supports 2 Bits for the Priority Levels 
//
#define __NVIC_PRIO_BITS        2      

//
//! Set to 1 if different SysTick Config is used 
//
#define __Vendor_SysTickConfig  0    

//
//! Cortex-M0 processor and core peripherals  
//
#include "core_cm0.h"               

//
//! M051Series System 
//
#include "system_M051Series.h"   
#include "System\SysInfra.h"    


#if defined ( __CC_ARM   )
#pragma anon_unions
#endif

//*****************************************************************************
//
//! Initialize the system clock
//!
//! \param  none
//! \return none
//!
//! \brief  Setup the microcontroller system
//!          Initialize the PLL and update the SystemFrequency variable
//
//*****************************************************************************
extern void SystemInit (void);				  


//*****************************************************************************
//
//! Device Specific Peripheral registers structures      
//
//*****************************************************************************

//*****************************************************************************
//
//! General Purpose Input and Ouptut    
//
//***************************************************************************** 

//
//! Statr of previous definitions, for backward compatible 
//
typedef struct
{
    __IO uint32_t 	PMD0:2;
    __IO uint32_t 	PMD1:2;
    __IO uint32_t 	PMD2:2;
    __IO uint32_t 	PMD3:2;
    __IO uint32_t 	PMD4:2;
    __IO uint32_t 	PMD5:2;
    __IO uint32_t 	PMD6:2;
    __IO uint32_t 	PMD7:2;
    __I  uint32_t  	RESERVE:16;    
} GPIO_PMD_T;

typedef __IO uint32_t GPIO_OFFD_T;

typedef __IO uint32_t GPIO_DOUT_T;

typedef __IO uint32_t GPIO_DMASK_T;

typedef __IO uint32_t GPIO_PIN_T;

typedef __IO uint32_t GPIO_DBEN_T;

typedef __IO uint32_t GPIO_IMD_T;

typedef __IO uint32_t GPIO_IEN_T;

typedef __IO uint32_t GPIO_ISRC_T;

typedef struct
{
    union {
        __IO uint32_t u32PMD;
        struct {
            __IO uint32_t 	PMD0:2;
            __IO uint32_t 	PMD1:2;
            __IO uint32_t 	PMD2:2;
            __IO uint32_t 	PMD3:2;
            __IO uint32_t 	PMD4:2;
            __IO uint32_t 	PMD5:2;
            __IO uint32_t 	PMD6:2;
            __IO uint32_t 	PMD7:2;
            __I  uint32_t  	RESERVE:16;    
        } PMD;
    };

    union {
        __IO uint32_t u32OFFD;
        __IO uint32_t OFFD;
    };

    union {
        __IO uint32_t u32DOUT;
        __IO uint32_t DOUT;
    };

    union {
        __IO uint32_t u32DMASK;
        __IO uint32_t DMASK;
    };

    union {
        __IO uint32_t u32PIN;
        __IO uint32_t PIN;
    };

    union {
        __IO uint32_t u32DBEN;
        __IO uint32_t DBEN;
    };

    union {
        __IO uint32_t u32IMD;
        __IO uint32_t IMD;
    };

    union {
        __IO uint32_t u32IEN;
        __IO uint32_t IEN;
    };

    union {
        __IO uint32_t u32ISRC;
        __IO uint32_t ISRC;
    };
} GPIO_T;

typedef struct
{
    union {
        __IO uint32_t u32DBNCECON;
        struct {
            __IO uint32_t   DBCLKSEL:4;
            __IO uint32_t   DBCLKSRC:1;
            __IO uint32_t   ICLK_ON:1;
            __I  uint32_t   RESERVE:26;    
        } DBNCECON;
    };
} GPIO_DBNCECON_T;

//
//! New definitions of GPIO PORT
//
typedef struct
{
    union {
        __IO uint32_t u32PMD;
        struct {
            __IO uint32_t 	PMD0:2;
            __IO uint32_t 	PMD1:2;
            __IO uint32_t 	PMD2:2;
            __IO uint32_t 	PMD3:2;
            __IO uint32_t 	PMD4:2;
            __IO uint32_t 	PMD5:2;
            __IO uint32_t 	PMD6:2;
            __IO uint32_t 	PMD7:2;
            __I  uint32_t  	RESERVE:16;    
        } PMD;
    };

    union {
        __IO uint32_t u32OFFD;
        __IO uint32_t OFFD;
    };

    union {
        __IO uint32_t u32DOUT;
        __IO uint32_t DOUT;
    };

    union {
        __IO uint32_t u32DMASK;
        __IO uint32_t DMASK;
    };

    union {
        __IO uint32_t u32PIN;
        __IO uint32_t PIN;
    };

    union {
        __IO uint32_t u32DBEN;
        __IO uint32_t DBEN;
    };

    union {
        __IO uint32_t u32IMD;
        __IO uint32_t IMD;
    };

    union {
        __IO uint32_t u32IEN;
        __IO uint32_t IEN;
    };

    union {
        __IO uint32_t u32ISRC;
        __IO uint32_t ISRC;
    };
} PORT_T;

typedef struct
{
    union {
        __IO uint32_t u32DBNCECON;
        struct {
            __IO uint32_t   DBCLKSEL:4;
            __IO uint32_t   DBCLKSRC:1;
            __IO uint32_t   ICLK_ON:1;
            __I  uint32_t   RESERVE:26;    
        } DBNCECON;
    };
} PORT_DBNCECON_T;

//
//! PORT PMD Bit Field Definitions 
//
#define PORT_PMD_PMD7_Pos           14
#define PORT_PMD_PMD7_Msk           (0x3ul << PORT_PMD_PMD7_Pos)

#define PORT_PMD_PMD6_Pos           12
#define PORT_PMD_PMD6_Msk           (0x3ul << PORT_PMD_PMD6_Pos)

#define PORT_PMD_PMD5_Pos           10
#define PORT_PMD_PMD5_Msk           (0x3ul << PORT_PMD_PMD5_Pos)

#define PORT_PMD_PMD4_Pos           8
#define PORT_PMD_PMD4_Msk           (0x3ul << PORT_PMD_PMD4_Pos)

#define PORT_PMD_PMD3_Pos           6
#define PORT_PMD_PMD3_Msk           (0x3ul << PORT_PMD_PMD3_Pos)

#define PORT_PMD_PMD2_Pos           4
#define PORT_PMD_PMD2_Msk           (0x3ul << PORT_PMD_PMD2_Pos)

#define PORT_PMD_PMD1_Pos           2
#define PORT_PMD_PMD1_Msk           (0x3ul << PORT_PMD_PMD1_Pos)

#define PORT_PMD_PMD0_Pos           0
#define PORT_PMD_PMD0_Msk           (0x3ul << PORT_PMD_PMD0_Pos)

//
//! PORT OFFD Bit Field Definitions 
//
#define PORT_OFFD_Pos               16
#define PORT_OFFD_Msk               (0xFFul << PORT_OFFD_Pos)

//
//! PORT DOUT Bit Field Definitions
//
#define PORT_DOUT_Pos               0
#define PORT_DOUT_Msk               (0xFFul << PORT_DOUT_Pos)

//
//! PORT DMASK Bit Field Definitions
//
#define PORT_DMASK_Pos              0
#define PORT_DMASK_Msk              (0xFFul << PORT_DMASK_Pos)

//
//! PORT PIN Bit Field Definitions
//
#define PORT_PIN_Pos                0
#define PORT_PIN_Msk                (0xFFul << PORT_PIN_Pos)

//
//! PORT DBEN Bit Field Definitions
//
#define PORT_DBEN_Pos               0
#define PORT_DBEN_Msk               (0xFFul << PORT_DBEN_Pos)

//
//! PORT IMD Bit Field Definitions
//
#define PORT_IMD_Pos                0
#define PORT_IMD_Msk                (0xFFul << PORT_IMD_Pos)

//
//! PORT IEN Bit Field Definitions
//
#define PORT_IEN_IR_EN_Pos          16
#define PORT_IEN_IR_EN_Msk          (0xFFul << PORT_IEN_IR_EN_Pos)

#define PORT_IEN_IF_EN_Pos          0
#define PORT_IEN_IF_EN_Msk          (0xFFul << PORT_IEN_IF_EN_Pos)

//
//! PORT ISRC Bit Field Definitions
//
#define PORT_ISRC_Pos               0
#define PORT_ISRC_Msk               (0xFFul << PORT_ISRC_Pos)

//
//! PORT DBNCECON Bit Field Definitions
//
#define PORT_DBNCECON_ICLK_ON_Pos   5
#define PORT_DBNCECON_ICLK_ON_Msk   (1ul << PORT_DBNCECON_ICLK_ON_Pos)

#define PORT_DBNCECON_DBCLKSRC_Pos  4
#define PORT_DBNCECON_DBCLKSRC_Msk  (1ul << PORT_DBNCECON_DBCLKSRC_Pos)

#define PORT_DBNCECON_DBCLKSEL_Pos  0
#define PORT_DBNCECON_DBCLKSEL_Msk  (0xFul << PORT_DBNCECON_DBCLKSEL_Pos)

//
//! GPIO Port I/O Bit Output/Input Control Bit Field Definitions
//
#define PORT_Px_DOUT_Pos            0
#define PORT_Px_DOUT_Msk            (1ul << PORT_Px_DOUT_Pos)


//*****************************************************************************
//
//! Device Specific Peripheral registers structures      
//
//*****************************************************************************

typedef __IO uint32_t UART_DATA_T;


typedef struct
{
    __IO uint32_t  RDA_IEN:1;
    __IO uint32_t  THRE_IEN:1;
    __IO uint32_t  RLS_IEN:1;
    __IO uint32_t  MODEM_IEN:1;
    __IO uint32_t  RTO_IEN:1; 
    __IO uint32_t  BUF_ERR_IEN:1;
    __IO uint32_t  WAKE_EN:1;
    __I  uint32_t  RESERVE1:4;

	//
	// Time-out counter enable
	//
    __IO uint32_t  TIME_OUT_EN:1;    
    __IO uint32_t  AUTO_RTS_EN:1;
    __IO uint32_t  AUTO_CTS_EN:1;
    __I  uint32_t  RESERVE2:18;    
    
} UART_IER_T;

typedef struct
{
    __I  uint32_t  RESERVE0:1;
    __IO uint32_t  RFR:1;
    __IO uint32_t  TFR:1;
    __I  uint32_t  RESERVE1:1;

	//
	// Rx FIFO Interrupt Trigger Level
	//
    __IO uint32_t  RFITL:4;         
    __IO uint32_t  RX_DIS:1;
    __I  uint32_t  RESERVE2:7;
    __IO uint32_t  RTS_TRI_LEV:4;
    __I  uint32_t  RESERVE3:12;
} UART_FCR_T;

typedef struct
{
    //
    // Word length select
    //
    __IO uint32_t  WLS:2;              

	//
	// Number of STOP bit
	//
    __IO uint32_t  NSB:1;          

	//
	// Parity bit enable 
	//
    __IO uint32_t  PBE:1;  

	//
	// Even parity enable
	//
    __IO uint32_t  EPE:1;         

	//
	// Stick parity enable
	//
    __IO uint32_t  SPE:1;           

	//
	// Break control bit 
	//
    __IO uint32_t  BCB:1;       
    __I  uint32_t  RESERVE:25;
} UART_LCR_T;

typedef struct
{
    __I  uint32_t  RESERVE0:1;
    __IO uint32_t  RTS:1;
    __I  uint32_t  RESERVE1:3;
    __I  uint32_t  RESERVE2:4;
    __IO uint32_t  LEV_RTS:1;
    __I  uint32_t  RESERVE3:3;

	//
	// RTS status 
	//
    __I  uint32_t  RTS_ST:1;       
    __I  uint32_t  RESERVE4:18;
} UART_MCR_T;

typedef struct
{
    __IO uint32_t  DCTSF:1;
    __I  uint32_t  RESERVE0:3;

	//
	// CTS status
	//
    __I  uint32_t  CTS_ST:1;  
    __I  uint32_t  RESERVE1:3;
    __IO uint32_t  LEV_CTS:1;
    __I  uint32_t  RESERVE2:23;
} UART_MSR_T;

typedef struct
{
    __IO uint32_t  RX_OVER_IF:1;
    __I  uint32_t  RESERVE0:2;
    __IO uint32_t  RS485_ADD_DETF:1;
    __IO uint32_t  PEF:1;
    __IO uint32_t  FEF:1;
    __IO uint32_t  BIF:1;
    __I  uint32_t  RESERVE1:1;
    __I  uint32_t  RX_POINTER:6;
    __I  uint32_t  RX_EMPTY:1;
    __I  uint32_t  RX_FULL:1;
    __I  uint32_t  TX_POINTER:6;
    __I  uint32_t  TX_EMPTY:1;
    __I  uint32_t  TX_FULL:1;
    __IO uint32_t  TX_OVER_IF:1;
    __I  uint32_t  RESERVE2:3;

	//
	// Transmitter empty flag
	//
    __I  uint32_t  TE_FLAG:1;            
    __I  uint32_t  RESERVE3:3;
} UART_FSR_T;



 typedef struct
{
    __IO uint32_t  RDA_IF:1;
    __IO uint32_t  THRE_IF:1;
    __IO uint32_t  RLS_IF:1;
    __IO uint32_t  MODEM_IF:1;
    __IO uint32_t  TOUT_IF:1;
    __IO uint32_t  BUF_ERR_IF:1;
    __I  uint32_t  RESERVE0:2;

    __IO uint32_t  RDA_INT:1;
    __IO uint32_t  THRE_INT:1;
    __IO uint32_t  RLS_INT:1;
    __IO uint32_t  MODEM_INT:1;
    __IO uint32_t  TOUT_INT:1;
    __IO uint32_t  BUF_ERR_INT:1;
    __I  uint32_t  RESERVE1:18;
} UART_ISR_T;


typedef struct
{
    __IO uint32_t  TOIC:8;
    __IO uint32_t  DLY:8;
    __I  uint32_t  RESERVE1:16;

}UART_TOR_T;


typedef struct
{
    __IO uint32_t  BRD:16;
    __I  uint32_t  RESERVE0:8;
    __IO uint32_t  DIVIDER_X:4;
    __IO uint32_t  DIV_X_ONE:1;            
    __IO uint32_t  DIV_X_EN:1;            
    __I  uint32_t  RESERVE1:2;
} UART_BAUD_T;

typedef struct
{
    __I  uint32_t  RESERVE0:1;
    __IO uint32_t  TX_SELECT:1;
    __I  uint32_t  RESERVE1:3;         
    __IO uint32_t  INV_TX:1;            
    __IO uint32_t  INV_RX:1;
    __I  uint32_t  RESERVE2:25;
} UART_IRCR_T;


typedef struct
{
    __I  uint32_t  RESERVE0:8;
    __IO uint32_t  RS485_NMM:1;
	__IO uint32_t  RS485_AAD:1;
	__IO uint32_t  RS485_AUD:1;
	__I  uint32_t  RESERVE1:4;
	__IO uint32_t  RS485_ADD_EN:1;
	__I  uint32_t  RESERVE2:8;
	__IO uint32_t  ADDR_MATCH:8;
} UART_ALTCON_T;

typedef struct
{
    __IO uint32_t  FUN_SEL:2;
    __I  uint32_t  RESERVE0:30;

} UART_FUNSEL_T;

typedef struct
{
    
    union {
        __IO uint32_t u32DATA;
        __IO uint32_t DATA;
    };
    union {
        __IO uint32_t u32IER;
        struct {
            __IO uint32_t  RDA_IEN:1;
            __IO uint32_t  THRE_IEN:1;
            __IO uint32_t  RLS_IEN:1;
            __IO uint32_t  MODEM_IEN:1;
            __IO uint32_t  RTO_IEN:1; 
            __IO uint32_t  BUF_ERR_IEN:1; 
            __IO uint32_t  WAKE_EN:1;
            __I  uint32_t  RESERVE1:4; 
            __IO uint32_t  TIME_OUT_EN:1;
            __IO uint32_t  AUTO_RTS_EN:1;
            __IO uint32_t  AUTO_CTS_EN:1;
            __I  uint32_t  RESERVE2:18;
        } IER;
    };

    union {
        __IO uint32_t u32FCR;
        struct {
            __I  uint32_t  RESERVE0:1;
            __IO uint32_t  RFR:1;
            __IO uint32_t  TFR:1;
            __IO uint32_t  RESERVE1:1;
            __IO uint32_t  RFITL:4;          
            __IO uint32_t  RX_DIS:1; 
            __I  uint32_t  RESERVE2 :7;
            __IO uint32_t  RTS_TRI_LEV:4;
            __I  uint32_t  RESERVE3 :4;
        } FCR;
    };

    union {
        __IO uint32_t u32LCR;
        struct {
            __IO uint32_t  WLS:2;
            __IO uint32_t  NSB:1;
            __IO uint32_t  PBE:1;
            __IO uint32_t  EPE:1;
            __IO uint32_t  SPE:1;          
            __IO uint32_t  BCB:1; 
            __I  uint32_t  RESERVE :25;
        } LCR;
    };

    union {
        __IO uint32_t u32MCR;
        struct {
            __I  uint32_t  RESERVE0:1;
            __IO uint32_t  RTS:1;
            __I  uint32_t  RESERVE1:7;
            __IO uint32_t  LEV_RTS:1;
            __I  uint32_t  RESERVE2:3;          
            __IO uint32_t  RTS_ST:1; 
            __I  uint32_t  RESERVE3:18;
        } MCR;
    };


    union {
        __IO uint32_t u32MSR;
        struct {
            __IO uint32_t  DCTSF:1;
            __I  uint32_t  RESERVE0:3;
            __IO uint32_t  CTS_ST:1;
            __I  uint32_t  RESERVE1:3;
            __IO uint32_t  LEV_CTS:1;          
            __I  uint32_t  RESERVE2:23;
        } MSR;
    };


    union {
        __IO uint32_t u32FSR;
        struct {
            __IO uint32_t  RX_OVER_IF:1;
            __I  uint32_t  RESERVE0:2;
            __IO uint32_t  RS485_ADD_DETF:1;
            __IO uint32_t  PEF:1;
            __IO uint32_t  FEF:1;
            __IO uint32_t  BIF:1;
            __I  uint32_t  RESERVE1:1;
            __IO uint32_t  RX_POINTER:6;
            __IO uint32_t  RX_EMPTY:1;
            __IO uint32_t  RX_FULL:1;
            __IO uint32_t  TX_POINTER:6;
            __IO uint32_t  TX_EMPTY:1;
            __IO uint32_t  TX_FULL:1;
            __IO uint32_t  TX_OVER_IF:1;
            __I  uint32_t  RESERVE2:3;
            __IO uint32_t  TE_FLAG:1;
            __I  uint32_t  RESERVE3:3;
        } FSR;
    };

    union {
        __IO uint32_t u32ISR;
        struct {
            __IO uint32_t  RDA_IF:1;
            __IO uint32_t  THRE_IF:1;
            __IO uint32_t  RLS_IF:1;
            __IO uint32_t  MODEM_IF:1;
            __IO uint32_t  TOUT_IF:1;
            __IO uint32_t  BUF_ERR_IF:1;
            __I  uint32_t  RESERVE0:2;
            __IO uint32_t  RDA_INT:1;
            __IO uint32_t  THRE_INT:1;
            __IO uint32_t  RLS_INT:1;
            __IO uint32_t  MODEM_INT:1;
            __IO uint32_t  TOUT_INT:1;
            __IO uint32_t  BUF_ERR_INT:1;
            __I  uint32_t  RESERVE1:18;

        } ISR;
    };

    union {
        __IO uint32_t u32TOR;
        struct {
            __IO uint32_t  TOIC:8;
            __IO uint32_t  DLY:8;
            __I  uint32_t  RESERVE1:16;
        } TOR;
    };

    union {
        __IO uint32_t u32BAUD;
        struct {
            __IO uint32_t  BRD:16;
            __I  uint32_t  RESERVE0:8;
            __IO uint32_t  DIVIDER_X:4;
            __IO uint32_t  DIV_X_ONE:1;
            __IO uint32_t  DIV_X_EN:1;
            __I  uint32_t  RESERVE1:2;
        } BAUD;
    };

    union {
        __IO uint32_t u32IRCR;
        struct {
            __I  uint32_t  RESERVE0:1;
            __IO uint32_t  TX_SELECT:1;
            __I  uint32_t  RESERVE1:3;
            __IO uint32_t  INV_TX:1;
            __IO uint32_t  INV_RX:1;
            __I  uint32_t  RESERVE2:25;
        } IRCR;
    };

    union {
        __IO uint32_t u32ALTCON;
        struct {
            __I  uint32_t  RESERVE0:8;
            __IO uint32_t  RS485_NMM:1;
            __IO uint32_t  RS485_AAD:1;
            __IO uint32_t  RS485_AUD:1;
            __I  uint32_t  RESERVE1:4;
            __IO uint32_t  RS485_ADD_EN :1;
            __I  uint32_t  RESERVE2 :8;
            __IO uint32_t  ADDR_MATCH :8;
        } ALTCON;
    };


    union {
        __IO uint32_t u32FUNSEL;
        struct {
            __IO uint32_t  FUN_SEL:2;
            __I  uint32_t  RESERVE0:30;
        } FUNSEL;
    };
} UART_T;



//
//! UART THR Bit Field Definitions
//
#define UART_THR_THR_Pos         0
#define UART_THR_THR_Msk        (0xFul << UART_THR_THR_Pos)

//
//! UART RBR Bit Field Definitions 
//
#define UART_RBR_RBR_Pos         0
#define UART_RBR_RBR_Msk        (0xFul << UART_RBR_RBR_Pos)

//
//! UART IER Bit Field Definitions
//
#define UART_IER_AUTO_CTS_EN_Pos    13
#define UART_IER_AUTO_CTS_EN_Msk    (1ul << UART_IER_AUTO_CTS_EN_Pos)

#define UART_IER_AUTO_RTS_EN_Pos    12
#define UART_IER_AUTO_RTS_EN_Msk    (1ul << UART_IER_AUTO_RTS_EN_Pos)

#define UART_IER_TIME_OUT_EN_Pos    11
#define UART_IER_TIME_OUT_EN_Msk    (1ul << UART_IER_TIME_OUT_EN_Pos)

#define UART_IER_WAKE_EN_Pos        6
#define UART_IER_WAKE_EN_Msk        (1ul << UART_IER_WAKE_EN_Pos)

#define UART_IER_BUF_ERR_IEN_Pos    5
#define UART_IER_BUF_ERR_IEN_Msk    (1ul << UART_IER_BUF_ERR_IEN_Pos)

#define UART_IER_RTO_IEN_Pos        4
#define UART_IER_RTO_IEN_Msk        (1ul << UART_IER_RTO_IEN_Pos)

#define UART_IER_MODEM_IEN_Pos      3
#define UART_IER_MODEM_IEN_Msk      (1ul << UART_IER_MODEM_IEN_Pos)

#define UART_IER_RLS_IEN_Pos        2
#define UART_IER_RLS_IEN_Msk        (1ul << UART_IER_RLS_IENN_Pos)

#define UART_IER_THRE_IEN_Pos       1
#define UART_IER_THRE_IEN_Msk       (1ul << UART_IER_THRE_IEN_Pos)

#define UART_IER_RDA_IEN_Pos        0
#define UART_IER_RDA_IEN_Msk        (1ul << UART_IER_RDA_IEN_Pos)

//
//! UART FCR Bit Field Definitions 
//
#define UART_FCR_RTS_TRI_LEV_Pos    16
#define UART_FCR_RTS_TRI_LEV_Msk    (0xFul << UART_FCR_RTS_TRI_LEV_Pos)

#define UART_FCR_RX_DIS_Pos         8
#define UART_FCR_RX_DIS_Msk         (1ul << UART_FCR_RX_DIS_Pos)

#define UART_FCR_RFITL_Pos          4
#define UART_FCR_RFITL_Msk          (0xFul << UART_FCR_RFITL_Pos)

#define UART_FCR_TFR_Pos            2
#define UART_FCR_TFR_Msk            (1ul << UART_FCR_TFR_Pos)

#define UART_FCR_RFR_Pos            1
#define UART_FCR_RFR_Msk            (1ul << UART_FCR_RFR_Pos)

//
//! UART LCR Bit Field Definitions
//
#define UART_LCR_BCB_Pos            6
#define UART_LCR_BCB_Msk            (1ul << UART_LCR_BCB_Pos)

#define UART_LCR_SPE_Pos            5
#define UART_LCR_SPE_Msk            (1ul << UART_LCR_SPE_Pos)

#define UART_LCR_EPE_Pos            4
#define UART_LCR_EPE_Msk            (1ul << UART_LCR_EPE_Pos)

#define UART_LCR_PBE_Pos            3
#define UART_LCR_PBE_Msk            (1ul << UART_LCR_PBE_Pos)

#define UART_LCR_NSB_Pos            2
#define UART_LCR_NSB_Msk            (1ul << UART_LCR_NSB_Pos)

#define UART_LCR_WLS_Pos            0
#define UART_LCR_WLS_Msk            (0x3ul << UART_LCR_WLS_Pos)

//
//! UART MCR Bit Field Definitions
//
#define UART_MCR_RTS_ST_Pos         13
#define UART_MCR_RTS_ST_Msk         (1ul << UART_MCR_RTS_ST_Pos)

#define UART_MCR_LEV_RTS_Pos        9
#define UART_MCR_LEV_RTS_Msk        (1ul << UART_MCR_LEV_RTS_Pos)

#define UART_MCR_RTS_Pos            1
#define UART_MCR_RTS_Msk            (1ul << UART_MCR_RTS_Pos)

//
//! UART MSR Bit Field Definitions
//
#define UART_MSR_LEV_CTS_Pos        8
#define UART_MSR_LEV_CTS_Msk        (1ul << UART_MSR_LEV_CTS_Pos)

#define UART_MSR_CTS_ST_Pos         4
#define UART_MSR_CTS_ST_Msk         (1ul << UART_MSR_CTS_ST_Pos)

#define UART_MSR_DCTSF_Pos          0
#define UART_MSR_DCTSF_Msk          (1ul << UART_MSR_DCTSF_Pos)

//
//! UART FSR Bit Field Definitions
//
#define UART_FSR_TE_FLAG_Pos        28
#define UART_FSR_TE_FLAG_Msk        (1ul << UART_FSR_TE_FLAG_Pos)

#define UART_FSR_TX_OVER_IF_Pos     24
#define UART_FSR_TX_OVER_IF_Msk     (1ul << UART_FSR_TX_OVER_IF_Pos)

#define UART_FSR_TX_FULL_Pos        23
#define UART_FSR_TX_FULL_Msk        (1ul << UART_FSR_TX_FULL_Pos)

#define UART_FSR_TX_EMPTY_Pos       22
#define UART_FSR_TX_EMPTY_Msk       (1ul << UART_FSR_TX_EMPTY_Pos)

#define UART_FSR_TX_POINTER_Pos     16
#define UART_FSR_TX_POINTER_Msk     (0x3Ful << UART_FSR_TX_POINTER_Pos)

#define UART_FSR_RX_FULL_Pos        15
#define UART_FSR_RX_FULL_Msk        (1ul << UART_FSR_RX_FULL_Pos)

#define UART_FSR_RX_EMPTY_Pos       14
#define UART_FSR_RX_EMPTY_Msk       (1ul << UART_FSR_RX_EMPTY_Pos)

#define UART_FSR_RX_POINTER_Pos     8
#define UART_FSR_RX_POINTER_Msk     (0x3Ful << UART_FSR_RX_POINTER_Pos)

#define UART_FSR_BIF_Pos            6
#define UART_FSR_BIF_Msk            (1ul << UART_FSR_BIF_Pos)

#define UART_FSR_FEF_Pos            5
#define UART_FSR_FEF_Msk            (1ul << UART_FSR_FEF_Pos)

#define UART_FSR_PEF_Pos            4
#define UART_FSR_PEF_Msk            (1ul << UART_FSR_PEF_Pos)

#define UART_FSR_RS485_ADD_DETF_Pos 3
#define UART_FSR_RS485_ADD_DETF_Msk (1ul << UART_FSR_RS485_ADD_DETF_Pos)

#define UART_FSR_RX_OVER_IF_Pos     0
#define UART_FSR_RX_OVER_IF_Msk     (1ul << UART_FSR_RX_OVER_IF_Pos)

//
//! UART ISR Bit Field Definitions
//
#define UART_ISR_BUF_ERR_INT_Pos           13
#define UART_ISR_BUF_ERR_INT_Msk           (1ul << UART_ISR_BUF_ERR_INT_Pos)

#define UART_ISR_TOUT_INT_Pos              12
#define UART_ISR_TOUT_INT_Msk              (1ul << UART_ISR_TOUT_INT_Pos)

#define UART_ISR_MODEM_INT_Pos             11    
#define UART_ISR_MODEM_INT_Msk             (1ul << UART_ISR_MODEM_INT_Pos)

#define UART_ISR_RLS_INT_Pos               10    
#define UART_ISR_RLS_INT_Msk               (1ul << UART_ISR_RLS_INT_Pos)

#define UART_ISR_THRE_INT_Pos              9    
#define UART_ISR_THRE_INT_Msk              (1ul << UART_ISR_THRE_INT_Pos)

#define UART_ISR_RDA_INT_Pos               8    
#define UART_ISR_RDA_INT_Msk               (1ul << UART_ISR_RDA_INT_Pos)

#define UART_ISR_BUF_ERR_IF_Pos            5    
#define UART_ISR_BUF_ERR_IF_Msk            (1ul << UART_ISR_BUF_ERR_IF_Pos)

#define UART_ISR_TOUT_IF_Pos               4    
#define UART_ISR_TOUT_IF_Msk               (1ul << UART_ISR_TOUT_IF_Pos)

#define UART_ISR_MODEM_IF_Pos              3    
#define UART_ISR_MODEM_IF_Msk              (1ul << UART_ISR_MODEM_IF_Pos)

#define UART_ISR_RLS_IF_Pos                2    
#define UART_ISR_RLS_IF_Msk                (1ul << UART_ISR_RLS_IF_Pos)

#define UART_ISR_THRE_IF_Pos               1    
#define UART_ISR_THRE_IF_Msk               (1ul << UART_ISR_THRE_IF_Pos)

#define UART_ISR_RDA_IF_Pos                0    
#define UART_ISR_RDA_IF_Msk                (1ul << UART_ISR_RDA_IF_Pos)

//
//! UART TOR Bit Field Definitions
//
#define UART_TOR_DLY_Pos           8                            
#define UART_TOR_DLY_Msk           (0xFFul << UART_TOR_DLY_Pos)

#define UART_TOR_TOIC_Pos          0
#define UART_TOR_TOIC_Msk          (0xFFul << UART_TOR_TOIC_Pos)

//
//! UART BAUD Bit Field Definitions
//
#define UART_BAUD_DIV_X_EN_Pos    29
#define UART_BAUD_DIV_X_EN_Msk    (1ul << UART_BAUD_DIV_X_EN_Pos)

#define UART_BAUD_DIV_X_ONE_Pos   28
#define UART_BAUD_DIV_X_ONE_Msk   (1ul << UART_BAUD_DIV_X_ONE_Pos)

#define UART_BAUD_DIVIDER_X_Pos   24
#define UART_BAUD_DIVIDER_X_Msk   (0xFul << UART_BAUD_DIVIDER_X_Pos)

#define UART_BAUD_BRD_Pos         0
#define UART_BAUD_BRD_Msk         (0xFFul << UART_BAUD_BRD_Pos)

//
//! UART IRCR Bit Field Definitions
//
#define UART_IRCR_INV_RX_Pos      6
#define UART_IRCR_INV_RX_Msk     (1ul << UART_IRCR_INV_RX_Pos)

#define UART_IRCR_INV_TX_Pos      5
#define UART_IRCR_INV_TX_Msk     (1ul << UART_IRCR_INV_TX_Pos)

#define UART_IRCR_TX_SELECT_Pos   1
#define UART_IRCR_TX_SELECT_Msk   (1ul << UART_IRCR_TX_SELECT_Pos)

//
//! UART ALT_CSR Bit Field Definitions
//
#define UART_ALT_CSR_ADDR_MATCH_Pos      24
#define UART_ALT_CSR_ADDR_MATCH_Msk     (0xFFul << UART_ALT_CSR_ADDR_MATCH_Pos)

#define UART_ALT_CSR_RS485_ADD_EN_Pos   15
#define UART_ALT_CSR_RS485_ADD_EN_Msk   (1ul << UART_ALT_CSR_RS485_ADD_EN_Pos)

#define UART_ALT_CSR_RS485_AUD_Pos      10
#define UART_ALT_CSR_RS485_AUD_Msk      (1ul << UART_ALT_CSR_RS485_AUD_Pos)

#define UART_ALT_CSR_RS485_AAD_Pos      9
#define UART_ALT_CSR_RS485_AAD_Msk      (1ul << UART_ALT_CSR_RS485_AAD_Pos)

#define UART_ALT_CSR_RS485_NMM_Pos      8
#define UART_ALT_CSR_RS485_NMM_Msk      (1ul << UART_ALT_CSR_RS485_NMM_Pos)

//
//! UART FUN_SEL Bit Field Definitions
//
#define UART_FUN_SEL_FUN_SEL_Pos        0
#define UART_FUN_SEL_FUN_SEL_Msk       (0x3ul << UART_FUN_SEL_FUN_SEL_Pos)

//*****************************************************************************
//
//! Timer Controller      
//
//*****************************************************************************
typedef struct
{
    __IO uint32_t  PRESCALE:8;
    __I  uint32_t  RESERVE0:8;
    __IO uint32_t  TDR_EN:1;
    __I  uint32_t  RESERVE1:8;
    __IO uint32_t  CACT:1;
    __IO uint32_t  CRST:1;
    __IO uint32_t  MODE:2;
    __IO uint32_t  IE:1;
    __IO uint32_t  CEN:1;
    __IO uint32_t  DBGACK_TMR:1;
} TIMER_TCSR_T;

typedef __IO uint32_t TIMER_TCMPR_T;

typedef struct
{
    __IO uint32_t  TIF:1;
    __I  uint32_t  RESERVE:31;
} TIMER_TISR_T;

typedef __IO uint32_t TIMER_TDR_T;

typedef struct
{
    union {
        __IO uint32_t u32TCSR;
        struct {
            __IO uint32_t  PRESCALE:8;
            __I  uint32_t  RESERVE0:8;
            __IO uint32_t  TDR_EN:1;
            __I  uint32_t  RESERVE1:8;
            __IO uint32_t  CACT:1;
            __IO uint32_t  CRST:1;
            __IO uint32_t  MODE:2;
            __IO uint32_t  IE:1;
            __IO uint32_t  CEN:1;
            __IO uint32_t  DBGACK_TMR:1;
        } TCSR;
    };

    union {
        __IO uint32_t u32TCMPR;
        __IO uint32_t TCMPR;
    };

    union {
        __IO uint32_t u32TISR;
        struct {
            __IO uint32_t  TIF:1;
            __I  uint32_t  RESERVE:31;
        } TISR;
    };

    union {
        __IO uint32_t u32TDR;
        __IO uint32_t TDR;
    };
 } TIMER_T;

//
//! Timer TCSR Bit Field Definitions
//
#define TIMER_TCSR_DBGACK_TMR_Pos   31
#define TIMER_TCSR_DBGACK_TMR_Msk   (1ul << TIMER_TCSR_DBGACK_TMR_Pos)

#define TIMER_TCSR_CEN_Pos          30
#define TIMER_TCSR_CEN_Msk          (1ul << TIMER_TCSR_CEN_Pos)

#define TIMER_TCSR_IE_Pos           29
#define TIMER_TCSR_IE_Msk           (1ul << TIMER_TCSR_IE_Pos)

#define TIMER_TCSR_MODE_Pos         27
#define TIMER_TCSR_MODE_Msk         (0x3ul << TIMER_TCSR_MODE_Pos)

#define TIMER_TCSR_CRST_Pos         26
#define TIMER_TCSR_CRST_Msk         (1ul << TIMER_TCSR_CRST_Pos)

#define TIMER_TCSR_CACT_Pos         25
#define TIMER_TCSR_CACT_Msk         (1ul << TIMER_TCSR_CACT_Pos)

#define TIMER_TCSR_TDR_EN_Pos       16
#define TIMER_TCSR_TDR_EN_Msk       (1ul << TIMER_TCSR_TDR_EN_Pos)

#define TIMER_TCSR_PRESCALE_Pos     0
#define TIMER_TCSR_PRESCALE_Msk     (0xFFul << TIMER_TCSR_PRESCALE_Pos)

//
//! Timer TCMPR Bit Field Definitions
//
#define TIMER_TCMP_Pos              0
#define TIMER_TCMP_Msk              (0xFFFFFFul << TIMER_TCMP_Pos)

//
//! Timer TISR Bit Field Definitions
//
#define TIMER_TISR_TIF_Pos          0
#define TIMER_TISR_TIF_Msk          (1ul << TIMER_TISR_TIF_Pos)

//
//! Timer TDR Bit Field Definitions
//
#define TIMER_TDR_Pos               0
#define TIMER_TDR_Msk               (0xFFFFFFul << TIMER_TDR_Pos)

//*****************************************************************************
//
//! WDT Controller      
//
//*****************************************************************************
typedef struct
{
    __IO uint32_t  WTR:1;
    __IO uint32_t  WTRE:1;
    __IO uint32_t  WTRF:1;
    __IO uint32_t  WTIF:1;
    __IO uint32_t  WTWKE:1;
    __IO uint32_t  WTWKF:1;
    __IO uint32_t  WTIE:1;
    __IO uint32_t  WTE:1;
    __IO uint32_t  WTIS:3;
    __I  uint32_t  RESERVE1:20;
    __IO uint32_t  DBGACK_WDT:1;
} WDT_WTCR_T;

typedef struct
{
    union {
        __IO uint32_t u32WTCR;
        struct {
            __IO uint32_t  WTR:1;
            __IO uint32_t  WTRE:1;
            __IO uint32_t  WTRF:1;
            __IO uint32_t  WTIF:1;
            __IO uint32_t  WTWKE:1;
            __IO uint32_t  WTWKF:1;
            __IO uint32_t  WTIE:1;
            __IO uint32_t  WTE:1;
            __IO uint32_t  WTIS:3;
            __I  uint32_t  RESERVE1:20;
            __IO uint32_t  DBGACK_WDT:1;
        } WTCR;
    };
} WDT_T;

//
//! WDT WTCR Bit Field Definitions
//
#define WDT_WTCR_DBGACK_WDT_Pos 31
#define WDT_WTCR_DBGACK_WDT_Msk (1ul << WDT_WTCR_DBGACK_WDT_Pos)

#define WDT_WTCR_WTIS_Pos       8  
#define WDT_WTCR_WTIS_Msk       (0x3ul << WDT_WTCR_WTIS_Pos)

#define WDT_WTCR_WTE_Pos        7  
#define WDT_WTCR_WTE_Msk        (1ul << WDT_WTCR_WTE_Pos)

#define WDT_WTCR_WTIE_Pos       6  
#define WDT_WTCR_WTIE_Msk       (1ul << WDT_WTCR_WTIE_Pos)

#define WDT_WTCR_WTWKF_Pos      5  
#define WDT_WTCR_WTWKF_Msk      (1ul << WDT_WTCR_WTWKF_Pos)

#define WDT_WTCR_WTWKE_Pos      4  
#define WDT_WTCR_WTWKE_Msk      (1ul << WDT_WTCR_WTWKE_Pos)

#define WDT_WTCR_WTIF_Pos       3  
#define WDT_WTCR_WTIF_Msk       (1ul << WDT_WTCR_WTIF_Pos)

#define WDT_WTCR_WTRF_Pos       2  
#define WDT_WTCR_WTRF_Msk       (1ul << WDT_WTCR_WTRF_Pos)

#define WDT_WTCR_WTRE_Pos       1  
#define WDT_WTCR_WTRE_Msk       (1ul << WDT_WTCR_WTRE_Pos)

#define WDT_WTCR_WTR_Pos        0  
#define WDT_WTCR_WTR_Msk        (1ul << WDT_WTCR_WTR_Pos)

//*****************************************************************************
//
//! SPI Interface Controller     
//
//*****************************************************************************
typedef struct
{
    __IO uint32_t  GO_BUSY:1;
    __IO uint32_t  RX_NEG:1;
    __IO uint32_t  TX_NEG:1;
    __IO uint32_t  TX_BIT_LEN:5;
    __IO uint32_t  TX_NUM:2;
    __IO uint32_t  LSB:1;
    __IO uint32_t  CLKP:1;
    __IO uint32_t  SP_CYCLE:4;
    __IO uint32_t  IF:1;
    __IO uint32_t  IE:1;
    __IO uint32_t  SLAVE:1;
    __IO uint32_t  REORDER:2;
    __I  uint32_t  RESERVE0:2;
    __IO uint32_t  VARCLK_EN:1;
    __I  uint32_t  RESERVE1:8;
} SPI_CNTRL_T;

typedef struct
{
    __IO uint32_t  DIVIDER:16;
    __IO uint32_t  DIVIDER2:16;
} SPI_DIVIDER_T;

typedef struct
{
    __IO uint32_t  SSR:1;
    __I  uint32_t  RESERVE0:1;
    __IO uint32_t  SS_LVL:1;
    __IO uint32_t  AUTOSS:1;
    __IO uint32_t  SS_LTRIG:1;
    __I  uint32_t  LTRIG_FLAG:1;
    __I  uint32_t  RESERVE:26;
} SPI_SSR_T;

typedef __I  uint32_t   SPI_RX_T;
typedef __O  uint32_t   SPI_TX_T;
typedef __IO uint32_t   SPI_VARCLK_T;


typedef struct
{
    union {
        __IO uint32_t u32CNTRL;
        struct {
            __IO uint32_t  GO_BUSY:1;
            __IO uint32_t  RX_NEG:1;
            __IO uint32_t  TX_NEG:1;
            __IO uint32_t  TX_BIT_LEN:5;
            __IO uint32_t  TX_NUM:2;
            __IO uint32_t  LSB:1;
            __IO uint32_t  CLKP:1;
            __IO uint32_t  SP_CYCLE:4;
            __IO uint32_t  IF:1;
            __IO uint32_t  IE:1;
            __IO uint32_t  SLAVE:1;
            __IO uint32_t  REORDER:2;
            __I  uint32_t  RESERVE0:2;
            __IO uint32_t  VARCLK_EN:1;
            __I  uint32_t  RESERVE1:8;
        } CNTRL;
    };

    union {
        __IO uint32_t u32DIVIDER;
        struct {
            __IO uint32_t  DIVIDER:16;
            __IO uint32_t  DIVIDER2:16;
        } DIVIDER;
    };

    union {
        __IO uint32_t u32SSR;
        struct {
            __IO uint32_t  SSR:1;
            __I  uint32_t  RESERVE0:1;
            __IO uint32_t  SS_LVL:1;
            __IO uint32_t  AUTOSS:1;
            __IO uint32_t  SS_LTRIG:1;
            __I  uint32_t  LTRIG_FLAG:1;
            __I  uint32_t  RESERVE1:26;
        } SSR;
    };

    __I uint32_t RESERVE0;

    union {
        __I uint32_t u32RX[2];
        __I uint32_t RX[2];
    };

    __I uint32_t RESERVE1;
    __I uint32_t RESERVE2;

    union {
        __O uint32_t u32TX[2];
        __O uint32_t TX[2];
    };

    __I uint32_t RESERVE3;
    __I uint32_t RESERVE4;
    __I uint32_t RESERVE5;

    union {
        __IO uint32_t u32VARCLK;
        __IO uint32_t VARCLK;
    };

} SPI_T;

//
//! SPI_CNTRL Bit Field Definitions
//
#define SPI_CNTRL_VARCLK_EN_Pos    23
#define SPI_CNTRL_VARCLK_EN_Msk    (1ul << SPI_CNTRL_VARCLK_EN_Pos)

#define SPI_CNTRL_REORDER_Pos      19
#define SPI_CNTRL_REORDER_Msk      (3ul << SPI_CNTRL_REORDER_Pos)

#define SPI_CNTRL_SLAVE_Pos        18
#define SPI_CNTRL_SLAVE_Msk        (1ul << SPI_CNTRL_SLAVE_Pos)

#define SPI_CNTRL_IE_Pos           17
#define SPI_CNTRL_IE_Msk           (1ul << SPI_CNTRL_IE_Pos)

#define SPI_CNTRL_IF_Pos           16
#define SPI_CNTRL_IF_Msk           (1ul << SPI_CNTRL_IF_Pos)

#define SPI_CNTRL_SP_CYCLE_Pos     12
#define SPI_CNTRL_SP_CYCLE_Msk     (0xFul << SPI_CNTRL_SP_CYCLE_Pos)

#define SPI_CNTRL_CLKP_Pos         11
#define SPI_CNTRL_CLKP_Msk         (1ul << SPI_CNTRL_CLKP_Pos)

#define SPI_CNTRL_LSB_Pos          10
#define SPI_CNTRL_LSB_Msk          (1ul << SPI_CNTRL_LSB_Pos)

#define SPI_CNTRL_TX_NUM_Pos       8
#define SPI_CNTRL_TX_NUM_Msk       (3ul << SPI_CNTRL_TX_NUM_Pos)

#define SPI_CNTRL_TX_BIT_LEN_Pos   3
#define SPI_CNTRL_TX_BIT_LEN_Msk   (0x1Ful << SPI_CNTRL_TX_BIT_LEN_Pos)

#define SPI_CNTRL_TX_NEG_Pos       2
#define SPI_CNTRL_TX_NEG_Msk       (1ul << SPI_CNTRL_TX_NEG_Pos)

#define SPI_CNTRL_RX_NEG_Pos       1
#define SPI_CNTRL_RX_NEG_Msk       (1ul << SPI_CNTRL_RX_NEG_Pos)

#define SPI_CNTRL_GO_BUSY_Pos      0
#define SPI_CNTRL_GO_BUSY_Msk      (1ul << SPI_CNTRL_GO_BUSY_Pos)

//
//! SPI_DIVIDER Bit Field Definitions
//
#define SPI_DIVIDER_DIVIDER2_Pos   16
#define SPI_DIVIDER_DIVIDER2_Msk   (0xFFFFul << SPI_DIVIDER_DIVIDER2_Pos)

#define SPI_DIVIDER_DIVIDER_Pos    0
#define SPI_DIVIDER_DIVIDE2_Msk    (0xFFFFul << SPI_DIVIDER_DIVIDER_Pos)

//
//! SPI_SSR Bit Field Definitions
//
#define SPI_SSR_LTRIG_FLAG_Pos     5
#define SPI_SSR_LTRIG_FLAG_Msk     (1ul << SPI_SSR_LTRIG_FLAG_Pos)

#define SPI_SSR_SS_LTRIG_Pos       4
#define SPI_SSR_SS_LTRIG_Msk       (1ul << SPI_SSR_SS_LTRIG_Pos)

#define SPI_SSR_AUTOSS_Pos         3
#define SPI_SSR_AUTOSS_Msk         (1ul << SPI_SSR_AUTOSS_Pos)

#define SPI_SSR_SS_LVL_Pos         2
#define SPI_SSR_SS_LVL_Msk         (1ul << SPI_SSR_SS_LVL_Pos)

#define SPI_SSR_SSR_Pos            0
#define SPI_SSR_SSR_Msk            (1ul << SPI_SSR_SSR_Pos)

//*****************************************************************************
//
//! I2C Controller    
//
//*****************************************************************************
typedef struct
{
    __I  uint32_t  RESERVE0:2;
    __IO uint32_t  AA:1;
    __IO uint32_t  SI:1;
    __IO uint32_t  STO:1;
    __IO uint32_t  STA:1;
    __IO uint32_t  ENS1:1;
   	__IO uint32_t  EI:1;
    __I  uint32_t  RESERVE1:24;
} I2C_I2CON_T;

typedef struct
{
    __IO uint32_t  GC:1;
    __IO uint32_t  I2CADDR:7;
    __I  uint32_t  RESERVE:24;
} I2C_I2CADDR_T;

typedef __IO uint32_t I2C_I2CDAT_T;

typedef __I  uint32_t I2C_I2CSTATUS_T;

typedef __IO uint32_t I2C_I2CLK_T;

typedef struct
{
    __IO uint32_t  TIF:1;
    __IO uint32_t  DIV4:1;
    __IO uint32_t  ENTI:1;
    __I  uint32_t  RESERVE:29;
} I2C_I2CTOC_T;

typedef struct
{
    __I  uint32_t  RESERVE0:1;
    __IO uint32_t  I2CADM:7;
    __I  uint32_t  RESERVE1:24;
} I2C_I2CADM_T;

typedef struct
{
    union 
    {
        __IO uint32_t u32I2CON;
        
        struct 
        {
            __I  uint32_t  RESERVE0:2;
            __IO uint32_t  AA:1;
            __IO uint32_t  SI:1;
            __IO uint32_t  STO:1;
            __IO uint32_t  STA:1;
            __IO uint32_t  ENS1:1;
            __IO uint32_t  EI:1;
            __I  uint32_t  RESERVE1:24;
        } I2CON;
    };

    union 
    {
        __IO uint32_t u32I2CADDR0;
        
        struct 
        {
            __IO uint32_t  GC:1;
            __IO uint32_t  I2CADDR:7;
            __I  uint32_t  RESERVE:24;
        } I2CADDR0;
    };

    union 
    {
        __IO uint32_t u32I2CDAT;
        __IO uint32_t I2CDAT;
    };

    union 
    {
        __I uint32_t u32I2CSTATUS;
        __I uint32_t I2CSTATUS;
    };
    
    union 
    {
        __IO uint32_t u32I2CLK;
        __IO uint32_t I2CLK;
    };

    union 
    {
        __IO uint32_t u32I2CTOC;
        
        struct 
        {
            __IO uint32_t  TIF:1;
            __IO uint32_t  DIV4:1;
            __IO uint32_t  ENTI:1;
            __I  uint32_t  RESERVE:29;
        } I2CTOC;
    };

    union 
    {
        __IO uint32_t u32I2CADDR1;
        
        struct 
        {
            __IO uint32_t  GC:1;
            __IO uint32_t  I2CADDR:7;
            __I  uint32_t  RESERVE:24;
        } I2CADDR1;
    };
    
    union 
    {
        __IO uint32_t u32I2CADDR2;
        
        struct 
        {
            __IO uint32_t  GC:1;
            __IO uint32_t  I2CADDR:7;
            __I  uint32_t  RESERVE:24;
        } I2CADDR2;
    };    
    
    union 
    {
        __IO uint32_t u32I2CADDR3;
        
        struct 
        {
            __IO uint32_t  GC:1;
            __IO uint32_t  I2CADDR:7;
            __I  uint32_t  RESERVE:24;
        } I2CADDR3;
    }; 
    
    union 
    {
        __IO uint32_t u32I2CADM0;
        
        struct 
        {
            __I  uint32_t  RESERVE0:1;
            __IO uint32_t  I2CADM:7;
            __I  uint32_t  RESERVE1:24;
        } I2CADM0;
    };     
    
    union 
    {
        __IO uint32_t u32I2CADM1;
        
        struct 
        {
            __I  uint32_t  RESERVE0:1;
            __IO uint32_t  I2CADM:7;
            __I  uint32_t  RESERVE1:24;
        } I2CADM1;
    };

    union 
    {
        __IO uint32_t u32I2CADM2;
        
        struct 
        {
            __I  uint32_t  RESERVE0:1;
            __IO uint32_t  I2CADM:7;
            __I  uint32_t  RESERVE1:24;
        } I2CADM2;
    };
       
    union 
    {
        __IO uint32_t u32I2CADM3;
        
        struct 
        {
            __I  uint32_t  RESERVE0:1;
            __IO uint32_t  I2CADM:7;
            __I  uint32_t  RESERVE1:24;
        } I2CADM3;
    };       
} I2C_T;

//
//! I2C I2CON Bit Field Definitions 
//
#define I2C_I2CON_EI_Pos                        7
#define I2C_I2CON_EI_Msk                        (1ul << I2C_I2CON_EI_Pos)

#define I2C_I2CON_ENS1_Pos                      6
#define I2C_I2CON_ENS1_Msk                      (1ul << I2C_I2CON_ENS1_Pos)

#define I2C_I2CON_STA_Pos                       5
#define I2C_I2CON_STA_Msk                       (1ul << I2C_I2CON_STA_Pos)

#define I2C_I2CON_STO_Pos                       4
#define I2C_I2CON_STO_Msk                       (1ul << I2C_I2CON_STO_Pos)

#define I2C_I2CON_SI_Pos                        3
#define I2C_I2CON_SI_Msk                        (1ul << I2C_I2CON_SI_Pos)

#define I2C_I2CON_AA_Pos                        2
#define I2C_I2CON_AA_Msk                        (1ul << I2C_I2CON_AA_Pos)

//
//! I2C I2CADDR Bit Field Definitions
//
#define I2C_I2CADDR_I2CADDR_Pos                 1
#define I2C_I2CADDR_I2CADDR_Msk                 (0x7Ful << I2C_I2CADDR_I2CADDR_Pos)

#define I2C_I2CADDR_GC_Pos                      0
#define I2C_I2CADDR_GC_Msk                      (1ul << I2C_I2CADDR_GC_Pos)

//
//! I2C I2CDAT Bit Field Definitions
//
#define I2C_I2CDAT_I2CDAT_Pos                   0
#define I2C_I2CDAT_I2CDAT_Msk                   (0xFFul << I2C_I2CDAT_I2CDAT_Pos)

//
//! I2C I2CSTATUS Bit Field Definitions
//
#define I2C_I2CSTATUS_I2CSTATUS_Pos             0
#define I2C_I2CSTATUS_I2CSTATUS_Msk             (0xFFul << I2C_I2CSTATUS_I2CSTATUS_Pos)

//
//! I2C I2CLK Bit Field Definitions
//
#define I2C_I2CLK_I2CLK_Pos                     0
#define I2C_I2CLK_I2CLK_Msk                     (0xFFul << I2C_I2CLK_I2CLK_Pos)

//
//! I2C I2CTOC Bit Field Definitions
//
#define I2C_I2CTOC_ENTI_Pos                     2
#define I2C_I2CTOC_ENTI_Msk                     (1ul << I2C_I2CTOC_ENTI_Pos)

#define I2C_I2CTOC_DIV4_Pos                     1
#define I2C_I2CTOC_DIV4_Msk                     (1ul << I2C_I2CTOC_DIV4_Pos)

#define I2C_I2CTOC_TIF_Pos                      0
#define I2C_I2CTOC_TIF_Msk                      (1ul << I2C_I2CTOC_TIF_Pos)

//
//! I2C I2CADM Bit Field Definitions
//
#define I2C_I2CADM_I2CADM_Pos                   1
#define I2C_I2CADM_I2CADM_Msk                   (0x7Ful << I2C_I2CADM_I2CADM_Pos)

//*****************************************************************************
//
//! ADC Controller   
//
//*****************************************************************************
typedef struct
{
    __I  uint32_t  RSLT:16;
    __I  uint32_t  OVERRUN:1;
    __I  uint32_t  VALID:1;
    __I  uint32_t  RESERVE1:14;
} ADC_ADDR_T;

typedef struct
{
    __IO uint32_t  ADEN:1;
    __IO uint32_t  ADIE:1;
    __IO uint32_t  ADMD:2;
    __IO uint32_t  TRGS:2;
    __IO uint32_t  TRGCOND:2;
    __IO uint32_t  TRGEN:1;
    __I  uint32_t  RESERVE0:1;
    __IO uint32_t  DIFFEN:1;
    __IO uint32_t  ADST:1;
    __I  uint32_t  RESERVE1:20;
} ADC_ADCR_T;

typedef struct
{
    __IO uint32_t  CHEN:8;
    __IO uint32_t  PRESEL:2;
    __I  uint32_t  RESERVE:22;
} ADC_ADCHER_T;

typedef struct
{
    __IO uint32_t  CMPEN:1;
    __IO uint32_t  CMPIE:1;
    __IO uint32_t  CMPCOND:1;
    __IO uint32_t  CMPCH:3;
    __I  uint32_t  RESERVE0:2;
    __IO uint32_t  CMPMATCNT:4;
    __I  uint32_t  RESERVE1:4;
    __IO uint32_t  CMPD:12;
    __I  uint32_t  RESERVE2:4;
} ADC_ADCMPR_T;

typedef struct
{
    __IO uint32_t  ADF:1;
    __IO uint32_t  CMPF0:1;
    __IO uint32_t  CMPF1:1;
    __I  uint32_t  BUSY:1;
    __I  uint32_t  CHANNEL:3;
    __I  uint32_t  RESERVE0:1;
    __I  uint32_t  VALID:8;
    __I  uint32_t  OVERRUN:8;
    __I  uint32_t  RESERVE1:8;
} ADC_ADSR_T;

typedef struct
{
    __IO uint32_t  CALEN:1;
    __I  uint32_t  CALDONE:1;
    __I  uint32_t  RESERVE:30;
} ADC_ADCALR_T;


typedef struct
{
    union {
        __I uint32_t u32ADDR[8];
        struct {
            __I  uint32_t  RSLT:16;
            __I  uint32_t  OVERRUN:1;
            __I  uint32_t  VALID:1;
            __I  uint32_t  RESERVE1:14;
        } ADDR[8];
    };
    
    union {
        __IO uint32_t u32ADCR;
        struct {
            __IO uint32_t  ADEN:1;
            __IO uint32_t  ADIE:1;
            __IO uint32_t  ADMD:2;
            __IO uint32_t  TRGS:2;
            __IO uint32_t  TRGCOND:2;
            __IO uint32_t  TRGEN:1;
            __I  uint32_t  RESERVE0:1;
            __IO uint32_t  DIFFEN:1;
            __IO uint32_t  ADST:1;
            __I  uint32_t  RESERVE1:20;
        } ADCR;
    };
    
    union {
        __IO uint32_t u32ADCHER;
        struct {
            __IO uint32_t  CHEN:8;
            __IO uint32_t  PRESEL:2;
            __I  uint32_t  RESERVE:22;
        } ADCHER;
    };
    
    union {
        __IO uint32_t u32ADCMPR[2];
        struct {
            __IO uint32_t  CMPEN:1;
            __IO uint32_t  CMPIE:1;
            __IO uint32_t  CMPCOND:1;
            __IO uint32_t  CMPCH:3;
            __I  uint32_t  RESERVE0:2;
            __IO uint32_t  CMPMATCNT:4;
            __I  uint32_t  RESERVE1:4;
            __IO uint32_t  CMPD:12;
            __I  uint32_t  RESERVE2:4;
        } ADCMPR[2];
    };
    
    union {
        __IO uint32_t u32ADSR;
        struct {
            __IO uint32_t  ADF:1;
            __IO uint32_t  CMPF0:1;
            __IO uint32_t  CMPF1:1;
            __I  uint32_t  BUSY:1;
            __I  uint32_t  CHANNEL:3;
            __I  uint32_t  RESERVE0:1;
            __I  uint32_t  VALID:8;
            __I  uint32_t  OVERRUN:8;
            __I  uint32_t  RESERVE1:8;
        } ADSR;
    };
    
    union {
        __IO uint32_t u32ADCALR;
        struct {
            __IO uint32_t  CALEN:1;
            __I  uint32_t  CALDONE:1;
            __I  uint32_t  RESERVE:30;
        } ADCALR;
    };
    
} ADC_T;

//
//! ADDR Bit Field Definitions
//
#define ADC_ADDR_VALID_Pos      17
#define ADC_ADDR_VALID_Msk      (1ul << ADC_ADDR_VALID_Pos)

#define ADC_ADDR_OVERRUN_Pos    16
#define ADC_ADDR_OVERRUN_Msk    (1ul << ADC_ADDR_OVERRUN_Pos)

#define ADC_ADDR_RSLT_Pos       0
#define ADC_ADDR_RSLT_Msk       (0xFFFFul << ADC_ADDR_RSLT_Pos)

//
//! ADCR Bit Field Definitions
//
#define ADC_ADCR_ADST_Pos       11
#define ADC_ADCR_ADST_Msk       (1ul << ADC_ADCR_ADST_Pos)

#define ADC_ADCR_DIFFEN_Pos     10
#define ADC_ADCR_DIFFEN_Msk     (1ul << ADC_ADCR_DIFFEN_Pos)

#define ADC_ADCR_TRGEN_Pos      8
#define ADC_ADCR_TRGEN_Msk      (1ul << ADC_ADCR_TRGEN_Pos)

#define ADC_ADCR_TRGCOND_Pos    6
#define ADC_ADCR_TRGCOND_Msk    (3ul << ADC_ADCR_TRGCOND_Pos)

#define ADC_ADCR_TRGS_Pos       4
#define ADC_ADCR_TRGS_Msk       (3ul << ADC_ADCR_TRGS_Pos)

#define ADC_ADCR_ADMD_Pos       2
#define ADC_ADCR_ADMD_Msk       (3ul << ADC_ADCR_ADMD_Pos)

#define ADC_ADCR_ADIE_Pos       1
#define ADC_ADCR_ADIE_Msk       (1ul << ADC_ADCR_ADIE_Pos)

#define ADC_ADCR_ADEN_Pos       0
#define ADC_ADCR_ADEN_Msk       (1ul << ADC_ADCR_ADEN_Pos)

//
//! ADCHER Bit Field Definitions
//
#define ADC_ADCHER_PRESEL_Pos   8
#define ADC_ADCHER_PRESEL_Msk   (3ul << ADC_ADCHER_PRESEL_Pos)

#define ADC_ADCHER_CHEN_Pos     0
#define ADC_ADCHER_CHEN_Msk     (0xFFul << ADC_ADCHER_CHEN_Pos)

//
//! ADCMPR Bit Field Definitions
//
#define ADC_ADCMPR_CMPD_Pos        16
#define ADC_ADCMPR_CMPD_Msk        (0xFFFul << ADC_ADCMPR_CMPD_Pos)

#define ADC_ADCMPR_CMPMATCNT_Pos   8
#define ADC_ADCMPR_CMPMATCNT_Msk   (0xFul << ADC_ADCMPR_CMPMATCNT_Pos)

#define ADC_ADCMPR_CMPCH_Pos       3
#define ADC_ADCMPR_CMPCH_Msk       (7ul << ADC_ADCMPR_CMPCH_Pos)

#define ADC_ADCMPR_CMPCOND_Pos     2
#define ADC_ADCMPR_CMPCOND_Msk     (1ul << ADC_ADCMPR_CMPCOND_Pos)

#define ADC_ADCMPR_CMPIE_Pos       1
#define ADC_ADCMPR_CMPIE_Msk       (1ul << ADC_ADCMPR_CMPIE_Pos)

#define ADC_ADCMPR_CMPEN_Pos       0
#define ADC_ADCMPR_CMPEN_Msk       (1ul << ADC_ADCMPR_CMPEN_Pos)

//
//! ADSR Bit Field Definitions
//
#define ADC_ADSR_OVERRUN_Pos       16
#define ADC_ADSR_OVERRUN_Msk       (0xFFul << ADC_ADSR_OVERRUN_Pos)

#define ADC_ADSR_VALID_Pos         8
#define ADC_ADSR_VALID_Msk         (0xFFul << ADC_ADSR_VALID_Pos)

#define ADC_ADSR_CHANNEL_Pos       4
#define ADC_ADSR_CHANNEL_Msk       (7ul << ADC_ADSR_CHANNEL_Pos)

#define ADC_ADSR_BUSY_Pos          3
#define ADC_ADSR_BUSY_Msk          (1ul << ADC_ADSR_BUSY_Pos)

#define ADC_ADSR_CMPF1_Pos         2
#define ADC_ADSR_CMPF1_Msk         (1ul << ADC_ADSR_CMPF1_Pos)

#define ADC_ADSR_CMPF0_Pos         1
#define ADC_ADSR_CMPF0_Msk         (1ul << ADC_ADSR_CMPF0_Pos)

#define ADC_ADSR_ADF_Pos           0
#define ADC_ADSR_ADF_Msk           (1ul << ADC_ADSR_ADF_Pos)

//
//! ADCALR Bit Field Definitions
//
#define ADC_ADCALR_CALDONE_Pos     1
#define ADC_ADCALR_CALDONE_Msk     (1ul << ADC_ADCALR_CALDONE_Pos)

#define ADC_ADCALR_CALEN_Pos       0
#define ADC_ADCALR_CALEN_Msk       (1ul << ADC_ADCALR_CALEN_Pos)


//*****************************************************************************
//
//! Clock Controller 
//
//*****************************************************************************
typedef struct
{
    __IO uint32_t  XTL12M_EN:1;
    __I  uint32_t  RESERVE0:1;
    __IO uint32_t  OSC22M_EN:1;
    __IO uint32_t  OSC10K_EN:1;
    __IO uint32_t  PD_WU_DLY:1;
    __IO uint32_t  PD_WU_INT_EN:1;
    __IO uint32_t  PD_WU_STS:1;
    __IO uint32_t  PWR_DOWN_EN:1;
    __IO uint32_t  PD_WAIT_CPU:1;
    __I  uint32_t  RESERVE1:23;
} SYSCLK_PWRCON_T;

typedef struct
{
	__I  uint32_t  RESERVE0:2;
    __IO uint32_t  ISP_EN:1;
	__IO uint32_t  EBI_EN:1;
    __I  uint32_t  RESERVE1:28;
} SYSCLK_AHBCLK_T;

typedef struct
{
    __IO uint32_t  WDT_EN:1;
	__I  uint32_t  RESERVE0:1;
    __IO uint32_t  TMR0_EN:1;
    __IO uint32_t  TMR1_EN:1;
    __IO uint32_t  TMR2_EN:1;
    __IO uint32_t  TMR3_EN:1;
    __IO uint32_t  FDIV_EN:1;
	__I  uint32_t  RESERVE1:1;
    __IO uint32_t  I2C_EN:1;
    __I  uint32_t  RESERVE2:3;
    __IO uint32_t  SPI0_EN:1;
    __IO uint32_t  SPI1_EN:1;
    __I  uint32_t  RESERVE3:2;
    __IO uint32_t  UART0_EN:1;
    __IO uint32_t  UART1_EN:1;
    __I  uint32_t  RESERVE4:2;
    __IO uint32_t  PWM01_EN:1;
    __IO uint32_t  PWM23_EN:1;
	__IO uint32_t  PWM45_EN:1;
    __IO uint32_t  PWM67_EN:1;
    __I  uint32_t  RESERVE5:4;
    __IO uint32_t  ADC_EN:1;
    __I  uint32_t  RESERVE6:3;
} SYSCLK_APBCLK_T;

typedef struct
{
    __I  uint32_t  XTL12M_STB:1;
	__I  uint32_t  RESERVE0:1;
    __I  uint32_t  PLL_STB:1;
	__I  uint32_t  OSC10K_STB:1;
	__I  uint32_t  OSC22M_STB:1;
    __I  uint32_t  RESERVE1:2;
	__IO uint32_t  CLK_SW_FAIL:1;
    __I  uint32_t  RESERVE2:24;
} SYSCLK_CLKSTATUS_T;


typedef struct
{
    __IO uint32_t  HCLK_S:3;
    __IO uint32_t  STCLK_S:3;
    __I  uint32_t  RESERVE:26;
} SYSCLK_CLKSEL0_T;

typedef struct
{
    __IO uint32_t  WDT_S:2;
    __IO uint32_t  ADC_S:2;
    __I  uint32_t  RESERVE1:4;
    __IO uint32_t  TMR0_S:3;
    __I  uint32_t  RESERVE2:1;
    __IO uint32_t  TMR1_S:3;
    __I  uint32_t  RESERVE3:1;
    __IO uint32_t  TMR2_S:3;
    __I  uint32_t  RESERVE4:1;
    __IO uint32_t  TMR3_S:3;
    __I  uint32_t  RESERVE5:1;
    __IO uint32_t  UART_S:2;
    __I  uint32_t  RESERVE6:2;
	__IO uint32_t  PWM01_S:2;
    __IO uint32_t  PWM23_S:2;
} SYSCLK_CLKSEL1_T;

typedef struct
{
    __I  uint32_t  RESERVE0:2;
	__IO uint32_t  FRQDIV_S:2;
	__IO uint32_t  PWM45_S:2;
    __IO uint32_t  PWM67_S:2;
    __I  uint32_t  RESERVE1:24;
} SYSCLK_CLKSEL2_T;

typedef struct
{
    __IO uint32_t  HCLK_N:4;
    __I  uint32_t  RESERVE0:4;
	__IO uint32_t  UART_N:4;
    __I  uint32_t  RESERVE1:4;
	__IO uint32_t  ADC_N:8;
    __I  uint32_t  RESERVE2:8;
} SYSCLK_CLKDIV_T;

typedef struct
{
    __IO uint32_t  FB_DV:9;
    __IO uint32_t  IN_DV:5;
    __IO uint32_t  OUT_DV:2;
    __IO uint32_t  PD:1;
    __IO uint32_t  BP:1;
    __IO uint32_t  OE:1;
    __IO uint32_t  PLL_SRC:1;
    __I  uint32_t  RESERVE:12;
} SYSCLK_PLLCON_T;

typedef struct
{    
    __IO uint32_t  FSEL:4;
	__IO uint32_t  FDIV_EN:1;
    __I  uint32_t  RESERVE:27;
} SYSCLK_FRQDIV_T;

typedef struct
{
    union 
    {
        __IO uint32_t u32PWRCON;
        
        struct 
        {
            __IO uint32_t  XTL12M_EN:1;
            __I  uint32_t  RESERVE0:1;
            __IO uint32_t  OSC22M_EN:1;
            __IO uint32_t  OSC10K_EN:1;
            __IO uint32_t  PD_WU_DLY:1;
            __IO uint32_t  PD_WU_INT_EN:1;
            __IO uint32_t  PD_WU_STS:1;
            __IO uint32_t  PWR_DOWN_EN:1;
            __IO uint32_t  PD_WAIT_CPU:1;
            __I  uint32_t  RESERVE1:23;
        } PWRCON;
    };    
    
    union 
    {
        __IO uint32_t u32AHBCLK;
        
        struct 
        {
        	__I  uint32_t  RESERVE0:2;
            __IO uint32_t  ISP_EN:1;
        	__IO uint32_t  EBI_EN:1;
            __I  uint32_t  RESERVE1:28;
        } AHBCLK;
    };    

    union 
    {
        __IO uint32_t u32APBCLK;
        
        struct 
        {
            __IO uint32_t  WDT_EN:1;
        	__I  uint32_t  RESERVE0:1;
            __IO uint32_t  TMR0_EN:1;
            __IO uint32_t  TMR1_EN:1;
            __IO uint32_t  TMR2_EN:1;
            __IO uint32_t  TMR3_EN:1;
            __IO uint32_t  FDIV_EN:1;
        	__I  uint32_t  RESERVE1:1;
            __IO uint32_t  I2C_EN:1;
            __I  uint32_t  RESERVE2:3;
            __IO uint32_t  SPI0_EN:1;
            __IO uint32_t  SPI1_EN:1;
            __I  uint32_t  RESERVE3:2;
            __IO uint32_t  UART0_EN:1;
            __IO uint32_t  UART1_EN:1;
            __I  uint32_t  RESERVE4:2;
            __IO uint32_t  PWM01_EN:1;
            __IO uint32_t  PWM23_EN:1;
        	__IO uint32_t  PWM45_EN:1;
            __IO uint32_t  PWM67_EN:1;
            __I  uint32_t  RESERVE5:4;
            __IO uint32_t  ADC_EN:1;
            __I  uint32_t  RESERVE6:3;
        } APBCLK;
    };          

    union 
    {
        __IO uint32_t u32CLKSTATUS;
        
        struct 
        {
            __I  uint32_t  XTL12M_STB:1;
        	__I  uint32_t  RESERVE0:1;
            __I  uint32_t  PLL_STB:1;
        	__I  uint32_t  OSC10K_STB:1;
        	__I  uint32_t  OSC22M_STB:1;
            __I  uint32_t  RESERVE1:2;
        	__IO uint32_t  CLK_SW_FAIL:1;
            __I  uint32_t  RESERVE2:24;
        } CLKSTATUS;
    };  

    union 
    {
        __IO uint32_t u32CLKSEL0;
        
        struct 
        {
            __IO uint32_t  HCLK_S:3;
            __IO uint32_t  STCLK_S:3;
            __I  uint32_t  RESERVE:26;
        } CLKSEL0;
    };  

    union 
    {
        __IO uint32_t u32CLKSEL1;
        
        struct 
        {
            __IO uint32_t  WDT_S:2;
            __IO uint32_t  ADC_S:2;
            __I  uint32_t  RESERVE1:4;
            __IO uint32_t  TMR0_S:3;
            __I  uint32_t  RESERVE2:1;
            __IO uint32_t  TMR1_S:3;
            __I  uint32_t  RESERVE3:1;
            __IO uint32_t  TMR2_S:3;
            __I  uint32_t  RESERVE4:1;
            __IO uint32_t  TMR3_S:3;
            __I  uint32_t  RESERVE5:1;
            __IO uint32_t  UART_S:2;
            __I  uint32_t  RESERVE6:2;
            __IO uint32_t  PWM01_S:2;
            __IO uint32_t  PWM23_S:2;
        } CLKSEL1;
    };  

    union 
    {
        __IO uint32_t u32CLKDIV;
        
        struct 
        {
            __IO uint32_t  HCLK_N:4;
            __I  uint32_t  RESERVE0:4;
        	__IO uint32_t  UART_N:4;
            __I  uint32_t  RESERVE1:4;
        	__IO uint32_t  ADC_N:8;
            __I  uint32_t  RESERVE2:8;
        } CLKDIV;
    };  

    union 
    {
        __IO uint32_t u32CLKSEL2;
        
        struct 
        {
            __I  uint32_t  RESERVE0:2;
        	__IO uint32_t  FRQDIV_S:2;
        	__IO uint32_t  PWM45_S:2;
            __IO uint32_t  PWM67_S:2;
            __I  uint32_t  RESERVE1:24;
        } CLKSEL2;
    };  
    
    union 
    {
        __IO uint32_t u32PLLCON;
        
        struct 
        {
            __IO uint32_t  FB_DV:9;
            __IO uint32_t  IN_DV:5;
            __IO uint32_t  OUT_DV:2;
            __IO uint32_t  PD:1;
            __IO uint32_t  BP:1;
            __IO uint32_t  OE:1;
            __IO uint32_t  PLL_SRC:1;
            __I  uint32_t  RESERVE:12;
        } PLLCON;
    };  
    
    union 
    {
        __IO uint32_t u32FRQDIV;
        
        struct 
        {
            __IO uint32_t  FSEL:4;
            __IO uint32_t  FDIV_EN:1;
            __I  uint32_t  RESERVE:27;
        } FRQDIV;
    };      
} SYSCLK_T;

//
//! SYSCLK PWRCON Bit Field Definitions
//
#define SYSCLK_PWRCON_PD_WAIT_CPU_Pos           8
#define SYSCLK_PWRCON_PD_WAIT_CPU_Msk           (1ul << SYSCLK_PWRCON_PD_WAIT_CPU_Pos)

#define SYSCLK_PWRCON_PWR_DOWN_EN_Pos           7
#define SYSCLK_PWRCON_PWR_DOWN_EN_Msk           (1ul << SYSCLK_PWRCON_PWR_DOWN_EN_Pos)

#define SYSCLK_PWRCON_PD_WU_STS_Pos             6
#define SYSCLK_PWRCON_PD_WU_STS_Msk             (1ul << SYSCLK_PWRCON_PD_WU_STS_Pos)

#define SYSCLK_PWRCON_PD_WU_INT_EN_Pos          5
#define SYSCLK_PWRCON_PD_WU_INT_EN_Msk          (1ul << SYSCLK_PWRCON_PD_WU_INT_EN_Pos)

#define SYSCLK_PWRCON_PD_WU_DLY_Pos             4
#define SYSCLK_PWRCON_PD_WU_DLY_Msk             (1ul << SYSCLK_PWRCON_PD_WU_DLY_Pos)

#define SYSCLK_PWRCON_OSC10K_EN_Pos             3
#define SYSCLK_PWRCON_OSC10K_EN_Msk             (1ul << SYSCLK_PWRCON_OSC10K_EN_Pos)

#define SYSCLK_PWRCON_OSC22M_EN_Pos             2
#define SYSCLK_PWRCON_OSC22M_EN_Msk             (1ul << SYSCLK_PWRCON_OSC22M_EN_Pos)

#define SYSCLK_PWRCON_XTL12M_EN_Pos             0
#define SYSCLK_PWRCON_XTL12M_EN_Msk             (1ul << SYSCLK_PWRCON_XTL12M_EN_Pos)

//
//! SYSCLK AHBCLK Bit Field Definitions
//
#define SYSCLK_AHBCLK_EBI_EN_Pos                3
#define SYSCLK_AHBCLK_EBI_EN_Msk                (1ul << SYSCLK_AHBCLK_EBI_EN_Pos)

#define SYSCLK_AHBCLK_ISP_EN_Pos                2
#define SYSCLK_AHBCLK_ISP_EN_Msk                (1ul << SYSCLK_AHBCLK_ISP_EN_Pos)

//
//! SYSCLK APBCLK Bit Field Definitions
//
#define SYSCLK_APBCLK_ADC_EN_Pos                28
#define SYSCLK_APBCLK_ADC_EN_Msk                (1ul << SYSCLK_APBCLK_ADC_EN_Pos)

#define SYSCLK_APBCLK_PWM67_EN_Pos              23
#define SYSCLK_APBCLK_PWM67_EN_Msk              (1ul << SYSCLK_APBCLK_PWM67_EN_Pos)

#define SYSCLK_APBCLK_PWM45_EN_Pos              22
#define SYSCLK_APBCLK_PWM45_EN_Msk              (1ul << SYSCLK_APBCLK_PWM45_EN_Pos)

#define SYSCLK_APBCLK_PWM23_EN_Pos              21
#define SYSCLK_APBCLK_PWM23_EN_Msk              (1ul << SYSCLK_APBCLK_PWM23_EN_Pos)

#define SYSCLK_APBCLK_PWM01_EN_Pos              20
#define SYSCLK_APBCLK_PWM01_EN_Msk              (1ul << SYSCLK_APBCLK_PWM01_EN_Pos)

#define SYSCLK_APBCLK_UART1_EN_Pos              17
#define SYSCLK_APBCLK_UART1_EN_Msk              (1ul << SYSCLK_APBCLK_UART1_EN_Pos)

#define SYSCLK_APBCLK_UART0_EN_Pos              16
#define SYSCLK_APBCLK_UART0_EN_Msk              (1ul << SYSCLK_APBCLK_UART0_EN_Pos)

#define SYSCLK_APBCLK_SPI1_EN_Pos               13
#define SYSCLK_APBCLK_SPI1_EN_Msk               (1ul << SYSCLK_APBCLK_SPI1_EN_Pos)

#define SYSCLK_APBCLK_SPI0_EN_Pos               12
#define SYSCLK_APBCLK_SPI0_EN_Msk               (1ul << SYSCLK_APBCLK_SPI0_EN_Pos)

#define SYSCLK_APBCLK_I2C_EN_Pos                8
#define SYSCLK_APBCLK_I2C_EN_Msk                (1ul << SYSCLK_APBCLK_I2C0_EN_Pos)

#define SYSCLK_APBCLK_FDIV_EN_Pos               6
#define SYSCLK_APBCLK_FDIV_EN_Msk               (1ul << SYSCLK_APBCLK_FDIV_EN_Pos)

#define SYSCLK_APBCLK_TMR3_EN_Pos               5
#define SYSCLK_APBCLK_TMR3_EN_Msk               (1ul << SYSCLK_APBCLK_TMR3_EN_Pos)

#define SYSCLK_APBCLK_TMR2_EN_Pos               4
#define SYSCLK_APBCLK_TMR2_EN_Msk               (1ul << SYSCLK_APBCLK_TMR2_EN_Pos)

#define SYSCLK_APBCLK_TMR1_EN_Pos               3
#define SYSCLK_APBCLK_TMR1_EN_Msk               (1ul << SYSCLK_APBCLK_TMR1_EN_Pos)
                                                
#define SYSCLK_APBCLK_TMR0_EN_Pos               2
#define SYSCLK_APBCLK_TMR0_EN_Msk               (1ul << SYSCLK_APBCLK_TMR0_EN_Pos)

#define SYSCLK_APBCLK_WDT_EN_Pos                0
#define SYSCLK_APBCLK_WDT_EN_Msk                (1ul << SYSCLK_APBCLK_WDT_EN_Pos)

//
//! SYSCLK CLKSTATUS Bit Field Definitions
//
#define SYSCLK_CLKSTATUS_CLK_SW_FAIL_Pos        7
#define SYSCLK_CLKSTATUS_CLK_SW_FAIL_Msk        (1ul << SYSCLK_CLKSTATUS_CLK_SW_FAIL_Pos)

#define SYSCLK_CLKSTATUS_OSC22M_STB_Pos         4
#define SYSCLK_CLKSTATUS_OSC22M_STB_Msk         (1ul << SYSCLK_CLKSTATUS_OSC22M_STB_Pos)

#define SYSCLK_CLKSTATUS_OSC10K_STB_Pos         3
#define SYSCLK_CLKSTATUS_OSC10K_STB_Msk         (1ul << SYSCLK_CLKSTATUS_OSC10K_STB_Pos)

#define SYSCLK_CLKSTATUS_PLL_STB_Pos            2
#define SYSCLK_CLKSTATUS_PLL_STB_Msk            (1ul << SYSCLK_CLKSTATUS_PLL_STB_Pos)

#define SYSCLK_CLKSTATUS_XTL12M_STB_Pos         0
#define SYSCLK_CLKSTATUS_XTL12M_STB_Msk         (1ul << SYSCLK_CLKSTATUS_XTL12M_STB_Pos)

//
//! SYSCLK CLKSEL0 Bit Field Definitions
//
#define SYSCLK_CLKSEL0_STCLK_S_Pos              3
#define SYSCLK_CLKSEL0_STCLK_S_Msk              (7ul << SYSCLK_CLKSEL0_STCLK_S_Pos)

#define SYSCLK_CLKSEL0_HCLK_S_Pos               0
#define SYSCLK_CLKSEL0_HCLK_S_Msk               (7ul << SYSCLK_CLKSEL0_HCLK_S_Pos)

//
//! SYSCLK CLKSEL1 Bit Field Definitions
//
#define SYSCLK_CLKSEL1_PWM23_S_Pos              30
#define SYSCLK_CLKSEL1_PWM23_S_Msk              (3ul << SYSCLK_CLKSEL1_PWM23_S_Pos)

#define SYSCLK_CLKSEL1_PWM01_S_Pos              28
#define SYSCLK_CLKSEL1_PWM01_S_Msk              (3ul << SYSCLK_CLKSEL1_PWM01_S_Pos)

#define SYSCLK_CLKSEL1_UART_S_Pos               24
#define SYSCLK_CLKSEL1_UART_S_Msk               (7ul << SYSCLK_CLKSEL1_UART_S_Pos)

#define SYSCLK_CLKSEL1_TMR3_S_Pos               20
#define SYSCLK_CLKSEL1_TMR3_S_Msk               (7ul << SYSCLK_CLKSEL1_TMR3_S_Pos)

#define SYSCLK_CLKSEL1_TMR2_S_Pos               16
#define SYSCLK_CLKSEL1_TMR2_S_Msk               (7ul << SYSCLK_CLKSEL1_TMR2_S_Pos)

#define SYSCLK_CLKSEL1_TMR1_S_Pos               12
#define SYSCLK_CLKSEL1_TMR1_S_Msk               (7ul << SYSCLK_CLKSEL1_TMR1_S_Pos)

#define SYSCLK_CLKSEL1_TMR0_S_Pos               8
#define SYSCLK_CLKSEL1_TMR0_S_Msk               (7ul << SYSCLK_CLKSEL1_TMR0_S_Pos)

#define SYSCLK_CLKSEL1_ADC_S_Pos                2
#define SYSCLK_CLKSEL1_ADC_S_Msk                (3ul << SYSCLK_CLKSEL1_ADC_S_Pos)

#define SYSCLK_CLKSEL1_WDT_S_Pos                0
#define SYSCLK_CLKSEL1_WDT_S_Msk                (3ul << SYSCLK_CLKSEL1_WDT_S_Pos)

//
//! SYSCLK CLKSEL2 Bit Field Definitions
//
#define SYSCLK_CLKSEL2_PWM67_S_Pos              6
#define SYSCLK_CLKSEL2_PWM67_S_Msk              (3ul << SYSCLK_CLKSEL2_PWM67_S_Pos)

#define SYSCLK_CLKSEL2_PWM45_S_Pos              4
#define SYSCLK_CLKSEL2_PWM45_S_Msk              (3ul << SYSCLK_CLKSEL2_PWM45_S_Pos)

#define SYSCLK_CLKSEL2_FRQDIV_S_Pos             2
#define SYSCLK_CLKSEL2_FRQDIV_S_Msk             (3ul << SYSCLK_CLKSEL2_FRQDIV_S_Pos)

//
//! SYSCLK CLKDIV Bit Field Definitions
//
#define SYSCLK_CLKDIV_ADC_N_Pos                 16
#define SYSCLK_CLKDIV_ADC_N_Msk                 (0xFFul << SYSCLK_CLKDIV_ADC_N_Pos)

#define SYSCLK_CLKDIV_UART_N_Pos                8
#define SYSCLK_CLKDIV_UART_N_Msk                (0xFul << SYSCLK_CLKDIV_UART_N_Pos)

#define SYSCLK_CLKDIV_HCLK_N_Pos                0
#define SYSCLK_CLKDIV_HCLK_N_Msk                (0xFul << SYSCLK_CLKDIV_HCLK_N_Pos)

//
//! SYSCLK PLLCON Bit Field Definitions
//
#define SYSCLK_PLLCON_PLL_SRC_Pos               19
#define SYSCLK_PLLCON_PLL_SRC_Msk               (1ul << SYSCLK_PLLCON_PLL_SRC_Pos)

#define SYSCLK_PLLCON_OE_Pos                    18
#define SYSCLK_PLLCON_OE_Msk                    (1ul << SYSCLK_PLLCON_OE_Pos)

#define SYSCLK_PLLCON_BP_Pos                    17
#define SYSCLK_PLLCON_BP_Msk                    (1ul << SYSCLK_PLLCON_BP_Pos)

#define SYSCLK_PLLCON_PD_Pos                    16
#define SYSCLK_PLLCON_PD_Msk                    (1ul << SYSCLK_PLLCON_PD_Pos)

#define SYSCLK_PLLCON_OUT_DV_Pos                14
#define SYSCLK_PLLCON_OUT_DV_Msk                (3ul << SYSCLK_PLLCON_OUT_DV_Pos)

#define SYSCLK_PLLCON_IN_DV_Pos                 9
#define SYSCLK_PLLCON_IN_DV_Msk                 (0x1Ful << SYSCLK_PLLCON_IN_DV_Pos)

#define SYSCLK_PLLCON_FB_DV_Pos                 0
#define SYSCLK_PLLCON_FB_DV_Msk                 (0x1FFul << SYSCLK_PLLCON_FB_DV_Pos)

//
//! SYSCLK FRQDIV Bit Field Definitions
//
#define SYSCLK_FRQDIV_FDIV_EN_Pos               4
#define SYSCLK_FRQDIV_FDIV_EN_Msk               (1ul << SYSCLK_FRQDIV_FDIV_EN_Pos)

#define SYSCLK_FRQDIV_FSEL_Pos                  0
#define SYSCLK_FRQDIV_FSEL_Msk                  (0xFul << SYSCLK_FRQDIV_FSEL_Pos)

//*****************************************************************************
//
//! Global Controller 
//
//*****************************************************************************
typedef __I uint32_t GCR_PDID_T;  

typedef struct
{
	__IO uint32_t  RSTS_POR:1;
	__IO uint32_t  RSTS_RESET:1;
	__IO uint32_t  RSTS_WDT:1;
	__IO uint32_t  RSTS_LVR:1;
	__IO uint32_t  RSTS_BOD:1;
	__IO uint32_t  RSTS_MCU:1;
	__I  uint32_t  RESERVE0:1;
	__IO uint32_t  RSTS_CPU:1;    
	__I  uint32_t  RESERVE1:24;
} GCR_RSTSRC_T;

typedef struct
{
    __IO uint32_t  CHIP_RST:1;
    __IO uint32_t  CPU_RST:1;
    __I  uint32_t  RESERVE0:1;
	__IO uint32_t  EBI_RST:1;
    __I  uint32_t  RESERVE1:28;
} GCR_IPRSTC1_T;

typedef struct
{
    __I  uint32_t  RESERVE0:1;
    __IO uint32_t  GPIO_RST:1;
    __IO uint32_t  TMR0_RST:1;
    __IO uint32_t  TMR1_RST:1;
    __IO uint32_t  TMR2_RST:1;
    __IO uint32_t  TMR3_RST:1;
    __I  uint32_t  RESERVE1:2;
    __IO uint32_t  I2C_RST:1;
    __I  uint32_t  RESERVE2:3;
    __IO uint32_t  SPI0_RST:1;
    __IO uint32_t  SPI1_RST:1;
    __I  uint32_t  RESERVE3:2;
	__IO uint32_t  UART0_RST:1;
    __IO uint32_t  UART1_RST:1;
    __I  uint32_t  RESERVE4:2;
    __IO uint32_t  PWM03_RST:1;
    __IO uint32_t  PWM47_RST:1;
    __I  uint32_t  RESERVE5:6;
    __IO uint32_t  ADC_RST:1;
    __I  uint32_t  RESERVE6:3;
} GCR_IPRSTC2_T;

typedef struct
{
    __IO uint32_t  BOD_EN:1;
    __IO uint32_t  BOD_VL:2;
    __IO uint32_t  BOD_RSTEN:1;
    __IO uint32_t  BOD_INTF:1;
    __IO uint32_t  BOD_LPM:1;
    __IO uint32_t  BOD_OUT:1;
	__IO uint32_t  LVR_EN:1;
    __I  uint32_t  RESERVE1:24;
} GCR_BODCR_T;

typedef __IO uint32_t GCR_PORCR_T;

typedef struct
{
	__IO uint32_t 	MFP0_AD0_CTS1:1;
	__IO uint32_t 	MFP1_AD1_RTS1:1;
	__IO uint32_t 	MFP2_AD2_CTS0:1;
	__IO uint32_t 	MFP3_AD3_RTS0:1;
	__IO uint32_t 	MFP4_AD4_SS1:1;
	__IO uint32_t 	MFP5_AD5_MOSI1:1;
	__IO uint32_t 	MFP6_AD6_MISO1:1;
	__IO uint32_t 	MFP7_AD7_SCLK1:1;
	__IO uint32_t 	ALT0_AD0_CTS1:1;
	__IO uint32_t 	ALT1_AD1_RTS1:1;
	__IO uint32_t 	ALT2_AD2_CTS0:1;
	__IO uint32_t 	ALT3_AD3_RTS0:1;
	__IO uint32_t 	ALT4_AD4_SS1:1;
	__IO uint32_t 	ALT5_AD5_MOSI1:1;
	__IO uint32_t 	ALT6_AD6_MISO1:1;
	__IO uint32_t 	ALT7_AD7_SCLK1:1;
	__IO uint32_t 	SCHMITT:8;
	__I  uint32_t 	RESERVE:8;    
} GCR_P0_MFP_T;

typedef struct
{
	__IO uint32_t 	MFP0_AIN0_T2:1;
	__IO uint32_t 	MFP1_AIN1_T3:1;
	__IO uint32_t 	MFP2_AIN2_RXD1:1;
	__IO uint32_t 	MFP3_AIN3_TXD1:1;
	__IO uint32_t 	MFP4_AIN4_SS0:1;
	__IO uint32_t 	MFP5_AIN5_MOSI0:1;
	__IO uint32_t 	MFP6_AIN6_MISO0:1;
	__IO uint32_t 	MFP7_AIN7_SCLK0:1;
	__IO uint32_t 	ALT0_AIN0_T2:1;
	__IO uint32_t 	ALT1_AIN1_T3:1;
	__IO uint32_t 	ALT2_AIN2_RXD1:1;
	__IO uint32_t 	ALT3_AIN3_TXD1:1;
	__IO uint32_t 	ALT4_AIN4_SS0:1;
	__IO uint32_t 	ALT5_AIN5_MOSI0:1;
	__IO uint32_t 	ALT6_AIN6_MISO0:1;
	__IO uint32_t 	ALT7_AIN7_SCLK0:1;
	__IO uint32_t 	SCHMITT:8;
	__I  uint32_t 	RESERVE:8;    
} GCR_P1_MFP_T;

typedef struct
{
	__IO uint32_t 	MFP0_AD8_PWM0:1;
	__IO uint32_t 	MFP1_AD9_PWM1:1;
	__IO uint32_t 	MFP2_AD10_PWM2:1;
	__IO uint32_t 	MFP3_AD11_PWM3:1;
	__IO uint32_t 	MFP4_AD12_PWM4:1;
	__IO uint32_t 	MFP5_AD13_PWM5:1;
	__IO uint32_t 	MFP6_AD14_PWM6:1;
	__IO uint32_t 	MFP7_AD15_PWM7:1;
	__IO uint32_t 	ALT0_AD8_PWM0:1;
	__IO uint32_t 	ALT1_AD9_PWM1:1;
	__IO uint32_t 	ALT2_AD10_PWM2:1;
	__IO uint32_t 	ALT3_AD11_PWM3:1;
	__IO uint32_t 	ALT4_AD12_PWM4:1;
	__IO uint32_t 	ALT5_AD13_PWM5:1;
	__IO uint32_t 	ALT6_AD14_PWM6:1;
	__IO uint32_t 	ALT7_AD15_PWM7:1;
	__IO uint32_t 	SCHMITT:8;
	__I  uint32_t 	RESERVE:8;    
} GCR_P2_MFP_T;

typedef struct
{
	__IO uint32_t 	MFP0_RXD0:1;
	__IO uint32_t 	MFP1_TXD0:1;
	__IO uint32_t 	MFP2_INT0:1;
	__IO uint32_t 	MFP3_INT1_MCLK:1;
	__IO uint32_t 	MFP4_T0_SDA:1;
	__IO uint32_t 	MFP5_T1_SCL:1;
	__IO uint32_t 	MFP6_WR_CKO:1;
	__IO uint32_t 	MFP7_RD:1;
	__IO uint32_t 	ALT0_RXD0:1;
	__IO uint32_t 	ALT1_TXD0:1;
	__IO uint32_t 	ALT2_INT0:1;
	__IO uint32_t 	ALT3_INT1_MCLK:1;
	__IO uint32_t 	ALT4_T0_SDA:1;
	__IO uint32_t 	ALT5_T1_SCL:1;
	__IO uint32_t 	ALT6_WR_CKO:1;
	__IO uint32_t 	ALT7_RD:1;
	__IO uint32_t 	SCHMITT:8;
	__I  uint32_t 	RESERVE:8;    
} GCR_P3_MFP_T;

typedef struct
{
	__IO uint32_t 	MFP0_PWM0:1;
	__IO uint32_t 	MFP1_PWM1:1;
	__IO uint32_t 	MFP2_PWM2:1;
	__IO uint32_t 	MFP3_PWM3:1;
	__IO uint32_t 	MFP4_CS:1;
	__IO uint32_t 	MFP5_ALE:1;
	__IO uint32_t 	MFP6_ICE_CLK:1;
	__IO uint32_t 	MFP7_ICE_DAT:1;
	__IO uint32_t 	ALT0_PWM0:1;
	__IO uint32_t 	ALT1_PWM1:1;
	__IO uint32_t 	ALT2_PWM2:1;
	__IO uint32_t 	ALT3_PWM3:1;
	__IO uint32_t 	ALT4_CS:1;
	__IO uint32_t 	ALT5_ALE:1;
	__IO uint32_t 	ALT6_ICE_CLK:1;
	__IO uint32_t 	ALT7_ICE_DAT:1;
	__IO uint32_t 	SCHMITT:8;
	__I  uint32_t 	RESERVE:8;    
} GCR_P4_MFP_T;

typedef __IO uint32_t GCR_REGWRPROT_T;

typedef struct
{
    union 
    {
        __I uint32_t u32PDID;
        __I uint32_t PDID;
    };

    union 
    {
        __IO uint32_t u32RSTSRC;
        
        struct 
        {
        	__IO uint32_t  RSTS_POR:1;
        	__IO uint32_t  RSTS_RESET:1;
        	__IO uint32_t  RSTS_WDT:1;
        	__IO uint32_t  RSTS_LVR:1;
        	__IO uint32_t  RSTS_BOD:1;
        	__IO uint32_t  RSTS_MCU:1;
        	__I  uint32_t  RESERVE0:1;
        	__IO uint32_t  RSTS_CPU:1;    
        	__I  uint32_t  RESERVE1:24;
        } RSTSRC;
    };

    union 
    {
        __IO uint32_t u32IPRSTC1;
        
        struct 
        {
            __IO uint32_t  CHIP_RST:1;
            __IO uint32_t  CPU_RST:1;
            __I  uint32_t  RESERVE0:1;
        	__IO uint32_t  EBI_RST:1;
            __I  uint32_t  RESERVE1:28;
        } IPRSTC1;
    };    
    
    union 
    {
        __IO uint32_t u32IPRSTC2;
        
        struct 
        {
            __I  uint32_t  RESERVE0:1;
            __IO uint32_t  GPIO_RST:1;
            __IO uint32_t  TMR0_RST:1;
            __IO uint32_t  TMR1_RST:1;
            __IO uint32_t  TMR2_RST:1;
            __IO uint32_t  TMR3_RST:1;
            __I  uint32_t  RESERVE1:2;
            __IO uint32_t  I2C_RST:1;
            __I  uint32_t  RESERVE2:3;
            __IO uint32_t  SPI0_RST:1;
            __IO uint32_t  SPI1_RST:1;
            __I  uint32_t  RESERVE3:2;
        	__IO uint32_t  UART0_RST:1;
            __IO uint32_t  UART1_RST:1;
            __I  uint32_t  RESERVE4:2;
            __IO uint32_t  PWM03_RST:1;
            __IO uint32_t  PWM47_RST:1;
            __I  uint32_t  RESERVE5:6;
            __IO uint32_t  ADC_RST:1;
            __I  uint32_t  RESERVE6:3;
        } IPRSTC2;
    };    

    uint32_t RESERVE0[2];
    
    union 
    {
        __IO uint32_t u32BODCR;
        
        struct 
        {
            __IO uint32_t  BOD_EN:1;
            __IO uint32_t  BOD_VL:2;
            __IO uint32_t  BOD_RSTEN:1;
            __IO uint32_t  BOD_INTF:1;
            __IO uint32_t  BOD_LPM:1;
            __IO uint32_t  BOD_OUT:1;
            __IO uint32_t  LVR_EN:1;
            __I  uint32_t  RESERVE1:24;
        } BODCR;
    };

    uint32_t RESERVE1[2];
    
    union 
    {
        __IO uint32_t u32PORCR;
        __IO uint32_t PORCR;
    };   

    uint32_t RESERVE2[2];

    union 
    {
        __IO uint32_t u32P0_MFP;
        
        struct 
        {
        	__IO uint32_t 	MFP0_AD0_CTS1:1;
        	__IO uint32_t 	MFP1_AD1_RTS1:1;
        	__IO uint32_t 	MFP2_AD2_CTS0:1;
        	__IO uint32_t 	MFP3_AD3_RTS0:1;
        	__IO uint32_t 	MFP4_AD4_SS1:1;
        	__IO uint32_t 	MFP5_AD5_MOSI1:1;
        	__IO uint32_t 	MFP6_AD6_MISO1:1;
        	__IO uint32_t 	MFP7_AD7_SCLK1:1;
        	__IO uint32_t 	ALT0_AD0_CTS1:1;
        	__IO uint32_t 	ALT1_AD1_RTS1:1;
        	__IO uint32_t 	ALT2_AD2_CTS0:1;
        	__IO uint32_t 	ALT3_AD3_RTS0:1;
        	__IO uint32_t 	ALT4_AD4_SS1:1;
        	__IO uint32_t 	ALT5_AD5_MOSI1:1;
        	__IO uint32_t 	ALT6_AD6_MISO1:1;
        	__IO uint32_t 	ALT7_AD7_SCLK1:1;
        	__IO uint32_t 	SCHMITT:8;
        	__I  uint32_t 	RESERVE:8;    
        } P0_MFP;
    };   
    
    union 
    {
        __IO uint32_t u32P1_MFP;
        
        struct 
        {
        	__IO uint32_t 	MFP0_AIN0_T2:1;
        	__IO uint32_t 	MFP1_AIN1_T3:1;
        	__IO uint32_t 	MFP2_AIN2_RXD1:1;
        	__IO uint32_t 	MFP3_AIN3_TXD1:1;
        	__IO uint32_t 	MFP4_AIN4_SS0:1;
        	__IO uint32_t 	MFP5_AIN5_MOSI0:1;
        	__IO uint32_t 	MFP6_AIN6_MISO0:1;
        	__IO uint32_t 	MFP7_AIN7_SCLK0:1;
        	__IO uint32_t 	ALT0_AIN0_T2:1;
        	__IO uint32_t 	ALT1_AIN1_T3:1;
        	__IO uint32_t 	ALT2_AIN2_RXD1:1;
        	__IO uint32_t 	ALT3_AIN3_TXD1:1;
        	__IO uint32_t 	ALT4_AIN4_SS0:1;
        	__IO uint32_t 	ALT5_AIN5_MOSI0:1;
        	__IO uint32_t 	ALT6_AIN6_MISO0:1;
        	__IO uint32_t 	ALT7_AIN7_SCLK0:1;
        	__IO uint32_t 	SCHMITT:8;
        	__I  uint32_t 	RESERVE:8;    
        } P1_MFP;
    };   
    
    union 
    {
        __IO uint32_t u32P2_MFP;
        
        struct 
        {
        	__IO uint32_t 	MFP0_AD8_PWM0:1;
        	__IO uint32_t 	MFP1_AD9_PWM1:1;
        	__IO uint32_t 	MFP2_AD10_PWM2:1;
        	__IO uint32_t 	MFP3_AD11_PWM3:1;
        	__IO uint32_t 	MFP4_AD12_PWM4:1;
        	__IO uint32_t 	MFP5_AD13_PWM5:1;
        	__IO uint32_t 	MFP6_AD14_PWM6:1;
        	__IO uint32_t 	MFP7_AD15_PWM7:1;
        	__IO uint32_t 	ALT0_AD8_PWM0:1;
        	__IO uint32_t 	ALT1_AD9_PWM1:1;
        	__IO uint32_t 	ALT2_AD10_PWM2:1;
        	__IO uint32_t 	ALT3_AD11_PWM3:1;
        	__IO uint32_t 	ALT4_AD12_PWM4:1;
        	__IO uint32_t 	ALT5_AD13_PWM5:1;
        	__IO uint32_t 	ALT6_AD14_PWM6:1;
        	__IO uint32_t 	ALT7_AD15_PWM7:1;
        	__IO uint32_t 	SCHMITT:8;
        	__I  uint32_t 	RESERVE:8;    
        } P2_MFP;
    };   
    
    union 
    {
        __IO uint32_t u32P3_MFP;
        
        struct 
        {
        	__IO uint32_t 	MFP0_RXD0:1;
        	__IO uint32_t 	MFP1_TXD0:1;
        	__IO uint32_t 	MFP2_INT0:1;
        	__IO uint32_t 	MFP3_INT1_MCLK:1;
        	__IO uint32_t 	MFP4_T0_SDA:1;
        	__IO uint32_t 	MFP5_T1_SCL:1;
        	__IO uint32_t 	MFP6_WR_CKO:1;
        	__IO uint32_t 	MFP7_RD:1;
        	__IO uint32_t 	ALT0_RXD0:1;
        	__IO uint32_t 	ALT1_TXD0:1;
        	__IO uint32_t 	ALT2_INT0:1;
        	__IO uint32_t 	ALT3_INT1_MCLK:1;
        	__IO uint32_t 	ALT4_T0_SDA:1;
        	__IO uint32_t 	ALT5_T1_SCL:1;
        	__IO uint32_t 	ALT6_WR_CKO:1;
        	__IO uint32_t 	ALT7_RD:1;
        	__IO uint32_t 	SCHMITT:8;
        	__I  uint32_t 	RESERVE:8;    
        } P3_MFP;
    };   
    
    union 
    {
        __IO uint32_t u32P4_MFP;
        
        struct 
        {
        	__IO uint32_t 	MFP0_PWM0:1;
        	__IO uint32_t 	MFP1_PWM1:1;
        	__IO uint32_t 	MFP2_PWM2:1;
        	__IO uint32_t 	MFP3_PWM3:1;
        	__IO uint32_t 	MFP4_CS:1;
        	__IO uint32_t 	MFP5_ALE:1;
        	__IO uint32_t 	MFP6_ICE_CLK:1;
        	__IO uint32_t 	MFP7_ICE_DAT:1;
        	__IO uint32_t 	ALT0_PWM0:1;
        	__IO uint32_t 	ALT1_PWM1:1;
        	__IO uint32_t 	ALT2_PWM2:1;
        	__IO uint32_t 	ALT3_PWM3:1;
        	__IO uint32_t 	ALT4_CS:1;
        	__IO uint32_t 	ALT5_ALE:1;
        	__IO uint32_t 	ALT6_ICE_CLK:1;
        	__IO uint32_t 	ALT7_ICE_DAT:1;
        	__IO uint32_t 	SCHMITT:8;
        	__I  uint32_t 	RESERVE:8;  
        } P4_MFP;
    };                   
    
    uint32_t RESERVE3[47];
         
    union 
    {
        __IO uint32_t u32REGWRPROT;
        __IO uint32_t REGWRPROT;
    };  
} GCR_T;

//
//! GCR RSTSRC Bit Field Definitions
//
#define GCR_RSTSRC_RSTS_CPU_Pos                 7
#define GCR_RSTSRC_RSTS_CPU_Msk                 (1ul << GCR_RSTSRC_RSTS_CPU_Pos)

#define GCR_RSTSRC_RSTS_MCU_Pos                 5
#define GCR_RSTSRC_RSTS_MCU_Msk                 (1ul << GCR_RSTSRC_RSTS_SYS_Pos)

#define GCR_RSTSRC_RSTS_BOD_Pos                 4
#define GCR_RSTSRC_RSTS_BOD_Msk                 (1ul << GCR_RSTSRC_RSTS_BOD_Pos)

#define GCR_RSTSRC_RSTS_LVR_Pos                 3
#define GCR_RSTSRC_RSTS_LVR_Msk                 (1ul << GCR_RSTSRC_RSTS_LVR_Pos)

#define GCR_RSTSRC_RSTS_WDT_Pos                 2
#define GCR_RSTSRC_RSTS_WDT_Msk                 (1ul << GCR_RSTSRC_RSTS_WDT_Pos)

#define GCR_RSTSRC_RSTS_RESET_Pos               1
#define GCR_RSTSRC_RSTS_RESET_Msk               (1ul << GCR_RSTSRC_RSTS_RESET_Pos)

#define GCR_RSTSRC_RSTS_POR_Pos                 0
#define GCR_RSTSRC_RSTS_POR_Msk                 (1ul << GCR_RSTSRC_RSTS_POR_Pos)

//
//! GCR IPRSTC1 Bit Field Definitions
//
#define GCR_IPRSTC1_EBI_RST_Pos                 3
#define GCR_IPRSTC1_EBI_RST_Msk                 (1ul << GCR_IPRSTC1_EBI_RST_Pos)

#define GCR_IPRSTC1_CPU_RST_Pos                 1
#define GCR_IPRSTC1_CPU_RST_Msk                 (1ul << GCR_IPRSTC1_CPU_RST_Pos)

#define GCR_IPRSTC1_CHIP_RST_Pos                0
#define GCR_IPRSTC1_CHIP_RST_Msk                (1ul << GCR_IPRSTC1_CHIP_RST_Pos)

//
//! GCR IPRSTC2 Bit Field Definitions
//
#define GCR_IPRSTC2_ADC_RST_Pos                 28
#define GCR_IPRSTC2_ADC_RST_Msk                 (1ul << GCR_IPRSTC2_ADC_RST_Pos)

#define GCR_IPRSTC2_PWM47_RST_Pos               21
#define GCR_IPRSTC2_PWM47_RST_Msk               (1ul << GCR_IPRSTC2_PWM47_RST_Pos)

#define GCR_IPRSTC2_PWM03_RST_Pos               20
#define GCR_IPRSTC2_PWM03_RST_Msk               (1ul << GCR_IPRSTC2_PWM03_RST_Pos)

#define GCR_IPRSTC2_UART1_RST_Pos               17
#define GCR_IPRSTC2_UART1_RST_Msk               (1ul << GCR_IPRSTC2_UART1_RST_Pos)

#define GCR_IPRSTC2_UART0_RST_Pos               16
#define GCR_IPRSTC2_UART0_RST_Msk               (1ul << GCR_IPRSTC2_UART0_RST_Pos)

#define GCR_IPRSTC2_SPI1_RST_Pos                13
#define GCR_IPRSTC2_SPI1_RST_Msk                (1ul << GCR_IPRSTC2_SPI1_RST_Pos)

#define GCR_IPRSTC2_SPI0_RST_Pos                12
#define GCR_IPRSTC2_SPI0_RST_Msk                (1ul << GCR_IPRSTC2_SPI0_RST_Pos)

#define GCR_IPRSTC2_I2C_RST_Pos                 8
#define GCR_IPRSTC2_I2C_RST_Msk                 (1ul << GCR_IPRSTC2_I2C0_RST_Pos)

#define GCR_IPRSTC2_TMR3_RST_Pos                5
#define GCR_IPRSTC2_TMR3_RST_Msk                (1ul << GCR_IPRSTC2_TMR3_RST_Pos)

#define GCR_IPRSTC2_TMR2_RST_Pos                4
#define GCR_IPRSTC2_TMR2_RST_Msk                (1ul << GCR_IPRSTC2_TMR2_RST_Pos)

#define GCR_IPRSTC2_TMR1_RST_Pos                3
#define GCR_IPRSTC2_TMR1_RST_Msk                (1ul << GCR_IPRSTC2_TMR1_RST_Pos)

#define GCR_IPRSTC2_TMR0_RST_Pos                2
#define GCR_IPRSTC2_TMR0_RST_Msk                (1ul << GCR_IPRSTC2_TMR0_RST_Pos)

#define GCR_IPRSTC2_GPIO_RST_Pos                1
#define GCR_IPRSTC2_GPIO_RST_Msk                (1ul << GCR_IPRSTC2_GPIO_RST_Pos)

//
//! GCR BODCR Bit Field Definitions
//
#define GCR_BODCR_LVR_EN_Pos                    7
#define GCR_BODCR_LVR_EN_Msk                    (1ul << GCR_BODCR_LVR_EN_Pos)

#define GCR_BODCR_BOD_OUT_Pos                   6
#define GCR_BODCR_BOD_OUT_Msk                   (1ul << GCR_BODCR_BOD_OUT_Pos)

#define GCR_BODCR_BOD_LPM_Pos                   5
#define GCR_BODCR_BOD_LPM_Msk                   (1ul << GCR_BODCR_BOD_LPM_Pos)

#define GCR_BODCR_BOD_INTF_Pos                  4
#define GCR_BODCR_BOD_INTF_Msk                  (1ul << GCR_BODCR_BOD_INTF_Pos)

#define GCR_BODCR_BOD_RSTEN_Pos                 3
#define GCR_BODCR_BOD_RSTEN_Msk                 (1ul << GCR_BODCR_BOD_RSTEN_Pos)

#define GCR_BODCR_BOD_VL_Pos                    1
#define GCR_BODCR_BOD_VL_Msk                    (3ul << GCR_BODCR_BOD_VL_Pos)

#define GCR_BODCR_BOD_EN_Pos                    0
#define GCR_BODCR_BOD_EN_Msk                    (1ul << GCR_BODCR_BOD_EN_Pos)

//
//! GCR PORCR Bit Field Definitions
//
#define GCR_PORCR_POR_DIS_CODE_Pos              0
#define GCR_PORCR_POR_DIS_CODE_Msk              (0xFFFFul << GCR_PORCR_POR_DIS_CODE_Pos)

//
//! GCR P0_MFP Bit Field Definitions
//
#define GCR_P0_MFP_P0_TYPE_Pos                  16
#define GCR_P0_MFP_P0_TYPE_Msk                  (0xFFul << GCR_P0_MFP_P0_TYPE_Pos)

#define GCR_P0_MFP_P0_ALT_Pos                   8
#define GCR_P0_MFP_P0_ALT_Msk                   (0xFFul << GCR_P0_MFP_P0_ALT_Pos)

#define GCR_P0_MFP_P0_MFP_Pos                   0
#define GCR_P0_MFP_P0_MFP_Msk                   (0xFFul << GCR_P0_MFP_P0_MFP_Pos)

//
//! GCR P1_MFP Bit Field Definitions
//
#define GCR_P1_MFP_P1_TYPE_Pos                  16
#define GCR_P1_MFP_P1_TYPE_Msk                  (0xFFul << GCR_P1_MFP_P1_TYPE_Pos)

#define GCR_P1_MFP_P1_ALT_Pos                   8
#define GCR_P1_MFP_P1_ALT_Msk                   (0xFFul << GCR_P1_MFP_P1_ALT_Pos)

#define GCR_P1_MFP_P1_MFP_Pos                   0
#define GCR_P1_MFP_P1_MFP_Msk                   (0xFFul << GCR_P1_MFP_P1_MFP_Pos)

//
//! GCR P2_MFP Bit Field Definitions
//
#define GCR_P2_MFP_P2_TYPE_Pos                  16
#define GCR_P2_MFP_P2_TYPE_Msk                  (0xFFul << GCR_P2_MFP_P2_TYPE_Pos)

#define GCR_P2_MFP_P2_ALT_Pos                   8
#define GCR_P2_MFP_P2_ALT_Msk                   (0xFFul << GCR_P2_MFP_P2_ALT_Pos)

#define GCR_P2_MFP_P2_MFP_Pos                   0
#define GCR_P2_MFP_P2_MFP_Msk                   (0xFFul << GCR_P2_MFP_P2_MFP_Pos)

//
//! GCR P3_MFP Bit Field Definitions
//
#define GCR_P3_MFP_P3_TYPE_Pos                  16
#define GCR_P3_MFP_P3_TYPE_Msk                  (0xFFul << GCR_P3_MFP_P3_TYPE_Pos)

#define GCR_P3_MFP_P3_ALT_Pos                   8
#define GCR_P3_MFP_P3_ALT_Msk                   (0xFFul << GCR_P3_MFP_P3_ALT_Pos)

#define GCR_P3_MFP_P3_MFP_Pos                   0
#define GCR_P3_MFP_P3_MFP_Msk                   (0xFFul << GCR_P3_MFP_P3_MFP_Pos)

//
//! GCR P4_MFP Bit Field Definitions
//
#define GCR_P4_MFP_P4_TYPE_Pos                  16
#define GCR_P4_MFP_P4_TYPE_Msk                  (0xFFul << GCR_P4_MFP_P4_TYPE_Pos)

#define GCR_P4_MFP_P4_ALT_Pos                   8
#define GCR_P4_MFP_P4_ALT_Msk                   (0xFFul << GCR_P4_MFP_P4_ALT_Pos)

#define GCR_P4_MFP_P4_MFP_Pos                   0
#define GCR_P4_MFP_P4_MFP_Msk                   (0xFFul << GCR_P4_MFP_P4_MFP_Pos)

//
//! GCR REGWRPROT Bit Field Definitions
//
#define GCR_REGWRPROT_REGWRPROT_Pos             0
#define GCR_REGWRPROT_REGWRPROT_Msk             (0xFFul << GCR_REGWRPROT_REGWRPROT_Pos)


typedef struct
{
    __IO uint32_t  INTSRC:4;
    __I  uint32_t  RESERVE:28;
} GCR_IRQSRC_T;

typedef struct
{
    __IO uint32_t  NMISEL:5;
    __I  uint32_t  RESERVE:27;
} GCR_NMISEL_T;

typedef __IO uint32_t GCR_MCUIRQ_T;

typedef struct
{
    union 
    {
        __I uint32_t u32IRQSRC[32];
        __I uint32_t IRQSRC[32];
    };

    union 
    {
        __IO uint32_t u32NMISEL;
        
        struct 
        {
            __IO uint32_t  NMISEL:5;
            __I  uint32_t  RESERVE:27;
        } NMISEL;
    };

    union 
    {
        __IO uint32_t u32MCUIRQ;
        __IO uint32_t MCUIRQ;
    };
} GCR_INT_T;

//*****************************************************************************
//
//! FLASH Memory Controller
//
//*****************************************************************************
typedef struct
{
    __IO uint32_t  ISPEN:1;
    __IO uint32_t  BS:1;
	__I  uint32_t  RESERVE0:2;
    __IO uint32_t  CFGUEN:1;
	__IO uint32_t  LDUEN:1;
    __IO uint32_t  ISPFF:1;
    __IO uint32_t  SWRST:1;
    __IO uint32_t  PT:3;
	__I  uint32_t  RESERVE1:1;
	__IO uint32_t  ET:3;
    __I  uint32_t  RESERVE2:17;

} FMC_ISPCON_T;

typedef __IO uint32_t FMC_ISPADR_T;
typedef __IO uint32_t FMC_ISPDAT_T;

typedef struct
{
    __IO uint32_t  FCTRL:4;
    __IO uint32_t  FCEN:1;
    __IO uint32_t  FOEN:1;
    __I  uint32_t  RESERVE:26;
} FMC_ISPCMD_T;

typedef struct
{
    __IO uint32_t  ISPGO:1;
    __I  uint32_t  RESERVE:31;
} FMC_ISPTRG_T;

typedef __I uint32_t FMC_DFBADR_T;

typedef struct
{
    __IO uint32_t  FPSEN:1;
    __IO uint32_t  FATS:3;
    __IO uint32_t  LFOM:1;
	__I  uint32_t  RESERVE:27;
} FMC_FATCON_T;

typedef struct
{
    union 
    {
        __IO uint32_t u32ISPCON;
        
        struct 
        {
            __IO uint32_t  ISPEN:1;
            __IO uint32_t  BS:1;
        	__I  uint32_t  RESERVE0:2;
            __IO uint32_t  CFGUEN:1;
        	__IO uint32_t  LDUEN:1;
            __IO uint32_t  ISPFF:1;
            __IO uint32_t  SWRST:1;
            __IO uint32_t  PT:3;
        	__I  uint32_t  RESERVE1:1;
        	__IO uint32_t  ET:3;
            __I  uint32_t  RESERVE2:17;
        } ISPCON;
    };

    union 
    {
        __IO uint32_t u32ISPADR;
        __IO uint32_t ISPADR;
    };

    union 
    {
        __IO uint32_t u32ISPDAT;
        __IO uint32_t ISPDAT;
    };
    
    union 
    {
        __IO uint32_t u32ISPCMD;
        
        struct 
        {
            __IO uint32_t  FCTRL:4;
            __IO uint32_t  FCEN:1;
            __IO uint32_t  FOEN:1;
            __I  uint32_t  RESERVE:26;
        } ISPCMD;
    };

    union 
    {
        __I uint32_t u32ISPTRG;
        
        struct 
        {
            __IO uint32_t  ISPGO:1;
            __I  uint32_t  RESERVE:31;
        } ISPTRG;
    };
    
    union 
    {
        __I uint32_t u32DFBADR;
        __I uint32_t DFBADR;
    };

    union 
    {
        __IO uint32_t u32FATCON;
        
        struct 
        {
            __IO uint32_t  FPSEN:1;
            __IO uint32_t  FATS:3;
            __IO uint32_t  LFOM:1;
            __I  uint32_t  RESERVE:27;
        } FATCON;
    };
} FMC_T;

//
//! FMC ISPCON Bit Field Definitions
//
#define FMC_ISPCON_ET_Pos                       12
#define FMC_ISPCON_ET_Msk                       (7ul << FMC_ISPCON_ET_Pos)

#define FMC_ISPCON_PT_Pos                       8
#define FMC_ISPCON_PT_Msk                       (7ul << FMC_ISPCON_PT_Pos)

#define FMC_ISPCON_ISPFF_Pos                    6
#define FMC_ISPCON_ISPFF_Msk                    (1ul << FMC_ISPCON_ISPFF_Pos)

#define FMC_ISPCON_LDUEN_Pos                    5
#define FMC_ISPCON_LDUEN_Msk                    (1ul << FMC_ISPCON_LDUEN_Pos)

#define FMC_ISPCON_CFGUEN_Pos                   4
#define FMC_ISPCON_CFGUEN_Msk                   (1ul << FMC_ISPCON_CFGUEN_Pos)

#define FMC_ISPCON_BS_Pos                       1
#define FMC_ISPCON_BS_Msk                       (1ul << FMC_ISPCON_BS_Pos)

#define FMC_ISPCON_ISPEN_Pos                    0
#define FMC_ISPCON_ISPEN_Msk                    (1ul << FMC_ISPCON_ISPEN_Pos)

//
//! FMC ISPADR Bit Field Definitions
//
#define FMC_ISPADR_ISPADR_Pos                   0
#define FMC_ISPADR_ISPADR_Msk                   (0xFFFFFFFFul << FMC_ISPADR_ISPADR_Pos)

//
//! FMC ISPADR Bit Field Definitions
//
#define FMC_ISPDAT_ISPDAT_Pos                   0
#define FMC_ISPDAT_ISPDAT_Msk                   (0xFFFFFFFFul << FMC_ISPDAT_ISPDAT_Pos)

//
//! FMC ISPCMD Bit Field Definitions 
//
#define FMC_ISPCMD_FOEN_Pos                     5
#define FMC_ISPCMD_FOEN_Msk                     (1ul << FMC_ISPCMD_FOEN_Pos)

#define FMC_ISPCMD_FCEN_Pos                     4
#define FMC_ISPCMD_FCEN_Msk                     (1ul << FMC_ISPCMD_FCEN_Pos)

#define FMC_ISPCMD_FCTRL_Pos                    0
#define FMC_ISPCMD_FCTRL_Msk                    (0xFul << FMC_ISPCMD_FCTRL_Pos)

//
//! FMC ISPTRG Bit Field Definitions 
//
#define FMC_ISPTRG_ISPGO_Pos                    0
#define FMC_ISPTRG_ISPGO_Msk                    (1ul << FMC_ISPTRG_ISPGO_Pos)

//
//! FMC DFBADR Bit Field Definitions
//
#define FMC_DFBADR_DFBA_Pos                     0
#define FMC_DFBADR_DFBA_Msk                     (0xFFFFFFFFul << FMC_DFBADR_DFBA_Pos)

//
//! FMC FATCON Bit Field Definitions
//
#define FMC_FATCON_LFOM_Pos                     4
#define FMC_FATCON_LFOM_Msk                     (1ul << FMC_FATCON_LFOM_Pos)

#define FMC_FATCON_FATS_Pos                     1
#define FMC_FATCON_FATS_Msk                     (7ul << FMC_FATCON_FATS_Pos)

#define FMC_FATCON_FPSEN_Pos                    0
#define FMC_FATCON_FPSEN_Msk                    (1ul << FMC_FATCON_FPSEN_Pos)

//*****************************************************************************
//
//! PWM Controller
//
//*****************************************************************************
typedef struct
{
    __IO uint32_t  CP01:8;
    __IO uint32_t  CP23:8;
    __IO uint32_t  DZI01:8;
    __IO uint32_t  DZI23:8;
} PWM_PPR_T;

typedef struct
{
    __IO uint32_t  CSR0:3;
    __I  uint32_t  RESERVE0:1;
    __IO uint32_t  CSR1:3;
    __I  uint32_t  RESERVE1:1;
    __IO uint32_t  CSR2:3;
    __I  uint32_t  RESERVE2:1;
    __IO uint32_t  CSR3:3;
    __I  uint32_t  RESERVE3:17;
} PWM_CSR_T;

typedef struct
{
    __IO uint32_t  CH0EN:1;
    __I  uint32_t  RESERVE0:1;
    __IO uint32_t  CH0INV:1;
    __IO uint32_t  CH0MOD:1;
    __IO uint32_t  DZEN01:1;
    __IO uint32_t  DZEN23:1;
    __I  uint32_t  RESERVE1:2;
    __IO uint32_t  CH1EN:1;
    __I  uint32_t  RESERVE2:1;
    __IO uint32_t  CH1INV:1;
    __IO uint32_t  CH1MOD:1;
    __I  uint32_t  RESERVE3:4;
    __IO uint32_t  CH2EN:1;
    __I  uint32_t  RESERVE4:1;
    __IO uint32_t  CH2INV:1;
    __IO uint32_t  CH2MOD:1;
    __I  uint32_t  RESERVE5:4;
    __IO uint32_t  CH3EN:1;
    __I  uint32_t  RESERVE6:1;
    __IO uint32_t  CH3INV:1;
    __IO uint32_t  CH3MOD:1;
    __I  uint32_t  RESERVE7:4;   
} PWM_PCR_T;

typedef __IO uint32_t PWM_CNR_T;

typedef __IO uint32_t PWM_CMR_T;

typedef __I uint32_t PWM_PDR_T;

typedef struct
{
    __IO uint32_t  PWMIE0:1;
    __IO uint32_t  PWMIE1:1;
    __IO uint32_t  PWMIE2:1;
    __IO uint32_t  PWMIE3:1;
	__I  uint32_t  RESERVE:28;
} PWM_PIER_T;

typedef struct
{
    __IO uint32_t  PWMIF0:1;
    __IO uint32_t  PWMIF1:1;
    __IO uint32_t  PWMIF2:1;
    __IO uint32_t  PWMIF3:1;
	__I  uint32_t  RESERVE:28;
} PWM_PIIR_T;

typedef struct
{
    __IO uint32_t  INV0:1;
    __IO uint32_t  CRL_IE0:1;
    __IO uint32_t  CFL_IE0:1;
    __IO uint32_t  CAPCH0EN:1;
    __IO uint32_t  CAPIF0:1;
    __I  uint32_t  RESERVE0:1;
    __IO uint32_t  CRLRI0:1;
    __IO uint32_t  CFLRI0:1;
    __I  uint32_t  RESERVE1:8;
    __IO uint32_t  INV1:1;
    __IO uint32_t  CRL_IE1:1;
    __IO uint32_t  CFL_IE1:1;
    __IO uint32_t  CAPCH1EN:1;
    __IO uint32_t  CAPIF1:1;
    __I  uint32_t  RESERVE2:1;
    __IO uint32_t  CRLRI1:1;
    __IO uint32_t  CFLRI1:1;
    __I  uint32_t  RESERVE3:8;
} PWM_CCR0_T;

typedef struct
{
    __IO uint32_t  INV2:1;
    __IO uint32_t  CRL_IE2:1;
    __IO uint32_t  CFL_IE2:1;
    __IO uint32_t  CAPCH2EN:1;
    __IO uint32_t  CAPIF2:1;
    __I  uint32_t  RESERVE0:1;
    __IO uint32_t  CRLRI2:1;
    __IO uint32_t  CFLRI2:1;
    __I  uint32_t  RESERVE1:8;
    __IO uint32_t  INV3:1;
    __IO uint32_t  CRL_IE3:1;
    __IO uint32_t  CFL_IE3:1;
    __IO uint32_t  CAPCH3EN:1;
    __IO uint32_t  CAPIF3:1;
    __I  uint32_t  RESERVE2:1;
    __IO uint32_t  CRLRI3:1;
    __IO uint32_t  CFLRI3:1;
    __I  uint32_t  RESERVE3:8;
} PWM_CCR2_T;

typedef __IO uint32_t PWM_CRLR_T;

typedef __IO uint32_t PWM_CFLR_T;

typedef __IO uint32_t PWM_CAPENR_T;

typedef struct
{
    __IO uint32_t  PWM0:1;
    __IO uint32_t  PWM1:1;
    __IO uint32_t  PWM2:1;
    __IO uint32_t  PWM3:1;
	__I  uint32_t  RESERVE:28;
} PWM_POE_T;


typedef struct
{
    union 
    {
        __IO uint32_t u32PPR;
        
        struct 
        {
            __IO uint32_t  CP01:8;
            __IO uint32_t  CP23:8;
            __IO uint32_t  DZI01:8;
            __IO uint32_t  DZI23:8;
        } PPR;
    };
    
    union 
    {
        __IO uint32_t u32CSR;
        
        struct 
        {
            __IO uint32_t  CSR0:3;
            __I  uint32_t  RESERVE0:1;
            __IO uint32_t  CSR1:3;
            __I  uint32_t  RESERVE1:1;
            __IO uint32_t  CSR2:3;
            __I  uint32_t  RESERVE2:1;
            __IO uint32_t  CSR3:3;
            __I  uint32_t  RESERVE3:17;
        } CSR;
    };

    union 
    {
        __IO uint32_t u32PCR;
        
        struct 
        {
            __IO uint32_t  CH0EN:1;
            __I  uint32_t  RESERVE0:1;
            __IO uint32_t  CH0INV:1;
            __IO uint32_t  CH0MOD:1;
            __IO uint32_t  DZEN01:1;
            __IO uint32_t  DZEN23:1;
            __I  uint32_t  RESERVE1:2;
            __IO uint32_t  CH1EN:1;
            __I  uint32_t  RESERVE2:1;
            __IO uint32_t  CH1INV:1;
            __IO uint32_t  CH1MOD:1;
            __I  uint32_t  RESERVE3:4;
            __IO uint32_t  CH2EN:1;
            __I  uint32_t  RESERVE4:1;
            __IO uint32_t  CH2INV:1;
            __IO uint32_t  CH2MOD:1;
            __I  uint32_t  RESERVE5:4;
            __IO uint32_t  CH3EN:1;
            __I  uint32_t  RESERVE6:1;
            __IO uint32_t  CH3INV:1;
            __IO uint32_t  CH3MOD:1;
            __I  uint32_t  RESERVE7:4;   
        } PCR;
    };

    union 
    {
        __IO uint32_t u32CNR0;
        __IO uint32_t CNR0;
    };

    union 
    {
        __IO uint32_t u32CMR0;
        __IO uint32_t CMR0;
    };

    union 
    {
        __I  uint32_t u32PDR0;
        __I  uint32_t PDR0;
    };
    
    union 
    {
        __IO uint32_t u32CNR1;
        __IO uint32_t CNR1;
    };

    union 
    {
        __IO uint32_t u32CMR1;
        __IO uint32_t CMR1;
    };

    union 
    {
        __I  uint32_t u32PDR1;
        __I  uint32_t PDR1;
    };

    union 
    {
        __IO uint32_t u32CNR2;
        __IO uint32_t CNR2;
    };

    union 
    {
        __IO uint32_t u32CMR2;
        __IO uint32_t CMR2;
    };

    union 
    {
        __I  uint32_t u32PDR2;
        __I  uint32_t PDR2;
    };

    union 
    {
        __IO uint32_t u32CNR3;
        __IO uint32_t CNR3;
    };

    union 
    {
        __IO uint32_t u32CMR3;
        __IO uint32_t CMR3;
    };

    union 
    {
        __I  uint32_t u32PDR3;
        __I  uint32_t PDR3;
    };
    
    __I uint32_t RESERVE0;

    union 
    {
        __IO uint32_t u32PIER;
        
        struct 
        {
            __IO uint32_t  PWMIE0:1;
            __IO uint32_t  PWMIE1:1;
            __IO uint32_t  PWMIE2:1;
            __IO uint32_t  PWMIE3:1;
            __I  uint32_t  RESERVE:28;
        } PIER;
    };
    
    union 
    {
        __IO uint32_t u32PIIR;
        
        struct 
        {
            __IO uint32_t  PWMIF0:1;
            __IO uint32_t  PWMIF1:1;
            __IO uint32_t  PWMIF2:1;
            __IO uint32_t  PWMIF3:1;
            __I  uint32_t  RESERVE:28;
        } PIIR;
    };    
    
    __I uint32_t    RESERVE1[2];

    union 
    {
        __IO uint32_t u32CCR0;
        
        struct 
        {
            __IO uint32_t  INV0:1;
            __IO uint32_t  CRL_IE0:1;
            __IO uint32_t  CFL_IE0:1;
            __IO uint32_t  CAPCH0EN:1;
            __IO uint32_t  CAPIF0:1;
            __I  uint32_t  RESERVE0:1;
            __IO uint32_t  CRLRI0:1;
            __IO uint32_t  CFLRI0:1;
            __I  uint32_t  RESERVE1:8;
            __IO uint32_t  INV1:1;
            __IO uint32_t  CRL_IE1:1;
            __IO uint32_t  CFL_IE1:1;
            __IO uint32_t  CAPCH1EN:1;
            __IO uint32_t  CAPIF1:1;
            __I  uint32_t  RESERVE2:1;
            __IO uint32_t  CRLRI1:1;
            __IO uint32_t  CFLRI1:1;
            __I  uint32_t  RESERVE3:8;
        } CCR0;
    };
    
    union 
    {
        __IO uint32_t u32CCR2;
        
        struct 
        {
            __IO uint32_t  INV2:1;
            __IO uint32_t  CRL_IE2:1;
            __IO uint32_t  CFL_IE2:1;
            __IO uint32_t  CAPCH2EN:1;
            __IO uint32_t  CAPIF2:1;
            __I  uint32_t  RESERVE0:1;
            __IO uint32_t  CRLRI2:1;
            __IO uint32_t  CFLRI2:1;
            __I  uint32_t  RESERVE1:8;
            __IO uint32_t  INV3:1;
            __IO uint32_t  CRL_IE3:1;
            __IO uint32_t  CFL_IE3:1;
            __IO uint32_t  CAPCH3EN:1;
            __IO uint32_t  CAPIF3:1;
            __I  uint32_t  RESERVE2:1;
            __IO uint32_t  CRLRI3:1;
            __IO uint32_t  CFLRI3:1;
            __I  uint32_t  RESERVE3:8;
        } CCR2;
    };    

    union 
    {
        __IO uint32_t u32CRLR0;
        __IO uint32_t CRLR0;
    };

    union 
    {
        __IO uint32_t u32CFLR0;
        __IO uint32_t CFLR0;
    };

    union 
    {
        __IO uint32_t u32CRLR1;
        __IO uint32_t CRLR1;
    };

    union 
    {
        __IO uint32_t u32CFLR1;
        __IO uint32_t CFLR1;
    };

    union 
    {
        __IO uint32_t u32CRLR2;
        __IO uint32_t CRLR2;
    };

    union 
    {
        __IO uint32_t u32CFLR2;
        __IO uint32_t CFLR2;
    };
    
    union 
    {
        __IO uint32_t u32CRLR3;
        __IO uint32_t CRLR3;
    };

    union 
    {
        __IO uint32_t u32CFLR3;
        __IO uint32_t CFLR3;
    };    

    union 
    {
        __IO uint32_t u32CAPENR;
        __IO uint32_t CAPENR;
    };

    union 
    {
        __IO uint32_t u32POE;
        
        struct 
        {
            __IO uint32_t  PWM0:1;
            __IO uint32_t  PWM1:1;
            __IO uint32_t  PWM2:1;
            __IO uint32_t  PWM3:1;
            __I  uint32_t  RESERVE:28;
        } POE;
    }; 
} PWM_T;

//
//! PWM PPR Bit Field Definitions
//
#define PWM_PPR_DZI23_Pos                       24
#define PWM_PPR_DZI23_Msk                       (0xFFul << PWM_PPR_DZI23_Pos)

#define PWM_PPR_DZI01_Pos                       16
#define PWM_PPR_DZI01_Msk                       (0xFFul << PWM_PPR_DZI01_Pos)

#define PWM_PPR_CP23_Pos                        8
#define PWM_PPR_CP23_Msk                        (0xFFul << PWM_PPR_CP23_Pos)

#define PWM_PPR_CP01_Pos                        0
#define PWM_PPR_CP01_Msk                        (0xFFul << PWM_PPR_CP01_Pos)

//
//! PWM CSR Bit Field Definitions 
//
#define PWM_CSR_CSR3_Pos                        12
#define PWM_CSR_CSR3_Msk                        (7ul << PWM_CSR_CSR3_Pos)

#define PWM_CSR_CSR2_Pos                        8
#define PWM_CSR_CSR2_Msk                        (7ul << PWM_CSR_CSR2_Pos)

#define PWM_CSR_CSR1_Pos                        4
#define PWM_CSR_CSR1_Msk                        (7ul << PWM_CSR_CSR1_Pos)

#define PWM_CSR_CSR0_Pos                        0
#define PWM_CSR_CSR0_Msk                        (7ul << PWM_CSR_CSR0_Pos)

//
//! PWM PCR Bit Field Definitions
//
#define PWM_PCR_CH3MOD_Pos                      27
#define PWM_PCR_CH3MOD_Msk                      (1ul << PWM_PCR_CH3MOD_Pos)

#define PWM_PCR_CH3INV_Pos                      26
#define PWM_PCR_CH3INV_Msk                      (1ul << PWM_PCR_CH3INV_Pos)

#define PWM_PCR_CH3EN_Pos                       24
#define PWM_PCR_CH3EN_Msk                       (1ul << PWM_PCR_CH3EN_Pos)

#define PWM_PCR_CH2MOD_Pos                      19
#define PWM_PCR_CH2MOD_Msk                      (1ul << PWM_PCR_CH2MOD_Pos)

#define PWM_PCR_CH2INV_Pos                      18
#define PWM_PCR_CH2INV_Msk                      (1ul << PWM_PCR_CH2INV_Pos)

#define PWM_PCR_CH2EN_Pos                       16
#define PWM_PCR_CH2EN_Msk                       (1ul << PWM_PCR_CH2EN_Pos)

#define PWM_PCR_CH1MOD_Pos                      11
#define PWM_PCR_CH1MOD_Msk                      (1ul << PWM_PCR_CH1MOD_Pos)

#define PWM_PCR_CH1INV_Pos                      10
#define PWM_PCR_CH1INV_Msk                      (1ul << PWM_PCR_CH1INV_Pos)

#define PWM_PCR_CH1EN_Pos                       8
#define PWM_PCR_CH1EN_Msk                       (1ul << PWM_PCR_CH1EN_Pos)

#define PWM_PCR_DZEN23_Pos                      5
#define PWM_PCR_DZEN23_Msk                      (1ul << PWM_PCR_DZEN23_Pos)

#define PWM_PCR_DZEN01_Pos                      4
#define PWM_PCR_DZEN01_Msk                      (1ul << PWM_PCR_DZEN01_Pos)

#define PWM_PCR_CH0MOD_Pos                      3
#define PWM_PCR_CH0MOD_Msk                      (1ul << PWM_PCR_CH0MOD_Pos)

#define PWM_PCR_CH0INV_Pos                      2
#define PWM_PCR_CH0INV_Msk                      (1ul << PWM_PCR_CH0INV_Pos)

#define PWM_PCR_CH0EN_Pos                       0
#define PWM_PCR_CH0EN_Msk                       (1ul << PWM_PCR_CH0EN_Pos)

//
//! PWM CNR Bit Field Definitions
//
#define PWM_CNR_CNR_Pos                         0
#define PWM_CNR_CNR_Msk                         (0xFFFFul << PWM_CNR_CNR_Pos)

//
//! PWM CMR Bit Field Definitions
//
#define PWM_CMR_CMR_Pos                         0
#define PWM_CMR_CMR_Msk                         (0xFFFFul << PWM_CMR_CMR_Pos)

//
//! PWM PDR Bit Field Definitions
//
#define PWM_PDR_PDR_Pos                         0
#define PWM_PDR_PDR_Msk                         (0xFFFFul << PWM_PDR_PDR_Pos)

//
//! PWM PIER Bit Field Definitions
//
#define PWM_PIER_PWMIE3_Pos                     3
#define PWM_PIER_PWMIE3_Msk                     (1ul << PWM_PIER_PWMIE3_Pos)

#define PWM_PIER_PWMIE2_Pos                     2
#define PWM_PIER_PWMIE2_Msk                     (1ul << PWM_PIER_PWMIE2_Pos)

#define PWM_PIER_PWMIE1_Pos                     1
#define PWM_PIER_PWMIE1_Msk                     (1ul << PWM_PIER_PWMIE1_Pos)

#define PWM_PIER_PWMIE0_Pos                     0
#define PWM_PIER_PWMIE0_Msk                     (1ul << PWM_PIER_PWMIE0_Pos)

//
//! PWM PIIR Bit Field Definitions
//
#define PWM_PIIR_PWMIF3_Pos                     3
#define PWM_PIIR_PWMIF3_Msk                     (1ul << PWM_PIIR_PWMIF3_Pos)

#define PWM_PIIR_PWMIF2_Pos                     2
#define PWM_PIIR_PWMIF2_Msk                     (1ul << PWM_PIIR_PWMIF2_Pos)

#define PWM_PIIR_PWMIF1_Pos                     1
#define PWM_PIIR_PWMIF1_Msk                     (1ul << PWM_PIIR_PWMIF1_Pos)

#define PWM_PIIR_PWMIF0_Pos                     0
#define PWM_PIIR_PWMIF0_Msk                     (1ul << PWM_PIIR_PWMIF0_Pos)

//
//! PWM CCR0 Bit Field Definitions
//
#define PWM_CCR0_CFLRI1_Pos                     23
#define PWM_CCR0_CFLRI1_Msk                     (1ul << PWM_CCR0_CFLRI1_Pos)

#define PWM_CCR0_CRLRI1_Pos                     22
#define PWM_CCR0_CRLRI1_Msk                     (1ul << PWM_CCR0_CRLRI1_Pos)

#define PWM_CCR0_CAPIF1_Pos                     20
#define PWM_CCR0_CAPIF1_Msk                     (1ul << PWM_CCR0_CAPIF1_Pos)

#define PWM_CCR0_CAPCH1EN_Pos                   19
#define PWM_CCR0_CAPCH1EN_Msk                   (1ul << PWM_CCR0_CAPCH1EN_Pos)

#define PWM_CCR0_CFL_IE1_Pos                    18
#define PWM_CCR0_CFL_IE1_Msk                    (1ul << PWM_CCR0_CFL_IE1_Pos)

#define PWM_CCR0_CRL_IE1_Pos                    17
#define PWM_CCR0_CRL_IE1_Msk                    (1ul << PWM_CCR0_CRL_IE1_Pos)

#define PWM_CCR0_INV1_Pos                       16
#define PWM_CCR0_INV1_Msk                       (1ul << PWM_CCR0_INV1_Pos)

#define PWM_CCR0_CFLRI0_Pos                     7
#define PWM_CCR0_CFLRI0_Msk                     (1ul << PWM_CCR0_CFLRI0_Pos)

#define PWM_CCR0_CRLRI0_Pos                     6
#define PWM_CCR0_CRLRI0_Msk                     (1ul << PWM_CCR0_CRLRI0_Pos)

#define PWM_CCR0_CAPIF0_Pos                     4
#define PWM_CCR0_CAPIF0_Msk                     (1ul << PWM_CCR0_CAPIF0_Pos)

#define PWM_CCR0_CAPCH0EN_Pos                   3
#define PWM_CCR0_CAPCH0EN_Msk                   (1ul << PWM_CCR0_CAPCH0EN_Pos)

#define PWM_CCR0_CFL_IE0_Pos                    2
#define PWM_CCR0_CFL_IE0_Msk                    (1ul << PWM_CCR0_CFL_IE0_Pos)

#define PWM_CCR0_CRL_IE0_Pos                    1
#define PWM_CCR0_CRL_IE0_Msk                    (1ul << PWM_CCR0_CRL_IE0_Pos)

#define PWM_CCR0_INV0_Pos                       0
#define PWM_CCR0_INV0_Msk                       (1ul << PWM_CCR0_INV0_Pos)

//
//! PWM CCR2 Bit Field Definitions
//
#define PWM_CCR2_CFLRI3_Pos                     23
#define PWM_CCR2_CFLRI3_Msk                     (1ul << PWM_CCR2_CFLRI3_Pos)

#define PWM_CCR2_CRLRI3_Pos                     22
#define PWM_CCR2_CRLRI3_Msk                     (1ul << PWM_CCR2_CRLRI3_Pos)

#define PWM_CCR2_CAPIF3_Pos                     20
#define PWM_CCR2_CAPIF3_Msk                     (1ul << PWM_CCR2_CAPIF3_Pos)

#define PWM_CCR2_CAPCH3EN_Pos                   19
#define PWM_CCR2_CAPCH3EN_Msk                   (1ul << PWM_CCR2_CAPCH3EN_Pos)

#define PWM_CCR2_CFL_IE3_Pos                    18
#define PWM_CCR2_CFL_IE3_Msk                    (1ul << PWM_CCR2_CFL_IE3_Pos)

#define PWM_CCR2_CRL_IE3_Pos                    17
#define PWM_CCR2_CRL_IE3_Msk                    (1ul << PWM_CCR2_CRL_IE3_Pos)

#define PWM_CCR2_INV3_Pos                       16
#define PWM_CCR2_INV3_Msk                       (1ul << PWM_CCR2_INV3_Pos)

#define PWM_CCR2_CFLRI2_Pos                     7
#define PWM_CCR2_CFLRI2_Msk                     (1ul << PWM_CCR2_CFLRI2_Pos)

#define PWM_CCR2_CRLRI2_Pos                     6
#define PWM_CCR2_CRLRI2_Msk                     (1ul << PWM_CCR2_CRLRI2_Pos)

#define PWM_CCR2_CAPIF2_Pos                     4
#define PWM_CCR2_CAPIF2_Msk                     (1ul << PWM_CCR2_CAPIF2_Pos)

#define PWM_CCR2_CAPCH2EN_Pos                   3
#define PWM_CCR2_CAPCH2EN_Msk                   (1ul << PWM_CCR2_CAPCH2EN_Pos)

#define PWM_CCR2_CFL_IE2_Pos                    2
#define PWM_CCR2_CFL_IE2_Msk                    (1ul << PWM_CCR2_CFL_IE2_Pos)

#define PWM_CCR2_CRL_IE2_Pos                    1
#define PWM_CCR2_CRL_IE2_Msk                    (1ul << PWM_CCR2_CRL_IE2_Pos)

#define PWM_CCR2_INV2_Pos                       0
#define PWM_CCR2_INV2_Msk                       (1ul << PWM_CCR2_INV2_Pos)

//
//! PWM CRLR Bit Field Definitions
//
#define PWM_CRLR_CRLR_Pos                       0
#define PWM_CRLR_CRLR_Msk                       (0xFFFFul << PWM_CRLR_CRLR_Pos)

//
//! PWM CFLR Bit Field Definitions
//
#define PWM_CFLR_CFLR_Pos                       0
#define PWM_CFLR_CFLR_Msk                       (0xFFFFul << PWM_CFLR_CFLR_Pos)

//
//! PWM CAPENR Bit Field Definitions
//
#define PWM_CAPENR_CAPENR_Pos                   0
#define PWM_CAPENR_CAPENR_Msk                   (0xFul << PWM_CAPENR_CAPENR_Pos)

//
//! PWM POE Bit Field Definitions
//
#define PWM_POE_PWM3_Pos                        3
#define PWM_POE_PWM3_Msk                        (1ul << PWM_POE_PWM3_Pos)

#define PWM_POE_PWM2_Pos                        2
#define PWM_POE_PWM2_Msk                        (1ul << PWM_POE_PWM2_Pos)

#define PWM_POE_PWM1_Pos                        1
#define PWM_POE_PWM1_Msk                        (1ul << PWM_POE_PWM1_Pos)

#define PWM_POE_PWM0_Pos                        0
#define PWM_POE_PWM0_Msk                        (1ul << PWM_POE_PWM0_Pos)

//*****************************************************************************
//
//! EBI Controller
//
//*****************************************************************************
typedef struct
{
    __IO uint32_t  ExtEN:1;
    __IO uint32_t  ExtBW16:1;
    __I  uint32_t  RESERVE0:6;
    __IO uint32_t  MCLKDIV:3;
    __I  uint32_t  RESERVE1:5;
    __IO uint32_t  ExttALE:3;
    __I  uint32_t  RESERVE2:13;
} EBI_EBICON_T;

typedef struct
{
    __I  uint32_t  RESERVE0:3;
    __IO uint32_t  ExttACC:5;
    __IO uint32_t  ExttAHD:3;
    __I  uint32_t  RESERVE1:1;
    __IO uint32_t  ExtIW2X:4;
    __I  uint32_t  RESERVE2:8;
    __IO uint32_t  ExtIR2R:4;
    __I  uint32_t  RESERVE3:4;
} EBI_EXTIME_T;

typedef struct
{
    union {
        __IO uint32_t u32EBICON;
        struct {
            __IO uint32_t  ExtEN:1;
            __IO uint32_t  ExtBW16:1;
            __I  uint32_t  RESERVE0:6;
            __IO uint32_t  MCLKDIV:3;
            __I  uint32_t  RESERVE1:5;
            __IO uint32_t  ExttALE:3;
            __I  uint32_t  RESERVE2:13;
        } EBICON;
    };

    union {
        __IO uint32_t u32EXTIME;
        struct {
            __I  uint32_t  RESERVE0:3;
            __IO uint32_t  ExttACC:5;
            __IO uint32_t  ExttAHD:3;
            __I  uint32_t  RESERVE1:1;
            __IO uint32_t  ExtIW2X:4;
            __I  uint32_t  RESERVE2:8;
            __IO uint32_t  ExtIR2R:4;
            __I  uint32_t  RESERVE3:4;
        } EXTIME;
    };
} EBI_T;

//
//! EBI EBICON Bit Field Definitions
//
#define EBI_EBICON_ExttALE_Pos      16
#define EBI_EBICON_ExttALE_Msk      (0x7ul << EBI_EBICON_ExttALE_Pos)

#define EBI_EBICON_MCLKDIV_Pos      8
#define EBI_EBICON_MCLKDIV_Msk      (0x7ul << EBI_EBICON_MCLKDIV_Pos)

#define EBI_EBICON_ExtBW16_Pos      1
#define EBI_EBICON_ExtBW16_Msk      (1ul << EBI_EBICON_ExtBW16_Pos)

#define EBI_EBICON_ExtEN_Pos        0
#define EBI_EBICON_ExtEN_Msk        (1ul << EBI_EBICON_ExtEN_Pos)

//
//! EBI EXTIME Bit Field Definitions 
//
#define EBI_EXTIME_ExtIR2R_Pos      24
#define EBI_EXTIME_ExtIR2R_Msk      (0xFul << EBI_EXTIME_ExtIR2R_Pos)

#define EBI_EXTIME_ExtIW2X_Pos      12
#define EBI_EXTIME_ExtIW2X_Msk      (0xFul << EBI_EXTIME_ExtIW2X_Pos)

#define EBI_EXTIME_ExttAHD_Pos      8
#define EBI_EXTIME_ExttAHD_Msk      (0x7ul << EBI_EXTIME_ExttAHD_Pos)

#define EBI_EXTIME_ExttACC_Pos      3
#define EBI_EXTIME_ExttACC_Msk      (0x1Ful << EBI_EXTIME_ExttACC_Pos)


//******************************************************************************
//
//! Peripheral memory map       
//
//******************************************************************************

//
//! Peripheral and SRAM base address
//
#define FLASH_BASE          ((     uint32_t)0x00000000)
#define SRAM_BASE           ((     uint32_t)0x20000000)
#define AHB_BASE            ((     uint32_t)0x50000000)
#define APB1_BASE           ((     uint32_t)0x40000000)
#define APB2_BASE           ((     uint32_t)0x40100000)

//
//! Peripheral memory map
//
#define GPIO_BASE           (AHB_BASE       + 0x4000)
#define PORT0_BASE          (GPIO_BASE             	)
#define PORT1_BASE         	(GPIO_BASE     	+ 0x0040)
#define PORT2_BASE         	(GPIO_BASE     	+ 0x0080)
#define PORT3_BASE         	(GPIO_BASE     	+ 0x00C0)
#define PORT4_BASE         	(GPIO_BASE     	+ 0x0100)
#define PORT_DBNCECON_BASE  (GPIO_BASE     	+ 0x0180)
#define PORT_BIT_DOUT_BASE	(GPIO_BASE 		+ 0x200)

#define UART0_BASE           (APB1_BASE      + 0x50000)
#define UART1_BASE           (APB2_BASE      + 0x50000)

#define TIMER0_BASE          (APB1_BASE      + 0x10000)  
#define TIMER1_BASE          (APB1_BASE      + 0x10020)  
#define TIMER2_BASE          (APB2_BASE      + 0x10000)  
#define TIMER3_BASE          (APB2_BASE      + 0x10020)  

#define WDT_BASE       	     (APB1_BASE      + 0x4000)  

#define SPI0_BASE            (APB1_BASE      + 0x30000)
#define SPI1_BASE            (APB1_BASE      + 0x34000)

#define I2C_BASE             (APB1_BASE      + 0x20000)

#define RTC_BASE             (APB1_BASE      + 0x08000)

#define ADC_BASE             (APB1_BASE      + 0xE0000) 

#define SYSCLK_BASE          (AHB_BASE       + 0x00200)

#define GCR_BASE             (AHB_BASE       + 0x00000)

#define INT_BASE             (AHB_BASE       + 0x00300)

#define FMC_BASE             (AHB_BASE       + 0x0C000)

#define PWMA_BASE            (APB1_BASE      + 0x40000)
#define PWMB_BASE            (APB2_BASE      + 0x40000)

#define EBI_BASE             (AHB_BASE       + 0x10000)

//*****************************************************************************
//
//! Peripheral declaration      
//
//*****************************************************************************
#define PORT0              	((PORT_T *) PORT0_BASE)
#define PORT1              	((PORT_T *) PORT1_BASE)
#define PORT2              	((PORT_T *) PORT2_BASE)
#define PORT3              	((PORT_T *) PORT3_BASE)
#define PORT4              	((PORT_T *) PORT4_BASE)
#define PORT_DBNCECON       ((PORT_DBNCECON_T *) PORT_DBNCECON_BASE)
#define PORT_BIT_DOUT		(PORT_BIT_DOUT_BASE)

#define UART0               ((UART_T *) UART0_BASE)
#define UART1               ((UART_T *) UART1_BASE)

#define TIMER0              ((TIMER_T *) TIMER0_BASE)
#define TIMER1              ((TIMER_T *) TIMER1_BASE)
#define TIMER2              ((TIMER_T *) TIMER2_BASE)
#define TIMER3              ((TIMER_T *) TIMER3_BASE)

#define WDT                 ((WDT_T *) WDT_BASE)

#define SPI0                ((SPI_T *) SPI0_BASE)
#define SPI1                ((SPI_T *) SPI1_BASE)

#define I2C                 ((I2C_T *) I2C_BASE)

#define ADC                 ((ADC_T *) ADC_BASE)

#define SYSCLK              ((SYSCLK_T *) SYSCLK_BASE)

#define SYS                 ((GCR_T *) GCR_BASE)

#define SYSINT              ((GCR_INT_T *) INT_BASE)

#define FMC                 ((FMC_T *) FMC_BASE)

#define PWMA                ((PWM_T *) PWMA_BASE)
#define PWMB                ((PWM_T *) PWMB_BASE)

#define EBI                 ((EBI_T *) EBI_BASE)

#define UNLOCKREG(x)        *((__IO uint32_t *)(GCR_BASE + 0x100)) = 0x59;*((__IO uint32_t *)(GCR_BASE + 0x100)) = 0x16;*((__IO uint32_t *)(GCR_BASE + 0x100)) = 0x88
#define LOCKREG(x)          *((__IO uint32_t *)(GCR_BASE + 0x100)) = 0x00;     

#define REGCOPY(dest, src)  *((uint32_t *)&(dest)) = *((uint32_t *)&(src))
#define CLEAR(dest) 		*((uint32_t *)&(dest)) = 0

typedef volatile unsigned char  vu8;
typedef volatile unsigned long  vu32;
typedef volatile unsigned short vu16;
#define M8(adr)  (*((vu8  *) (adr)))
#define M16(adr) (*((vu16 *) (adr)))
#define M32(adr) (*((vu32 *) (adr)))

#define outpw(port,value)	*((volatile unsigned int *)(port))=value
#define inpw(port)			(*((volatile unsigned int *)(port)))
#define outpb(port,value)	*((volatile unsigned char *)(port))=value
#define inpb(port)			(*((volatile unsigned char *)(port)))
#define outps(port,value)	*((volatile unsigned short *)(port))=value
#define inps(port)			(*((volatile unsigned short *)(port)))

#define outp32(port,value)	*((volatile unsigned int *)(port))=value
#define inp32(port)			(*((volatile unsigned int *)(port)))
#define outp8(port,value)	*((volatile unsigned char *)(port))=value
#define inp8(port)			(*((volatile unsigned char *)(port)))
#define outp16(port,value)	*((volatile unsigned short *)(port))=value
#define inp16(port)			(*((volatile unsigned short *)(port)))

#define E_SUCCESS 	0
#ifndef NULL
#define NULL        0
#endif

#define TRUE	   	1
#define FALSE   	0

#define ENABLE      1
#define DISABLE     0

//
//! Define one bit mask
//
#define BIT0	0x00000001
#define BIT1	0x00000002
#define BIT2	0x00000004
#define BIT3	0x00000008
#define BIT4	0x00000010
#define BIT5	0x00000020
#define BIT6	0x00000040
#define BIT7	0x00000080
#define BIT8	0x00000100
#define BIT9	0x00000200
#define BIT10	0x00000400
#define BIT11	0x00000800
#define BIT12	0x00001000
#define BIT13	0x00002000
#define BIT14	0x00004000
#define BIT15	0x00008000
#define BIT16	0x00010000
#define BIT17	0x00020000
#define BIT18	0x00040000
#define BIT19	0x00080000
#define BIT20	0x00100000
#define BIT21	0x00200000
#define BIT22	0x00400000
#define BIT23	0x00800000
#define BIT24	0x01000000
#define BIT25	0x02000000
#define BIT26	0x04000000
#define BIT27	0x08000000
#define BIT28	0x10000000
#define BIT29	0x20000000
#define BIT30	0x40000000
#define BIT31	0x80000000

#endif
                                                                                                 

