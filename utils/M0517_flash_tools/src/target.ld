MEMORY
{
	rom (rx)  : ORIGIN = 0x00000000, LENGTH = 0x00010000
	ram (rwx) : ORIGIN = 0x20000000, LENGTH = 0x00001000
}



/* Section Definitions */ 
SECTIONS 
{ 
        .text : 
        { 
                . = 0;        			 
                *(.text .text.*) 
                
        } > ram
        
        .rodata :
        {
        				
                *(.rodata .rodata*) 
        } > ram

        .data :
        {  
                *(.data .data.*) 

        } > ram
                
        .got : 
        {
                *(.got .got.*)         		
        } > ram

        /* .bss section which is used for uninitialized data */ 
        .bss : 
        { 

                *(.bss .bss.*) 
                *(COMMON) 
			
        } > ram
		
		.driver_info :
		{
			*(.driver_info .driver_info.*) 
		}  > ram
		
		

} 
