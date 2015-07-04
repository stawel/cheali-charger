#!/bin/bash

#TODO: fix
#sed "s/                                                                                                                                                             //g" < memdump.txt > memdump2.txt
cat e.txt | sed 's/\([^ ][^ ]\)\([^ ][^ ]\)\([^ ][^ ]\)\([^ ][^ ]\)/\4\3\2\1/g' > e2.txt