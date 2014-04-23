
#set(TOOLCHAIN arm-elf)
set(TOOLCHAIN arm-none-eabi)

include_directories(/usr/${TOOLCHAIN}/include)

SET(CMAKE_C_COMPILER ${TOOLCHAIN}-gcc)
SET(CMAKE_CXX_COMPILER ${TOOLCHAIN}-g++)

#add_definitions(-DUSE_STDPERIPH_DRIVER)
#add_definitions(-DSTM32F4XX)
#add_definitions(-DHSE_VALUE=8000000)

SET(CFLAGS "-mthumb -mcpu=cortex-m0  -Os  -Wall -g")
SET(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   ${CFLAGS} -ffunction-sections -fdata-sections")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CFLAGS} -ffunction-sections -fdata-sections")

set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
#set(CMAKE_EXE_LINKER_FLAGS "${CFLAGS} -nostartfiles -L${COMMON_DIR}/scripts -Tstm32f407vg.lds -Wl,--gc-sections")
set(CMAKE_EXE_LINKER_FLAGS "${CFLAGS} --specs=rdimon.specs  -Wl,--start-group -lgcc -lc -lm -lrdimon -Wl,--gc-sections -Wl,--end-group")
