
set(CURRENT_PATH ${CMAKE_SOURCE_DIR}/src/hardware/${TARGET_CPU}/cpu)

set(CPU_SOURCE
        memory.h
        Timer2.cpp

        Timer0.cpp
        Timer0.h

        Timer1.cpp
        Timer1.h

        Serial.h

        HardwareSerial.h
        HardwareSerial.cpp

        StackInfo.cpp

        IO.h
        Utils.cpp
        atomic.h
        cpu.h

)

include_directories(${CURRENT_PATH})

CHEALI_FIND(CPU_SOURCE_FILES "${CPU_SOURCE}" "${CURRENT_PATH}")

include_directories(/usr/lib/avr/include/)
link_libraries(m)

