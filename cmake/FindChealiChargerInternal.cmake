
MESSAGE(STATUS "setting up cheali-charger MACROS.")

MACRO(CHEALI_ADD DEST FILES)
    include_directories(${CMAKE_CURRENT_LIST_DIR})
    CHEALI_FIND("${DEST}" "${FILES}" "${CMAKE_CURRENT_LIST_DIR}")
#    message("CMAKE_CURRENT_LIST_DIR ${CMAKE_CURRENT_LIST_DIR}")
ENDMACRO(CHEALI_ADD)


MACRO(CHEALI_FIND DEST FILES PATHS)
    foreach (SOURCE ${FILES})
        if (";${SOURCE_FILES};" MATCHES ";${SOURCE};")
            message("file: ${SOURCE} redefined in target")
        else()
            find_file(THIS_FILE ${SOURCE} PATHS ${PATHS} NO_DEFAULT_PATH)
#           message("append ${THIS_FILE}, ${SOURCE}")
            list (APPEND ${DEST} ${THIS_FILE})
            unset(THIS_FILE CACHE)
        endif()
    endforeach()
ENDMACRO(CHEALI_FIND)



MACRO(CHEALI_HARDWARE hard)
    set(hardware ${hard})
    message(STATUS "target: ${hardware}" )
ENDMACRO(CHEALI_HARDWARE)


MACRO(CHEALI_CPU CPU)
    SET(TARGET_CPU ${CPU})
    message(STATUS "cpu: ${TARGET_CPU}" )
    include(${CMAKE_SOURCE_DIR}/src/hardware/${TARGET_CPU}/cpu/cpu.cmake)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR})
ENDMACRO(CHEALI_CPU)

MACRO(CHEALI_GENERIC_CHARGER CHARGER)
    SET(TARGET_GENERIC_CHARGER ${CHARGER})
    message(STATUS "generic charger: ${TARGET_GENERIC_CHARGER}" )
    include(${CMAKE_SOURCE_DIR}/src/hardware/${TARGET_CPU}/generic/${TARGET_GENERIC_CHARGER}/generic.cmake)

    if(enable-short-names)
        #    hexfile short names for windows
        set(name "${hardware}")
        set(execName "${name}_${TARGET_CPU}")
    else(enable-short-names)
        set(name "${CMAKE_PROJECT_NAME}-${hardware}")
        set(execName "${name}_${cheali-charger-version}-${cheali-charger-eeprom-version-string}-${cheali-charger-buildnumber}_${TARGET_CPU}")
    endif(enable-short-names)
    set(sizeName "${name}")

    include(${CMAKE_SOURCE_DIR}/src/core/core.cmake)

    SET(ALL_SOURCE_FILES
        ${SOURCE_FILES}
        ${GENERIC_SOURCE_FILES}
        ${CPU_SOURCE_FILES}
        ${CORE_SOURCE_FILES}
    )

ENDMACRO(CHEALI_GENERIC_CHARGER)



MACRO(CHEALI_GENERATE_ARM_EXEC)
    add_executable(${execName} ${ALL_SOURCE_FILES})

    add_custom_command(
        TARGET ${execName}
        POST_BUILD
        COMMAND ${TOOLCHAIN}-objcopy -O ihex $<TARGET_FILE:${execName}> $<TARGET_FILE:${execName}>.hex)

    add_custom_command(
        TARGET ${execName}
        POST_BUILD
        COMMAND ${TOOLCHAIN}-objcopy -O binary $<TARGET_FILE:${execName}> $<TARGET_FILE:${execName}>.bin)

    add_custom_target(${execName}.size ALL
        COMMAND ${TOOLCHAIN}-size $<TARGET_FILE:${execName}>
        DEPENDS ${execName})

ENDMACRO(CHEALI_GENERATE_ARM_EXEC)



MACRO(CHEALI_GENERATE_AVR_EXEC)
    add_executable(${execName} ${ALL_SOURCE_FILES})

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

    if(BASH)
        add_custom_target(${execName}.size ALL
            COMMAND ${CMAKE_SOURCE_DIR}/src/core/avr-size.sh $<TARGET_FILE:${execName}> ${sizeName}.size
            DEPENDS ${execName})
    endif(BASH)

ENDMACRO(CHEALI_GENERATE_AVR_EXEC)
