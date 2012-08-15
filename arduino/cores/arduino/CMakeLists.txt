include(${CMAKE_SOURCE_DIR}/arduino/arduino.cmake)


set(HEADERS
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



set(SOURCE_SMALL
        main.cpp
        new.cpp
        WString.cpp
        Print.cpp
        wiring_analog.c
        wiring.c
        wiring_digital.c
)



add_library (arduino_small
        ${HEADERS}
        ${SOURCE_SMALL}
)

add_library (arduino
        ${HEADERS}
        ${SOURCE_SMALL}
        CDC.cpp
        HardwareSerial.cpp
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
