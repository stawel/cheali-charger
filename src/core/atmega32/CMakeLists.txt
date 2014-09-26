
include(${CMAKE_SOURCE_DIR}/src/core/CMakeLists.txt)

set(ATMEGA32_SOURCE
        memory.h
        Timer2.cpp

        Timer0.cpp
        Timer0.h

        Timer1.cpp
        Timer1.h

        Serial.h
        Serial.cpp

        StackInfo.cpp

        IO.h
        Utils.cpp
        atomic.h
        cpu.h
)

foreach (SOURCE ${ATMEGA32_SOURCE})
    set(THIS_FILE THIS_FILE-NOTFOUND)
    find_file(THIS_FILE ${SOURCE} PATHS ${CMAKE_SOURCE_DIR}/src/core/atmega32 NO_DEFAULT_PATH)
#    message("append ${THIS_FILE}, ${SOURCE}")
    list (APPEND CORE_SOURCE_FILES ${THIS_FILE})
endforeach()

