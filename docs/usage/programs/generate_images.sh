#!/bin/bash

GEN=../../../utils/lcdScreenshotGenerator/generate.py
GEN2=../../../utils/lcdScreenshotGenerator/generate2.py


RESIZE=100%

EMPTY=$'\x10'
HALF=$'\x01'
FULL=$'\x02'

#$GEN ">options "        "i_options.png"   $RESIZE

$GEN2 "m$HALF$EMPTY$FULL$FULL$HALF  1:4.199V" "2:4.197V3:4.144V"  "balancing.png"    $RESIZE
$GEN "$HALF"   "half.png"   $RESIZE
$GEN "$FULL"   "full.png"   $RESIZE
$GEN "$EMPTY"   "empty.png"   $RESIZE

