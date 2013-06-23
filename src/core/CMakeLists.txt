set(CORE_SOURCE
        AnalogInputs.cpp
        AnalogInputs.h
        Balancer.cpp
        Balancer.h
        Strategy.cpp
        Strategy.h
        ChealiCharger2.cpp
        ChealiCharger2.h
        CMakeLists.txt
        EditName.cpp
        EditName.h
        Keyboard.cpp
        Keyboard.h
        LcdPrint.cpp
        LcdPrint.h
        LiquidCrystal.cpp
        LiquidCrystal.h
        MainMenu.h
        StaticMenu.h
        EditMenu.h
        EditMenu.cpp
        Menu.h
        Menu.cpp
        memory.h
        Multiplexer.cpp
        Multiplexer.h
        ProgramData.cpp
        ProgramData.h
        ProgramDataMenu.cpp
        ProgramDataMenu.h
        Screen.cpp
        Screen.h

        SimpleChargeStrategy.cpp
        SimpleChargeStrategy.h
        DeltaChargeStrategy.cpp
        DeltaChargeStrategy.h
        SimpleDischargeStrategy.cpp
        SimpleDischargeStrategy.h
        TheveninDischargeStrategy.cpp
        TheveninDischargeStrategy.h
        TheveninChargeStrategy.cpp
        TheveninChargeStrategy.h
        StorageStrategy.cpp
        StorageStrategy.h
        StartInfoStrategy.cpp
        StartInfoStrategy.h

        SMPS.cpp
        SMPS.h
        Discharger.cpp
        Discharger.h
        Program.cpp
        Program.h
        Thevenin.cpp
        Thevenin.h
        TheveninMethod.cpp
        TheveninMethod.h
        Timer.cpp
        Timer.h
        Utils.cpp
        Utils.h
        Blink.cpp
        Blink.h

        Monitor.cpp
        Monitor.h

        Calibrate.cpp
        Calibrate.h
        Options.cpp
        Options.h
        Settings.cpp
        Settings.h
        SettingsMenu.cpp
        SettingsMenu.h

        PolarityCheck.cpp
        PolarityCheck.h

        Buzzer.cpp
        Buzzer.h

        Version.h
        Version.cpp

        StackInfo.cpp
        StackInfo.h
)

set (CORE_SOURCE_FILES "")
foreach (SOURCE ${CORE_SOURCE})
    set(THIS_FILE THIS_FILE-NOTFOUND)
    find_file(THIS_FILE ${SOURCE} PATHS ${CMAKE_SOURCE_DIR}/src/core NO_DEFAULT_PATH)
#    message("append ${THIS_FILE}, ${SOURCE}")
    unset(SFILE)
    list (APPEND CORE_SOURCE_FILES ${THIS_FILE})
endforeach()

