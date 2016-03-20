
set(CPU_SOURCE
        memory.h
        Timer2.cpp

        Timer0.cpp
        Timer0.h

        Timer1.cpp
        Timer1.h

        Serial.h

        StackInfo.cpp

        IO.h
        UtilsD.cpp
        atomic.h
        cpu.h

        config.h

)

CHEALI_ADD(CPU_SOURCE_FILES "${CPU_SOURCE}")

include_directories(${CMAKE_CURRENT_LIST_DIR}/..)
#include_directories(/usr/lib/avr/include/)
#link_libraries(m)

