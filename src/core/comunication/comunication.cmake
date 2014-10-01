

set(CURRENT_DIR ${CORE_DIR}/comunication)

set(CORE_SOURCE
    SerialLog.cpp
    SerialLog.h
)

include_directories(${CURRENT_DIR})

CHEALI_FIND("CORE_SOURCE_FILES" "${CORE_SOURCE}" "${CURRENT_DIR}" )



