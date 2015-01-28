#
# https://github.com/hackocopter/SWD-Hacking/blob/master/Nulink-Logs/Chip%20erase%20sequence.txt
# https://gist.github.com/TheLastMutt/d1c1948acaace7444c1c#file-mini51-cfg-L175
#
# https://github.com/hackocopter/SWD-Hacking/blob/master/KEIL-Flashtools/Mini51flashtools.ini
# Ported from KEIL to OpenOCD tcl language and added some somments.
# The chip erase sequence got reverse engineered using a Nulink programmer, a logic analyzer
# and custom SWD log parser software.
# Info here:
# https://github.com/hackocopter/SWD-Hacking
# https://www.mikrocontroller.net/topic/309185 (German forum)
 
# This unlocks access to protected registers
# by writing to REGWRPROT register.


# ISPCON; // offset 0x000 R/W ISP Control Register
# ISPADR; // offset 0x004 R/W ISP Address Register
# ISPDAT; // offset 0x008 R/W ISP Data Register
# ISPCMD; // offset 0x00C R/W ISP Command Register
# ISPTRG; // offset 0x010 R/W ISP Trigger Register
# DFBADR; // offset 0x014 R Data Flash Start Address 
# FATCON; // offset 0x018 R/W Flash Access Window Control Registerproc ErasePage {adr} {
# ICPCON; // offset 0x01C R/W ??	mww 0x5000c000 0x33
# RMPCON; // offset 0x020 R/W ??	mww 0x5000c00c 0x22

# // ISP Command (ISPCMD) definitions
# FMC_STANDBY    0x30
# FMC_READ       0x00
# FMC_PROGRAM    0x21
# FMC_PAGE_ERASE 0x22
# FMC_READ_CID   0x0B
# FMC_READ_DID   0x0C


proc mrw {adr} {
	set v ""
	mem2array v 32 $adr 1
	return $v(0)
}

proc UnlockFlash {} {
	# # Halt target
	# mww 0xe000edf0 0x05f0003
	 
	# # # ?? Something Debug access port / Breakpoint unit
	# mww 0xe0002008 0x000000
	# mww 0xe000200C 0x000000
	# mww 0xe0002010 0x000000
	# mww 0xe0002014 0x000000
	 
	# Unlock sequence for protected registers
	mww 0x50000100 0x59
	mww 0x50000100 0x16
	mww 0x50000100 0x88
}

proc InitFash {} {
	reset init
	load_image NU_M051x.bin 0x20000000
	reg sp 0x20001000 
	reg pc 0x20000000
   	resume
   	wait_halt
	set r0 [expr [lindex [ocd_reg 0] 2]]
	if { $r0 != 0 } { 
		puts "ERROR:: Not able to unlock"
	} 
}


proc CHIP_RST {} {
	mww 0x50000100 0x59
	mww 0x50000100 0x16
	mww 0x50000100 0x88
	
	mww 0x50000008 1
}
 
proc ISP_Write {adr dat} {
#	mww 0x5000c000 0x31
	mww 0x5000c00c 0x21
	mww 0x5000c004 $adr
	mww 0x5000c008 $dat
	mww 0x5000c010 1
	while {[mrw 0x5000c010] != 0} {
		puts "."
	} 
	if { [expr {[mrw 0x5000c000] & 0x40}] } {
		puts "ISP Error"
		return
	}
#	mww 0x5000c000 0x30
} 

proc ISP_Read {adr} {
	mww 0x5000c000 0x31
	mww 0x5000c00c 0x00
	mww 0x5000c004 $adr
	mww 0x5000c010 1
	while {[mrw 0x5000c010] != 0} {
		puts "."
	}
	set out [mrw 0x5000c008]
#	mww 0x5000c000 0x30
	return $out
}
 
   
proc ErasePage {adr} {
	mww 0x5000c000 0x31
	mww 0x5000c00c 0x22
	mww 0x5000c004 $adr
	mww 0x5000c010 1
	while {[mrw 0x5000c010] != 0} {
		puts "."
	}
	if { [expr {[mrw 0x5000c000] & 0x40}] } {
		puts "ISP Error"
		return
	}	 
#	mww 0x5000c000 0x30
}
 

proc WriteConf {} {
	UnlockFlash
	ErasePage 0x300000
	ISP_Write 0x300000 0xF8FFFFFF
#	ISP_Write 0x300004 0x1F000
	puts "User config written"
}
 
proc ReadConf {} {
	puts "Reading User Config. registers"
#	reset init
	UnlockFlash
	set conf0 [ISP_Read 0x300000]
	set conf1 [ISP_Read 0x300004]
	set id [mrw 0x50000000]
	puts [format "Config0 (0x00300000):0x%X" $conf0]
	puts [format "Config1 (0x00300004):0x%X" $conf1]
	puts [format "Device ID :0x%X" $id]
	if {[expr {($conf0 & 2)}]} {
		puts "Flash is not locked!"
	} else {
		puts "Flash is locked!\n  to erase whole chip perform:"
		puts "  => EraseChip"
		puts "  => WriteConf"
	}
}
 

proc EraseChip {} {
	UnlockFlash
	set conf0 [ISP_Read 0x300000]
	if {[expr {$conf0 & 2}]} {
		puts "Flash is not locked!"
		return
	}
	puts "Flash is locked!"
	mww 0x5000c000 0x31
	mww 0x5000c01c 0x01
	if { [expr {[mrw 0x5000c000] & 0x40}] } {
		puts "ISP Error"
		return
	}
	if {[mrw 0x5000c010] != 0} {
		puts "ISP error Busy"
		return
	}
	# Erase-Chip
	mww 0x5000c00c 0x26
	mww 0x5000c004 0
	puts "Chip erase..."
	mww 0x5000c010 1
	while {[mrw 0x5000c010] != 0} {
		puts "."
	} 
	if { [expr {[mrw 0x5000c000] & 0x40}] } {
		puts "ISP Error"
		return
	}
#	mww 0x5000c000 0x30

	set t_adr 0x00000000
	if {[ISP_Read $t_adr] == 0xffffffff } {
		set err "Erased!"
	} else {
		set err "Erase Error!"
	}
	puts [format "APROM: $err: ($t_adr):0x%X" [ISP_Read $t_adr]]

	set t_adr 0x00100000
	if {[ISP_Read $t_adr] == 0xffffffff } {
		set err "Erased!"
	} else {
		set err "Erase Error!"
	}
	puts [format "LDROM: $err: ($t_adr):0x%X" [ISP_Read $t_adr]]

	set t_adr 0x0030000
	if {[ISP_Read $t_adr] == 0xffffffff } {
		set err "Erased!"
	} else {
		set err "Erase Error!"
	}
	puts [format "Config: $err: ($t_adr):0x%X" [ISP_Read $t_adr]]
	
	mww 0x5000c01c 0
} 