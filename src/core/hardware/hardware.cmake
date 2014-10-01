

set(CURRENT_DIR ${CORE_DIR}/hardware)

set(CORE_SOURCE
   Buzzer.cpp  Buzzer.h  Keyboard.cpp  Keyboard.h  StackInfo.h  Time.cpp  Time.h
)

include_directories(${CURRENT_DIR})

CHEALI_FIND("CORE_SOURCE_FILES" "${CORE_SOURCE}" "${CURRENT_DIR}" )



