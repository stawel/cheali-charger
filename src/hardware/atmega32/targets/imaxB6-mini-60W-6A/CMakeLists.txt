
set(SOURCE_FILES
    defaultCalibration.cpp
    HardwareConfig.h
)

CHEALI_HARDWARE(imaxB6-mini-60W-6A)
CHEALI_CPU(atmega32)
CHEALI_GENERIC_CHARGER(50W)

CHEALI_GENERATE_AVR_EXEC()
