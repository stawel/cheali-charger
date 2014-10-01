
set(SOURCE_FILES
    defaultCalibration.cpp
    HardwareConfig.h
)

CHEALI_HARDWARE(Turnigy-Accucel-8-150W-7A)
CHEALI_CPU(atmega32)
CHEALI_GENERIC_CHARGER(200W)

CHEALI_GENERATE_AVR_EXEC()
