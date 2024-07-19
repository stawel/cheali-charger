
set(GENERIC_SOURCE
    C3.cpp
    C3.h
    C3-pins.h
    SMPS_PID.h
    SMPS_PID.cpp
    AnalogInputsADC.cpp
    AnalogInputsADC.h

    outputPWM.cpp
    outputPWM.h

    Hardware.h
    HardwareConfigGeneric.h
)

CHEALI_ADD(GENERIC_SOURCE_FILES "${GENERIC_SOURCE}")
