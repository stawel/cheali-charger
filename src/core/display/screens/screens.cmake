

set(CURRENT_DIR ${CORE_DIR}/display/screens)

set(CORE_SOURCE
    standard/Screen.cpp
    standard/Screen.h
    standard/ScreenBalancer.cpp
    standard/ScreenBalancer.h
    standard/ScreenCycle.cpp
    standard/ScreenCycle.h
    standard/ScreenMethods.cpp
    standard/ScreenMethods.h
    standard/ScreenStartInfo.cpp
    standard/ScreenStartInfo.h
)

include_directories(${CURRENT_DIR}/standard)

CHEALI_FIND("CORE_SOURCE_FILES" "${CORE_SOURCE}" "${CURRENT_DIR}" )



