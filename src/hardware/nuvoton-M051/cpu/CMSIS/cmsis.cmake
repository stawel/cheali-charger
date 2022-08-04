
set(CMSIS_ROOT ${CMAKE_CURRENT_LIST_DIR})
set(CMSIS_DIRS
    ${CMSIS_ROOT}/CMSIS/Include
    ${CMSIS_ROOT}/Device/Nuvoton/M051Series/Include
    ${CMSIS_ROOT}/Device/Nuvoton/M051Series/Source/GCC
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
    FMC.c
    sys.c
    _syscalls.c
    system_M051Series.c
    startup_M051Series.S
)

set(CMSIS_LINKER_SCRIPT gcc_arm.ld)

CHEALI_FIND(CPU_SOURCE_FILES "${CMSIS_SOURCE}" "${CMSIS_DIRS}")
CHEALI_FIND(CPU_LINKER_SCRIPT "${CMSIS_LINKER_SCRIPT}" "${CMSIS_DIRS}")
