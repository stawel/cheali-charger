set(SOURCE_FILES
        AnalogInputs.cpp
        AnalogInputs.h
        Balancer.cpp
        Balancer.h
        ChargingStrategy.cpp
        ChargingStrategy.h
        ChealiCharger2.cpp
        ChealiCharger2.h
        CMakeLists.txt
        DummyLiquidCrystal.h
        EditName.cpp
        EditName.h
        GTPowerA610.cpp
        GTPowerA610.h
        Hardware.h
        Keyboard.cpp
        Keyboard.h
        LcdPrint.cpp
        LcdPrint.h
        LiquidCrystal.cpp
        LiquidCrystal.h
        MainMenu.h
        memory.h
        Menu.h
        Multiplexer.cpp
        Multiplexer.h
        Options.cpp
        Options.h
        ProgramData.cpp
        ProgramData.h
        ProgramDataMenu.cpp
        ProgramDataMenu.h
        Screen.cpp
        Screen.h
        SimpleCharge.cpp
        SimpleCharge.h
        SMPS.cpp
        SMPS.h
        Program.cpp
        Program.h
        TheveninCharge.cpp
        TheveninCharge.h
        Thevenin.cpp
        Thevenin.h
        Timer.cpp
        Timer.h
        TimerOne.cpp
        TimerOne.h
        Utils.cpp
        Utils.h
)

include(${CMAKE_SOURCE_DIR}/arduino/arduino.cmake)

include_directories(${CMAKE_SOURCE_DIR}/arduino/cores/arduino ${CMAKE_SOURCE_DIR}/arduino/variants/atmega32)

link_libraries(arduino)
link_libraries(m)

add_executable(cheali-charger ${SOURCE_FILES})


add_custom_command(
    TARGET cheali-charger
    POST_BUILD
    COMMAND avr-objcopy -R .eeprom -O ihex $<TARGET_FILE:cheali-charger> $<TARGET_FILE:cheali-charger>.hex)

add_custom_command(
    TARGET cheali-charger
    POST_BUILD
    COMMAND avr-objcopy -R .eeprom -O binary $<TARGET_FILE:cheali-charger> $<TARGET_FILE:cheali-charger>.bin)

add_custom_command(
    TARGET cheali-charger
    POST_BUILD
    COMMAND avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex $<TARGET_FILE:cheali-charger> $<TARGET_FILE:cheali-charger>.eep)


add_custom_target(cheali-charger.size ALL
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/avr-size.sh $<TARGET_FILE:cheali-charger> $<TARGET_FILE:cheali-charger>.size 
    DEPENDS cheali-charger)
