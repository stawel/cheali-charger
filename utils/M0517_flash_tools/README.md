# M0517_flash_tools
Flash tools for Nuvoton M0517LBN CPU for use with OpenOCD TCL scripts

start OpenOCD
win: openocd-0.8.0.exe -f interface/stlink-v2.cfg -f target_M0517_win.tcl
linux: openocd -f interface/stlink-v2.cfg -f target_M0517_linux.tcl

flash:
source M0517_flash.tcl
FlashAprom cheali-charger.bin

unlock:
source M0517_flash.tcl
ReadConf
EraseChip
WriteConf
