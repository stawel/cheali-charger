/* -----------------------------------------------------------------------------
 * Support function for flash NuMicro M051x microcontrolers 
 *   for use with OpenOCD TCL scripts
 *
 * Copyright (c) 2015 Saša Mihajlović
 *
 * -----------------------------------------------------------------------------
 * Copyright (c) 2014 ARM Ltd.
 *
 * This software is provided 'as-is', without any express or implied warranty. 
 * In no event will the authors be held liable for any damages arising from 
 * the use of this software. Permission is granted to anyone to use this 
 * software for any purpose, including commercial applications, and to alter 
 * it and redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not 
 *    claim that you wrote the original software. If you use this software in
 *    a product, an acknowledgment in the product documentation would be 
 *    appreciated but is not required. 
 * 
 * 2. Altered source versions must be plainly marked as such, and must not be 
 *    misrepresented as being the original software. 
 * 
 * 3. This notice may not be removed or altered from any source distribution.
 *   
 *
 * $Date:        15. April 2014
 * $Revision:    V1.00
 *  
 * Project:      Flash Programming Functions for Nuvoton NuMicro M05x Flash using ISP
 * --------------------------------------------------------------------------- */

typedef volatile unsigned long    vu32;
typedef          unsigned long     u32;

#define M32(adr) (*((vu32 *) (adr)))

// Flash Memory Map
#define APROM_BASE        (0x00000000)
#define LDROM_BASE        (0x00100000)
#define CFG_BASE          (0x00300000)
#define SRAM_BASE         (0x20000000)


// Peripheral Memory Map
#define GCR_BASE          (0x50000000)
#define CLK_BASE          (0x50000200)
#define FMC_BASE          (0x5000C000)


//unsigned long const GCR_BASE=(0x50000000);
//unsigned long const CLK_BASE=(0x50000200);
//unsigned long const FMC_BASE=(0x5000C000);

#define GCR               ((GCR_TypeDef *) GCR_BASE) //ok
#define CLK               ((CLK_TypeDef *) CLK_BASE)
#define FMC               ((FMC_TypeDef *) FMC_BASE)


// System Manager Control Registers
typedef struct {
  vu32 PDID;                 // offset 0x000 R/W Part Device Identification number Register
  vu32 RSTSRC;               // offset 0x004 R/W System Reset Source Register
  vu32 IPRSTC1;              // offset 0x008 R/W IP Reset Control Resister1
  vu32 IPRSTC2;              // offset 0x00C R/W IP Reset Control Resister2
  vu32 RESERVED1[2];
  vu32 BODCR;                // offset 0x018 R/W Brown Out Detector Control Register
  vu32 RESERVED2[5];
  vu32 GP0_MFP;              // offset 0x030 R/W GPIO0 multiple function and input type control register
  vu32 GP1_MFP;              // offset 0x034 R/W GPIO1 multiple function and input type control register
  vu32 GP2_MFP;              // offset 0x038 R/W GPIO2 multiple function and input type control register
  vu32 GP3_MF;               // offset 0x03C R/W GPIO3 multiple function and input type control register
  vu32 GP4_MFP;              // offset 0x040 R/W GPIO4 input type control register
  vu32 RESERVED3[47];
  vu32 REGWRPROT;            // offset 0x100 R/W Register Lock Key address
  vu32 RESERVED5[3];
  vu32 RCADJ;                // offset 0x110 R/W RC Adjustment Value
} GCR_TypeDef;

// Clock Control Registers
typedef struct {
  vu32 PWRCON;               // offset 0x000 R/W System Power Down Control Register
  vu32 AHBCLK;               // offset 0x004 R/W AHB Devices Clock Enable Control Register
  vu32 APBCLK;               // offset 0x008 R/W APB Devices Clock Enable Control Register
  vu32 CLKSTATUS;            // offset 0x00C R/W Clock status monitor Register
  vu32 CLKSEL0;              // offset 0x010 R/W Clock Source Select Control Register 0
  vu32 CLKSEL1;              // offset 0x014 R/W Clock Source Select Control Register 1
  vu32 CLKDIV;               // offset 0x018 R/W Clock Divider Number Register
  vu32 CLKSEL2;              // offset 0x01C R/W Clock Source Select Control Register 2
  vu32 PLLCON;               // offset 0x020 R/W PLL Control Register
  vu32 FRQDIV;               // offset 0x024 R/W Frequency Divider Control Register
} CLK_TypeDef;

// FLASH MEMORY CONTROLLER (FMC) Registers
typedef struct {
  vu32 ISPCON;               // offset 0x000 R/W ISP Control Register
  vu32 ISPADR;               // offset 0x004 R/W ISP Address Register
  vu32 ISPDAT;               // offset 0x008 R/W ISP Data Register
  vu32 ISPCMD;               // offset 0x00C R/W ISP Command Register
  vu32 ISPTRG;               // offset 0x010 R/W ISP Trigger Register
  vu32 DFBADR;               // offset 0x014 R Data Flash Start Address
  vu32 FATCON;               // offset 0x018 R/W Flash Access Window Control Register
  vu32 ICPCON;               // offset 0x01C R/W ??
  vu32 RMPCON;               // offset 0x020 R/W ??
  } FMC_TypeDef;

/*
  ------------------+---------------------+-----------------+-------------------
                    |ISPCMD               | ISPADR          |ISPDAT
  ISP Mode          +---------------------+-----------------+-------------------
                    |FOEN FCEN FCTRL[3:0] | A21 A20 A[19:0] | D[31:0]
  ------------------+---------------------+-----------------+-------------------
  Standby             1    1    X            x   x   x         x
  Read Company ID     0    0    1011         x   x   x        Data out D[31:0] = 0x000000DA
  Read Device ID      0    0    1100         x   x   0        Data out D[31:0] = 0x00000E80 (device ID depends on ROMMAP settings)
  FLASH Page Erase    1    0    0010         0  A20 A[19:0]    x
  FLASH Program       1    0    0001         0  A20 A[19:0]   Data in  D[31:0]
  FLASH Read          0    0    0000         0  A20 A[19:0]   Data out D[31:0]
  CONFIG Page Erase   1    0    0010         1   1  A[19:0]    x
  CONFIG Program      1    0    0001         1   1  A[19:0]   Data in  D[31:0]
  CONFIG Read         0    0    0000         1   1  A[19:0]   Data out D[31:0]
 */


// CLK AHBCLK ISP_EN
#define CLK_ISP_EN            ((unsigned int)0x00000040)

// ICP Control Register (ICPCON) definitions
#define FMC_ICPEN             ((unsigned int)0x00000001)

// ISP Control Register (ISPCON) definitions
#define FMC_ISPFF             ((unsigned int)0x00000040)
#define FMC_LDUEN             ((unsigned int)0x00000020)
#define FMC_ISPEN             ((unsigned int)0x00000001)

// ISP Command (ISPCMD) definitions
#define FMC_STANDBY             ((unsigned int)0x00000030)
#define FMC_READ                ((unsigned int)0x00000000)
#define FMC_PROGRAM             ((unsigned int)0x00000021)
#define FMC_PAGE_ERASE          ((unsigned int)0x00000022)
#define FMC_READ_CID            ((unsigned int)0x0000000B)
#define FMC_READ_DID            ((unsigned int)0x0000000C)

// ISP Trigger Control Register (ISPTRG) definitions
#define FMC_ISPGO               ((unsigned int)0x00000001)

//***************************
// register assigments
// input:
// 		r0 :: adr
// 		r1 :: sz
// 		r2 :: *buf
// output:
// 		r0 :: return (0=ok; !0= error)
//***************************

int FlashInit (void)
{
  int delay = 100;
  int ret=1;

  GCR->REGWRPROT = 0x59;      // Unlock System Control Registers
  GCR->REGWRPROT = 0x16;                     
  GCR->REGWRPROT = 0x88;                     

  if (!(GCR->REGWRPROT & 0x01))
  {
    ret = 1;
    goto end;                    // Not able to unlock 
  }

  CLK->PWRCON |= (1 << 2);       // enable internal OSC22MHz 
  CLK->AHBCLK |= (1 << 2); 		 // enable internal OSC22MHz
  while (delay--);

//  CLK->AHBCLK |= CLK_ISP_EN;              // enable the Flash ISP controller clock
  
  FMC->ISPCON |= (FMC_ISPEN | FMC_LDUEN);  // Enable ISP function
  FMC->ICPCON |= (FMC_ICPEN);              // Enable ICP functions ?

  if (!(FMC->ISPCON & FMC_ISPEN))
  {
    ret = 2;
    goto end;                               // ISP not enabled 
  }

  FMC->ISPCON |= FMC_ISPFF;                 // Reset Error Flag
  ret=0;

 end:                       
  __asm volatile ( "BKPT	#0 \n\t"  );
                    
  return ret;
}

/*
int UnInit (unsigned long fnc) {
	
  while (FMC->ISPTRG & FMC_ISPGO);     // Wait until command is finished

  FMC->ISPCON &= (~(FMC_ISPEN | FMC_LDUEN));
  FMC->ICPCON &= (~(FMC_ICPEN));                   

  return (0);
}
*/


// r0 :: adr	// first sector to erase address
// r1 :: s_num	// num sectors to erase
//
// r0 :: return (0=ok; !0= error)  
int EraseSectors(unsigned long adr,
				 unsigned long s_num)
{
	int i,ret;
	ret=1;
//	while (FMC->ISPTRG & FMC_ISPGO)     // Wait until command is finished
	
	FMC->ISPCON |= FMC_ISPFF;               // Reset Error Flags

	for(i = 0; i < s_num; i++)
	{
		FMC->ISPCMD = FMC_PAGE_ERASE;       // Prepare Command
		FMC->ISPADR = (adr & ~0x00000003);	// Prepare Address
		FMC->ISPTRG = FMC_ISPGO;            // Exacute Command
		__asm volatile ("ISB #0 \n\t"  );

		while (FMC->ISPTRG & FMC_ISPGO)     // Wait until command is finished
		{
		/* reload Watdchdog */              // Reload IWDG
		}	
			
		if (FMC->ISPCON &  FMC_ISPFF)       // Check for Error
		{	
			FMC->ISPCON |= FMC_ISPFF;       // Reset Error Flags
		    ret=1;
		    goto end;	                    // Failed
		}
		adr += 512;
	}
	ret=0;
 
 end:                       
	__asm volatile ( "BKPT	#0 \n\t"  );
                    
	return ret;
}


// r0 :: adr  >> where to copy (destination address)
// r1 :: sz   >> size in bytes
// r2 :: *buf >> pointer to data source buffer
//
// r0 :: return (0=ok; !0= error)
int ProgramPage (unsigned long adr,     
                      unsigned long sz,
                      unsigned char *buf)
{
	int ret;
	
	sz = (sz + 3) & ~3;                       // Adjust size for Words
	
	FMC->ISPCON |= FMC_ISPFF;                 // Reset Error Flags
	
	while (sz) 
	{
		FMC->ISPCMD = FMC_PROGRAM;            // Prepare Command
		FMC->ISPADR = (adr & ~0x00000003); 	  // Prepare Address
		FMC->ISPDAT	= *((u32 *)buf);          // Prepare Data
		FMC->ISPTRG = FMC_ISPGO;              // Exacute Command
		__asm volatile ("ISB #0 \n\t"  );
		
		while (FMC->ISPTRG & FMC_ISPGO) {     																// Wait until command is finished
		  /* reload Watdchdog */              // Reload IWDG
		}
		
		if (FMC->ISPCON &  FMC_ISPFF) {       															// Check for Error
		    FMC->ISPCON |= FMC_ISPFF;         // Reset Error Flags
		    ret=1;
		    goto end;	                      // Failed
		}
		
		adr += 4;                             // Go to next Word
		buf += 4;
		sz  -= 4;
	}
	ret=0;

end:                       
	__asm volatile ( "BKPT	#0 \n\t"  );            

	return ret;
}
