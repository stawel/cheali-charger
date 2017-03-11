cmake_minimum_required(VERSION 2.8.11)

Project(cheali-charger)

if("${CMAKE_HOST_SYSTEM_NAME}" MATCHES "[Ww]in")
        message(STATUS "windows detected: setting enable-short-names-default to ON")
        set(enable-short-names-default ON)
else()
        set(enable-short-names-default OFF)
endif()

option(enable-short-names "use short names for firmware file" ${enable-short-names-default})
message(STATUS "firmware file short names: ${enable-short-names}")

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(UnixCommands)
find_package(ChealiChargerInternal REQUIRED)

string(TIMESTAMP timestamp "%Y%m%d")

set(cheali-charger-version 2.01)
set(cheali-charger-eeprom-calibration-version 10)
set(cheali-charger-eeprom-programdata-version 3)
set(cheali-charger-eeprom-settings-version 12)
set(cheali-charger-eeprom-version-string "e${cheali-charger-eeprom-calibration-version}.${cheali-charger-eeprom-programdata-version}.${cheali-charger-eeprom-settings-version}")
set(cheali-charger-buildnumber ${timestamp})

message(STATUS "cheali-charger-version: ${cheali-charger-version}")
message(STATUS "cheali-charger-eeprom-calibration-version: ${cheali-charger-eeprom-calibration-version}")
message(STATUS "cheali-charger-eeprom-programdata-version: ${cheali-charger-eeprom-programdata-version}")
message(STATUS "cheali-charger-eeprom-settings-version: ${cheali-charger-eeprom-settings-version}")
message(STATUS "cheali-charger-eeprom-version-string: ${cheali-charger-eeprom-version-string}")
message(STATUS "cheali-charger-buildnumber: ${cheali-charger-buildnumber}")
if(enable-short-names)
    message(WARNING "short names enabled!")
endif(enable-short-names)

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
