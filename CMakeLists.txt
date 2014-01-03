cmake_minimum_required(VERSION 2.6)

Project(cheali-charger-mod)

set(cheali-charger-version 0.32m)
set(cheali-charger-eeprom-calibration-version 52)
set(cheali-charger-eeprom-programdata-version 52)  # TODO_NJ: for testing
set(cheali-charger-eeprom-settings-version 54)

configure_file(src/core/Version.h.in src/core/Version.h)

add_subdirectory(arduino)
add_subdirectory(src/core)
add_subdirectory(src/hardware/200W)
add_subdirectory(src/hardware/200W-commondisplay)
add_subdirectory(src/hardware/50W-5A)
add_subdirectory(src/hardware/50W-6A)
add_subdirectory(src/hardware/80W)
add_subdirectory(src/hardware/400W)
add_subdirectory(src/hardware/imaxB6ChalusBomod)




