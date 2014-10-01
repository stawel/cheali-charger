

set(CORE_DIR ${CMAKE_SOURCE_DIR}/src/core/)

set(CORE_SOURCE
        ChealiCharger2.cpp
        ChealiCharger2.h

        Utils.cpp
        Utils.h

        strings/standard.h
        strings/strings_common.h
        strings/strings.cpp
)

SET_SOURCE_FILES_PROPERTIES(${CORE_DIR}/strings/strings.cpp PROPERTIES OBJECT_DEPENDS ${CORE_DIR}/strings/standard.h)


include_directories(${CORE_DIR})

CHEALI_FIND("CORE_SOURCE_FILES" "${CORE_SOURCE}" "${CORE_DIR}" )

include(${CORE_DIR}/analog/analog.cmake)
include(${CORE_DIR}/calibration/calibration.cmake)
include(${CORE_DIR}/comunication/comunication.cmake)
include(${CORE_DIR}/display/display.cmake)
include(${CORE_DIR}/hardware/hardware.cmake)


