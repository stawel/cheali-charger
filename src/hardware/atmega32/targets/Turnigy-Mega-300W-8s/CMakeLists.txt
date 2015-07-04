
set(SOURCE_FILES
    defaultCalibration.cpp
    HardwareConfig.h
)

CHEALI_HARDWARE(Turnigy-Mega-300W-8s)
CHEALI_CPU(atmega32)
CHEALI_GENERIC_CHARGER(200W)

CHEALI_GENERATE_AVR_EXEC()
