include(${CMAKE_SOURCE_DIR}/arduino/arduino.cmake)


set(HEADER_FILES
        Arduino.h
        binary.h
        Client.h
        HardwareSerial.h
        IPAddress.h
        new.h
        Platform.h
        Printable.h
        Print.h
        Server.h
        Stream.h
        Udp.h
        USBAPI.h
        USBCore.h
        USBDesc.h
        WCharacter.h
        wiring_private.h
        WString.h
)

set(SOURCE_FILES
        main.cpp
        new.cpp
        WString.cpp
        Print.cpp
        wiring_analog.c
        wiring.c
        wiring_digital.c


        CDC.cpp
#        HardwareSerial.cpp
        HID.cpp
        IPAddress.cpp
        Stream.cpp
        Tone.cpp
        USBCore.cpp
        WMath.cpp

        WInterrupts.c
        wiring_pulse.c
        wiring_shift.c
)

add_library (arduino
        ${HEADER_FILES}
        ${SOURCE_FILES}
)
