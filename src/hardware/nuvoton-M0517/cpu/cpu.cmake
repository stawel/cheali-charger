
set(CURRENT_PATH ${CMAKE_SOURCE_DIR}/src/hardware/${TARGET_CPU}/cpu)

set(CPU_SOURCE
    atomic.h  cpu.h  irq_priority.h  memory.h    Serial.h    TxHardSerial.cpp  TxSoftSerial.cpp  UtilsD.cpp
    cpu.cpp   IO.h   memory.cpp      Serial.cpp  Timer0.cpp  TxHardSerial.h    TxSoftSerial.h    other.c
    startup_M051Series.c
    StackInfo.cpp
    IO.cpp
)

include_directories(${CURRENT_PATH})

CHEALI_FIND(CPU_SOURCE_FILES "${CPU_SOURCE}" "${CURRENT_PATH}")

include(${CURRENT_PATH}/CMSIS/cmsis.cmake)

link_libraries(m)

