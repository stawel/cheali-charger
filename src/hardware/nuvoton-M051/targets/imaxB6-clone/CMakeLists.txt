
set(hardware imaxB6-clone)

set(SOURCE_FILES
    defaultCalibration.cpp
    HardwareConfig.h
)

CHEALI_CPU(nuvoton-M051)
CHEALI_GENERIC_CHARGER(50W)

add_definitions(-D__HXT=16000000UL) # External Crystal Clock Frequency

CHEALI_GENERATE_ARM_EXEC()
