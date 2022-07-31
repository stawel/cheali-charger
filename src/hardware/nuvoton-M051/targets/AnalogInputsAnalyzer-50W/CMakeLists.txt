set(SOURCE_FILES
    defaultCalibration.cpp
    HardwareConfig.h
)

CHEALI_HARDWARE(AnalogInputsAnalyzer-50W)
CHEALI_CPU(nuvoton-M051)
CHEALI_GENERIC_CHARGER(50W)

CHEALI_GENERATE_ARM_EXEC()
