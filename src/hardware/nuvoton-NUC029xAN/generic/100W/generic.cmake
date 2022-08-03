
set(GENERIC_SOURCE
    CQ3.cpp
    CQ3.h
    CQ3-pins.h
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
