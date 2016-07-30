#!/bin/bash

GEN=../../../utils/lcdScreenshotGenerator/generate.py
GEN2=../../../utils/lcdScreenshotGenerator/generate2.py


RESIZE=50%

function STRING {
    $GEN "$2" "i_$1.png" $3 > /dev/null
    echo "|![$1](i_$1.png)         |   |   |"
}


STRING backlight    " backlight:   70"
STRING fanOn        " fan on: Tempera"
STRING fanTempOn    " |fan T on:50.0C"
STRING dischOff     " disch off:  60C"
STRING AudioBeep    " beep:       yes"
STRING minIc        " min Ic:    50mA"
STRING maxIc        " max Ic: 10000mA"
STRING minId        " min Id:    50mA"
STRING maxId        " max Id:  5000mA"
STRING inputLow     " input low:  10V"
STRING adcNoise     " adc noise:   no"
STRING UARTview     " UART:  disabled"
STRING UARTspeed    " |speed:   57600"
STRING UARToutput   " |output:   temp"
STRING MenuType     " menus:   simple"
STRING MenuButtons  " buttons:    rev"
STRING reset        " reset          "

#    //UARToutput menu
STRING temp     "temp"  $RESIZE
STRING pin7     "pin7"  $RESIZE
STRING pin38    "pin38" $RESIZE

#    //UART view menu
STRING disable      "disabled"  $RESIZE
STRING normal       "normal"  $RESIZE
STRING debug        "debug"  $RESIZE
STRING extDebug     "ext. deb"  $RESIZE
STRING extDebugAdc  "ext. Adc"  $RESIZE

#    //fanOn reason menu
#//  STRING(disable,     "disabled"); -- defined in UART view
STRING always       "always"  $RESIZE
STRING FanProgram   "program"  $RESIZE
STRING temperature  "Tempera"  $RESIZE
STRING tempProgram  "T-progr"  $RESIZE

#    //MenuType
STRING simple       "simple"   $RESIZE
STRING advanced     "advanced"  $RESIZE

#    //MenuButtons
#    //STRING(normal,      "normal"); - defined in UART view
STRING reversed       "rev"   $RESIZE

