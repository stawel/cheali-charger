##
#rlwrap -c tclsh
#source flash_image.tcl
#openocd -f interface/stlink-v2.cfg -f m051_nuc1x_win.cfg -f flash_image.tcl
#openocd-0.8.0.exe -f interface/stlink-v2.cfg
# -f target_M0517_win.tcl -f target_M0517_linux.tcl
# -f M0517_unlock.tcl -f M0517_flash.tcl
#FlashAprom cheali-charger.bin
##
#

##
# register allocation
# --------------------
# r0  arg0 & return
# r1  arg1 
# r2  arg2 
##

proc FlashAprom { Filename } {
	set fl_base_adr 0
	set fl_bank 0
	
	set SRAM_BUF 0x20000120
	set PGM_FILE NU_M051x.bin
	set SRAM_SEK 7

	set BUF_SIZE [expr $SRAM_SEK*512]
			
	set f_size [file size $Filename]
	set f_35k [expr ($f_size / $BUF_SIZE)]
	set f_05k [expr ($f_size % $BUF_SIZE)]
	set f_sec [expr ((($f_size + 511) & ~511)/512)]
	
	set time_start [clock seconds]

	puts "Image: $Filename; Size=$f_size; Sectors:$f_sec; FlashProces:($f_size;$BUF_SIZE,$f_35k;$f_05k,1)"
	
	file mkdir tmp
	exec split -b $BUF_SIZE -d "$Filename" tmp/fl.	

	puts ">>>>     Load FlashPgm to SRAM: $PGM_FILE => 0x20000000"
	reset init
	load_image $PGM_FILE 0x20000000
	
	puts ">>>>     FlashInit:"
	reg sp 0x20001000 
	reg pc 0x20000000
   	resume
   	wait_halt
	set r0 [expr [lindex [ocd_reg 0] 2]]
	if { $r0 != 0 } { 
		puts "ERROR:: Not able to unlock"
	} 
	puts ">>>>     FlashInit stop:"

	set time_init [clock seconds]
#	puts "Trajanje init: [expr $time_init - $time_start] sec"
	
# r0	//address od 1. sector for erease
# r1	//number od sectors to erease 
	puts ">>>>     EreaseFlash: start"	
	puts "     FLASH sector addr: [format 0x%08x $fl_base_adr]"
	puts "     sectors to erease: [format 0x%08x $f_sec]"
   	 
	reg r0 [format 0x%08x $fl_base_adr]
	reg r1 [format 0x%08x $f_sec]
	reg sp 0x20001000
    reg pc 0x20000058
	resume
	wait_halt
	
	set r0 [expr [lindex [ocd_reg 0] 2]]
	if { $r0 != 0 } { 
		puts "ERROR:: Not able to erease"
	} 
	puts ">>>>    FlashErease: stop"

	set time_brisi [clock seconds]
#	puts "Trajanje brisi: [expr $time_brisi - $time_init] sec"
	
#r0 :: adr  // adresa gdje se kopira img
#r1 :: sz   // velicna bloka koji se kopira u bajtima
#r2 :: *buf // pointer na baffer sa podacima
	puts ">>>>   FLASH image: $Filename to [format 0x%08x $fl_base_adr]"		

	set fl_sec $fl_bank
	set fl_adr $fl_base_adr

	foreach f [lsort [glob tmp/fl.*]] { 
		set s [file size $f]
		set sn [expr (($s+511) & ~511)]
		set dn [expr ($sn/512)]
		
		puts ">> Flash Sector: $fl_sec-[expr ($fl_sec + $dn -1)] => [format 0x%08x $fl_adr] ($s)"
		puts "     SRAM load : $f => $SRAM_BUF"
		puts "     FLASH addr: reg r0 [format 0x%08x $fl_adr]"
		puts "     SIZE  addr: reg r1 [format 0x%08x $sn]"	
		puts "     BUFFR addr: reg r2 $SRAM_BUF"
  		          
		load_image $f $SRAM_BUF
		
		reg r0 [format 0x%08x $fl_adr]
		reg r1 [format 0x%08x $sn]
		reg r2 $SRAM_BUF
		reg sp 0x20001000
		reg pc 0x200000ae
		resume
		 
		wait_halt
		set r0 [expr [lindex [ocd_reg 0] 2]]
		if { $r0 != 0 } { 
			puts "ERROR:: FlashImage"
		} 
		set fl_adr [expr $fl_adr + $s]
		set fl_sec [expr $fl_sec + $dn]			
	}
	puts ">>>>   FLASH image: stop"
		
	foreach f [lsort [glob tmp/fl.*]] {
		exec rm $f
	}
	
	puts ">>>>    Verify: verify_image $Filename $fl_base_adr" 	
	verify_image $Filename $fl_base_adr
	
	set time_stop [clock seconds]
	
	puts "Trajanje init: [expr $time_init - $time_start] sec"
	puts "Trajanje brisi: [expr $time_brisi - $time_init] sec"
	puts "Trajanje pisi: [expr $time_stop - $time_brisi] sec"
	puts "Trajanje ukupno: [expr $time_stop - $time_start] sec"
	
	reset
}




