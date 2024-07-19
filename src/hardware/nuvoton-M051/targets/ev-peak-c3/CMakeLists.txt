
set(hardware ev-peak-c3)

set(SOURCE_FILES
    defaultCalibration.cpp
    HardwareConfig.h
)

CHEALI_CPU(nuvoton-M051)
CHEALI_GENERIC_CHARGER(ev-peak-c3)

add_definitions(-D__HXT=12000000UL) # External Crystal Clock Frequency

CHEALI_GENERATE_ARM_EXEC()
