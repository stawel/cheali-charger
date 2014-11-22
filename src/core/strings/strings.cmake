

set(CURRENT_DIR ${CORE_DIR}/strings)

set(CORE_SOURCE
    standard.h  strings_common.h  strings.cpp
)

include_directories(${CURRENT_DIR})

CHEALI_FIND("CORE_SOURCE_FILES" "${CORE_SOURCE}" "${CURRENT_DIR}" )



