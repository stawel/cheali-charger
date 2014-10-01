
set(CURRENT_PATH ${CMAKE_SOURCE_DIR}/src/hardware/${TARGET_CPU}/cpu)

set(CPU_SOURCE
        cpu.cpp
        memory.cpp
        Serial.cpp
        Timer0.cpp
        TxHardSerial.cpp
        TxSoftSerial.cpp
        UtilsD.cpp
)

include_directories(${CURRENT_PATH})

CHEALI_FIND(CPU_SOURCE_FILES "${CPU_SOURCE}" "${CURRENT_PATH}")

include(${CURRENT_PATH}/CMSIS/cmsis.cmake)

link_libraries(m)

