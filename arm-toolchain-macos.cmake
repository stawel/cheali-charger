# 1. brew install --cask gcc-arm-embedded
# 2. make sure the underlying filesystem used for building is case-sensitive
# 3. mkdir build
#    cd build
#    cmake -DCMAKE_TOOLCHAIN_FILE=../arm-toolchain.cmake -G Ninja ..
#    ninja
# 4. openocd -f interface/stlink.cfg -f target/numicro.cfg -c "init; halt"
# 5. telnet localhost 4444
#    program firmware.bin verify; exit

set(TOOLCHAIN arm-none-eabi)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

SET(CMAKE_C_COMPILER ${TOOLCHAIN}-gcc)
SET(CMAKE_CXX_COMPILER ${TOOLCHAIN}-g++)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

SET(ARM-Cortex-M0 ON)
