cmake_minimum_required(VERSION 2.6)

Project(cheali-charger)

set(cheali-charger-version 0.21)
set(cheali-charger-eeprom-version 4)

configure_file(src/core/Version.h.in src/core/Version.h)

add_subdirectory(arduino)
add_subdirectory(src/core)
add_subdirectory(src/hardware/GTPowerA6-10-original)
add_subdirectory(src/hardware/imaxB6-original)
add_subdirectory(src/hardware/imaxB6-clone)

