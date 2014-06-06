cmake_minimum_required(VERSION 2.6)

MACRO (TODAY RESULT) 
        EXECUTE_PROCESS(COMMAND "date" "+%Y%m%d" OUTPUT_VARIABLE ${RESULT}) 
        string(REGEX REPLACE "(..)/(..)/..(..).*" "\\1\\2\\3" ${RESULT} ${${RESULT}}) 
        string(REGEX REPLACE "(\r?\n)+$" "" ${RESULT} ${${RESULT}}) 
ENDMACRO (TODAY) 





SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_C_COMPILER avr-gcc)
SET(CMAKE_CXX_COMPILER avr-g++)

Project(cheali-charger-mod)


  

TODAY(RESULT)
set(cheali-charger-buildnumber ${RESULT})
set(cheali-charger-version 0.33m)
set(cheali-charger-eeprom-calibration-version 8)
set(cheali-charger-eeprom-programdata-version 51)
set(cheali-charger-eeprom-settings-version 56)



configure_file(src/core/Version.h.in src/core/Version.h)


if(ARM-Cortex-M0)
    message(STATUS "target: ARM-Cortex-M0")
    include(arm-compiler.cmake)
    add_subdirectory(src/core)
    add_subdirectory(src/hardware/imaxB6-clone-ARM-Cortex-M0)
else(ARM-Cortex-M0)
    message(STATUS "target: avr")
    message(STATUS "Compilation date = ${RESULT}") 

    include(avr-compiler.cmake)
    add_subdirectory(src/core/atmega32)

    add_subdirectory(src/hardware/50W-5A)
   
endif(ARM-Cortex-M0)


