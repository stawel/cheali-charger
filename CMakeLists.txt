cmake_minimum_required(VERSION 2.6)

Project(cheali-charger)

set(cheali-charger-version 0.31)
set(cheali-charger-eeprom-calibration-version 7)
set(cheali-charger-eeprom-programdata-version 1)
set(cheali-charger-eeprom-settings-version 2)

configure_file(src/core/Version.h.in src/core/Version.h)

add_subdirectory(arduino)
add_subdirectory(src/core)
add_subdirectory(src/hardware/GTPowerA6-10-original)
add_subdirectory(src/hardware/imaxB6-original)
add_subdirectory(src/hardware/imaxB6-clone)
add_subdirectory(src/hardware/Turnigy-MEGA-400Wx2)
add_subdirectory(src/hardware/Turnigy-A-6-10-200W)
add_subdirectory(src/hardware/Turnigy-Accucel-6-50W-5A)
add_subdirectory(src/hardware/Turnigy-Accucel-6-50W-6A)

