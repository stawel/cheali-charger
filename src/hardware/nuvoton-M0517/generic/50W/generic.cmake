
set(GENERIC_SOURCE
    imaxB6.cpp
    imaxB6.h
    imaxB6-pins.h
    SMPS_PID.h
    SMPS_PID.cpp
    AnalogInputsADC.cpp
    AnalogInputsADC.h

    outputPWM.cpp
    outputPWM.h

    Hardware.h
    HardwareConfigGeneric.h

    main.cpp
)

CHEALI_ADD(GENERIC_SOURCE_FILES "${GENERIC_SOURCE}")
