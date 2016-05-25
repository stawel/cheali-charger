
set(hardware ev-peak-cq3)

set(SOURCE_FILES
    defaultCalibration.cpp
    HardwareConfig.h
)

CHEALI_CPU(nuvoton-M0517)
CHEALI_GENERIC_CHARGER(ev-peak)

CHEALI_GENERATE_ARM_EXEC()
