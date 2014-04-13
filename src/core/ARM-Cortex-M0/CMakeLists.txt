
include(${CMAKE_SOURCE_DIR}/src/core/CMakeLists.txt)

set(ARM_Cortex_M0_SOURCE
        memory.h
        Timer2.cpp

        IO.h
        Utils.cpp
        atomic.h
        cpu.h
)

foreach (SOURCE ${ARM_Cortex_M0_SOURCE})
    set(THIS_FILE THIS_FILE-NOTFOUND)
    find_file(THIS_FILE ${SOURCE} PATHS ${CMAKE_SOURCE_DIR}/src/core/ARM-Cortex-M0 NO_DEFAULT_PATH)
#    message("append ${THIS_FILE}, ${SOURCE}")
    list (APPEND CORE_SOURCE_FILES ${THIS_FILE})
endforeach()

