cmake_minimum_required(VERSION 2.6)

SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_C_COMPILER avr-gcc)
SET(CMAKE_CXX_COMPILER avr-g++)

Project(cheali-charger-mod)


set(cheali-charger-version 0.33m)
set(cheali-charger-eeprom-calibration-version 56)
set(cheali-charger-eeprom-programdata-version 56)
set(cheali-charger-eeprom-settings-version 56)



configure_file(src/core/Version.h.in src/core/Version.h)


if(ARM-Cortex-M0)
    message(STATUS "target: ARM-Cortex-M0")
    include(arm-compiler.cmake)
    add_subdirectory(src/core)
    add_subdirectory(src/hardware/imaxB6-clone-ARM-Cortex-M0)
else(ARM-Cortex-M0)
    message(STATUS "target: avr")
    include(avr-compiler.cmake)
    add_subdirectory(src/core/atmega32)

    add_subdirectory(src/hardware/200W)
    add_subdirectory(src/hardware/200W-commondisplay)
    add_subdirectory(src/hardware/200W-high-discharge-indus-mod)
    add_subdirectory(src/hardware/50W-5A)
    add_subdirectory(src/hardware/50W-6A)
    add_subdirectory(src/hardware/80W)
    add_subdirectory(src/hardware/400W)
    add_subdirectory(src/hardware/imaxB6ChalusBomod)
    add_subdirectory(src/hardware/Turnigy-Accucel-8-150W-7A)
endif(ARM-Cortex-M0)


