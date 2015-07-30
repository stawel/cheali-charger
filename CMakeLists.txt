cmake_minimum_required(VERSION 2.8.11)

SET(CMAKE_SYSTEM_NAME Generic)

if(ARM-Cortex-M0)
    message(STATUS "target architecture: ARM-Cortex-M0")
    include(arm-compiler.cmake)
else(ARM-Cortex-M0)
    message(STATUS "target architecture: avr")
    include(avr-compiler.cmake)
endif(ARM-Cortex-M0)

Project(cheali-charger)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(UnixCommands)
find_package(ChealiChargerInternal REQUIRED)

string(TIMESTAMP timestamp "%Y%m%d")

set(cheali-charger-version 1.99)
set(cheali-charger-eeprom-calibration-version 9)
set(cheali-charger-eeprom-programdata-version 3)
set(cheali-charger-eeprom-settings-version 10)
set(cheali-charger-buildnumber ${timestamp})

message(STATUS "cheali-charger-version: ${cheali-charger-version}")
message(STATUS "cheali-charger-eeprom-calibration-version: ${cheali-charger-eeprom-calibration-version}")
message(STATUS "cheali-charger-eeprom-programdata-version: ${cheali-charger-eeprom-programdata-version}")
message(STATUS "cheali-charger-eeprom-settings-version: ${cheali-charger-eeprom-settings-version}")
message(STATUS "cheali-charger-buildnumber: ${cheali-charger-buildnumber}")

configure_file(src/core/Version.h.in src/core/Version.h)

if(ARM-Cortex-M0)
    message(STATUS "target architecture: ARM-Cortex-M0")
    include(arm-compiler.cmake)
    add_subdirectory(src/hardware/nuvoton-M0517)
else(ARM-Cortex-M0)
    message(STATUS "target architecture: avr")
    include(avr-compiler.cmake)
    add_subdirectory(src/hardware/atmega32)
endif(ARM-Cortex-M0)
