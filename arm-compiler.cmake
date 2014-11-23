
#set(TOOLCHAIN arm-elf)
#please see for ubuntu: https://launchpad.net/~terry.guo/+archive/ubuntu/gcc-arm-embedded
set(TOOLCHAIN arm-none-eabi)
#set(TOOLCHAIN arm-linux-gnueabi)

include_directories(/usr/${TOOLCHAIN}/include)

SET(CMAKE_C_COMPILER ${TOOLCHAIN}-gcc)
SET(CMAKE_CXX_COMPILER ${TOOLCHAIN}-g++)


SET(CFLAGS "-mthumb -mcpu=cortex-m0  -Os  -Wall -g -std=c11")
SET(CXXFLAGS "-mthumb -mcpu=cortex-m0  -Os  -Wall -g -fno-rtti -fno-exceptions")
SET(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   ${CFLAGS}   -ffunction-sections -fdata-sections")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXXFLAGS} -ffunction-sections -fdata-sections")

#TODO: remove CoIDE dependency
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--gc-sections -Wl,-T${CMAKE_SOURCE_DIR}/CoIDE/arm-gcc-link.ld")
