

set(CURRENT_DIR ${CORE_DIR}/calibration)

set(CORE_SOURCE
    Calibrate.cpp
    Calibrate.h
)

include_directories(${CURRENT_DIR})

CHEALI_FIND("CORE_SOURCE_FILES" "${CORE_SOURCE}" "${CURRENT_DIR}" )



