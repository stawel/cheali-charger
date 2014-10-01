

set(CURRENT_DIR ${CORE_DIR}/display)

set(CORE_SOURCE
Blink.cpp  Blink.h  DummyLiquidCrystal.h  LcdPrint.cpp  LcdPrint.h  LiquidCrystal.cpp  LiquidCrystal.h
)

include_directories(${CURRENT_DIR})

CHEALI_FIND("CORE_SOURCE_FILES" "${CORE_SOURCE}" "${CURRENT_DIR}" )

include(${CORE_DIR}/display/menus/menus.cmake)
include(${CORE_DIR}/display/screens/screens.cmake)


