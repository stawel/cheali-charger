
include(${CMAKE_SOURCE_DIR}/src/core/CMakeLists.txt)

set(ATMEGA32_SOURCE
        memory.h
        Timer.cpp
        Timer.h
        Serial.h
        Serial.cpp
        StackInfo.cpp
        StackInfo.h
)

foreach (SOURCE ${ATMEGA32_SOURCE})
    set(THIS_FILE THIS_FILE-NOTFOUND)
    find_file(THIS_FILE ${SOURCE} PATHS ${CMAKE_SOURCE_DIR}/src/core/atmega32 NO_DEFAULT_PATH)
#    message("append ${THIS_FILE}, ${SOURCE}")
    list (APPEND CORE_SOURCE_FILES ${THIS_FILE})
endforeach()

