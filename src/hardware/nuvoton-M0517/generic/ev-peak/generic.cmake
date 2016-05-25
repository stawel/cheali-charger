
set(GENERIC_SOURCE
    ev-peak.cpp
    ev-peak.h
    ev-peak-pins.h
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
