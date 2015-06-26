
set(SOURCE_FILES
    defaultCalibration.cpp
    HardwareConfig.h
)

CHEALI_HARDWARE(ADCKeyboardAnalyzer-150W)
CHEALI_CPU(atmega32)
CHEALI_GENERIC_CHARGER(adc_key_test)

CHEALI_GENERATE_AVR_EXEC()
