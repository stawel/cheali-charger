

set(CURRENT_DIR ${CORE_DIR}/drivers)

set(CORE_SOURCE
    cprintf.cpp  Blink.cpp  Buzzer.cpp  Keyboard.h     LcdPrint.h    LiquidCrystal.h    PolarityCheck.h    SerialLog.h      Time.cpp
    cprintf.h    Blink.h    Buzzer.h    Keyboard.cpp   LcdPrint.cpp  LiquidCrystal.cpp  PolarityCheck.cpp  SerialLog.cpp    StackInfo.h  Time.h
)

include_directories(${CURRENT_DIR})

CHEALI_FIND("CORE_SOURCE_FILES" "${CORE_SOURCE}" "${CURRENT_DIR}" )



