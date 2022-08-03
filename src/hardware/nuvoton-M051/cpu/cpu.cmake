
set(CURRENT_PATH ${CMAKE_CURRENT_LIST_DIR})

set(CPU_SOURCE
    atomic.h  cpu.h  irq_priority.h  memory.h    Serial.h    TxHardSerial.cpp  TxSoftSerial.cpp  UtilsD.cpp
    cpu.cpp   IO.h   memory.cpp      Serial.cpp  Timer0.cpp  TxHardSerial.h    TxSoftSerial.h    other.c
    startup_M051Series.c
    StackInfo.cpp
    IO.cpp
)

CHEALI_ADD(CPU_SOURCE_FILES "${CPU_SOURCE}")

include_directories(${CMAKE_CURRENT_LIST_DIR}/..)
include(${CURRENT_PATH}/CMSIS/cmsis.cmake)

link_libraries(m)

