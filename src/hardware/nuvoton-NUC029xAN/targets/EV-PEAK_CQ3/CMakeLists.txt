set(hardware EV-PEAK_CQ3)

set(SOURCE_FILES
    defaultCalibration.cpp
    HardwareConfig.h
)

CHEALI_CPU(nuvoton-NUC029xAN)
CHEALI_GENERIC_CHARGER(100W)

CHEALI_GENERATE_ARM_EXEC()
