

set(CURRENT_DIR ${CORE_DIR}/screens)

set(CORE_SOURCE
    ScreenBalancer.cpp  Screen.cpp       ScreenCycle.h  ScreenMethods.cpp  ScreenPages.h  ScreenStartInfo.cpp
    ScreenBalancer.h    ScreenCycle.cpp  Screen.h       ScreenMethods.h    screens.cmake  ScreenStartInfo.h
)

include_directories(${CURRENT_DIR})

CHEALI_FIND("CORE_SOURCE_FILES" "${CORE_SOURCE}" "${CURRENT_DIR}" )



