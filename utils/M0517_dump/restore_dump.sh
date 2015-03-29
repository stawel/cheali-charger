#!/bin/bash


MEM_DUMP=memdump.txt
REG_DUMP=regdump.txt

./dump_to_mww.sh $MEM_DUMP | nc localhost 4444
./dump_to_reg.sh $REG_DUMP | nc localhost 4444