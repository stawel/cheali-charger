

set(CURRENT_DIR ${CORE_DIR}/display/screens)

set(CORE_SOURCE
    standard/Screen.cpp
    standard/Screen.h
)

include_directories(${CURRENT_DIR}/standard)

CHEALI_FIND("CORE_SOURCE_FILES" "${CORE_SOURCE}" "${CURRENT_DIR}" )



