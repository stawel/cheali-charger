
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

CHEALI_ADD(CPU_SOURCE_FILES "${CPU_SOURCE}")

include_directories(/usr/lib/avr/include/)
link_libraries(m)

