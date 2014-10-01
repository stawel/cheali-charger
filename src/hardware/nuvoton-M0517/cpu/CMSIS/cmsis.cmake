
set(CMSIS_ROOT ${CURRENT_PATH}/CMSIS)
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

CHEALI_FIND(CPU_SOURCE_FILES "${CMSIS_SOURCE}" "${CMSIS_DIRS}")

