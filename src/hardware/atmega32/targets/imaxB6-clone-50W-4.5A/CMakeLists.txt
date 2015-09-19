
set(SOURCE_FILES
    defaultCalibration.cpp
    HardwareConfig.h
)

CHEALI_HARDWARE(imaxB6-clone-50W-4.5A)
CHEALI_CPU(atmega32)
CHEALI_GENERIC_CHARGER(50W)

CHEALI_GENERATE_AVR_EXEC()
