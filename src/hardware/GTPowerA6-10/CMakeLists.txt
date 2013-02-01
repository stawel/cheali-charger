include(${CMAKE_SOURCE_DIR}/src/core/CMakeLists.txt)

set(hardware GTPowerA6-10)
set(execName "cheali-charger-${hardware}")


set(SOURCE_FILES
    GTPowerA6-10.cpp
    GTPowerA6-10.h
    Hardware.h
    ${CORE_SOURCE_FILES}
)

include(${CMAKE_SOURCE_DIR}/arduino/arduino.cmake)
include_directories(${CMAKE_SOURCE_DIR}/arduino/cores/arduino ${CMAKE_SOURCE_DIR}/arduino/variants/atmega32)
include_directories(${CMAKE_SOURCE_DIR}/src/core)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

include_directories(/usr/lib/avr/include/)
link_libraries(arduino)
link_libraries(m)

add_executable(${execName} ${SOURCE_FILES})

add_custom_command(
    TARGET ${execName}
    POST_BUILD
    COMMAND avr-objcopy -R .eeprom -O ihex $<TARGET_FILE:${execName}> $<TARGET_FILE:${execName}>.hex)

add_custom_command(
    TARGET ${execName}
    POST_BUILD
    COMMAND avr-objcopy -R .eeprom -O binary $<TARGET_FILE:${execName}> $<TARGET_FILE:${execName}>.bin)

add_custom_command(
    TARGET ${execName}
    POST_BUILD
    COMMAND avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex $<TARGET_FILE:${execName}> $<TARGET_FILE:${execName}>.eep)


add_custom_target(${execName}.size ALL
    COMMAND ${CMAKE_SOURCE_DIR}/src/core/avr-size.sh $<TARGET_FILE:${execName}> $<TARGET_FILE:${execName}>.size 
    DEPENDS ${execName})
