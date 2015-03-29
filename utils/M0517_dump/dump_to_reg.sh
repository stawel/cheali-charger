#!/bin/bash


sed 's/([0-9]*) \([^ ]*\).*: /reg \1 /' < $1
# > tmp
#sed 's/\([^ ]*\) \([^ ]*\) \([^ ]*\)/echo mww \$\(\(\1 + \2\)\) 0x\3/' < tmp > tmp2
#chmod a+x tmp2
# ./tmp2
