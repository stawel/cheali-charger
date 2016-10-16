
set(TOOLCHAIN arm-none-eabi)
#please see for ubuntu: https://launchpad.net/~terry.guo/+archive/ubuntu/gcc-arm-embedded
#set(TOOLCHAIN arm-elf)
#set(TOOLCHAIN arm-linux-gnueabi)


#cmake compiler test bypass
INCLUDE(CMakeForceCompiler)

SET(CMAKE_SYSTEM_NAME Generic)

SET(CMAKE_C_COMPILER ${TOOLCHAIN}-gcc)
SET(CMAKE_CXX_COMPILER ${TOOLCHAIN}-g++)


CMAKE_FORCE_C_COMPILER(${CMAKE_C_COMPILER} GNU)
CMAKE_FORCE_CXX_COMPILER(${CMAKE_CXX_COMPILER} GNU)

SET(ARM-Cortex-M0 ON)