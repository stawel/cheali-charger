
include(${CMAKE_SOURCE_DIR}/src/core/CMakeLists.txt)

include(${CMAKE_SOURCE_DIR}/src/core/ARM-Cortex-M0/CMSIS/CMakeLists.txt)


set(ARM_Cortex_M0_SOURCE
        cpu.cpp
        memory.cpp
        Serial.cpp
        Timer0.cpp
        TxHardSerial.cpp
        TxSoftSerial.cpp
        UtilsD.cpp
)

foreach (SOURCE ${ARM_Cortex_M0_SOURCE})
    set(THIS_FILE THIS_FILE-NOTFOUND)
    find_file(THIS_FILE ${SOURCE} PATHS ${CMAKE_SOURCE_DIR}/src/core/ARM-Cortex-M0 NO_DEFAULT_PATH)
#    message("append ${THIS_FILE}, ${SOURCE}")
    list (APPEND CORE_SOURCE_FILES ${THIS_FILE})
endforeach()

