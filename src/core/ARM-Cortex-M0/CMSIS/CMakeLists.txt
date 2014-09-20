
set(CMSIS_ROOT ${CMAKE_SOURCE_DIR}/src/core/ARM-Cortex-M0/CMSIS)
set(CMSIS_DIRS
    ${CMSIS_ROOT}/CMSIS/Include
    ${CMSIS_ROOT}/Device/Nuvoton/M051Series/Include
    ${CMSIS_ROOT}/Device/Nuvoton/M051Series/Source
    ${CMSIS_ROOT}/StdDriver/inc
    ${CMSIS_ROOT}/StdDriver/src
)

include_directories(${CMSIS_DIRS})

set(CMSIS_SOURCE
    clk.c
    adc.c
    timer.c
    gpio.c
    uart.c
    pwm.c
    fmc.c
    sys.c
    system_M051Series.c
)

foreach (SOURCE ${CMSIS_SOURCE})
    set(THIS_FILE THIS_FILE-NOTFOUND)
    find_file(THIS_FILE ${SOURCE} PATHS ${CMSIS_DIRS} NO_DEFAULT_PATH)
#    message("append ${THIS_FILE}, ${SOURCE}")
    list (APPEND CORE_SOURCE_FILES ${THIS_FILE})
endforeach()

