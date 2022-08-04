
set(CURRENT_PATH ${CMAKE_CURRENT_LIST_DIR})

set(CPU_SOURCE
    atomic.h  cpu.h  irq_priority.h  memory.h    Serial.h    TxHardSerial.cpp  UtilsD.cpp
    cpu.cpp   IO.h   memory.cpp      Serial.cpp  Timer0.cpp  TxHardSerial.h    StackInfo.cpp
    IO.cpp
)

if (NOT "${serial}" STREQUAL "hardware-only")
  list(APPEND CPU_SOURCE TxSoftSerial.cpp TxSoftSerial.h)
endif()

CHEALI_ADD(CPU_SOURCE_FILES "${CPU_SOURCE}")

include_directories(${CMAKE_CURRENT_LIST_DIR}/..)
include(${CURRENT_PATH}/CMSIS/cmsis.cmake)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-T${CPU_LINKER_SCRIPT}")

link_libraries(m)

