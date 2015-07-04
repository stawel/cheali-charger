

set(CURRENT_PATH ${CMAKE_SOURCE_DIR}/src/hardware/${TARGET_CPU}/generic/${TARGET_GENERIC_CHARGER})

set(GENERIC_SOURCE
    GTPowerA6-10.cpp
    GTPowerA6-10.h
    AnalogInputsADC.cpp
    AnalogInputsADC.h

    Hardware.h
    HardwareConfigGeneric.h
)

include_directories(${CURRENT_PATH})

CHEALI_FIND(GENERIC_SOURCE_FILES "${GENERIC_SOURCE}" "${CURRENT_PATH}")
