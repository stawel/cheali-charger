
set(SOURCE_FILES
    defaultCalibration.cpp
    HardwareConfig.h
)

CHEALI_HARDWARE(imaxB6-clone)
CHEALI_CPU(atmega32)
CHEALI_GENERIC_CHARGER(50W)

CHEALI_GENERATE_AVR_EXEC()
