# script for nuvoton M0517 OpenOCD windows

source [find target/swj-dp.tcl]

set _WORKAREASIZE 0x1000
set _CPUTAPID 0x0bb11477
set _CHIPNAME M0517

transport select hla_swd
hla newtap $_CHIPNAME cpu -expected-id $_CPUTAPID

set _TARGETNAME $_CHIPNAME.cpu
target create $_TARGETNAME hla_target -chain-position $_TARGETNAME

$_TARGETNAME configure -work-area-phys 0x20000000 -work-area-size $_WORKAREASIZE -work-area-backup 0

# adapter_khz 1000
# adapter_nsrst_delay 100

