

set(CORE_DIR ${CMAKE_CURRENT_LIST_DIR})
set(CORE_DIR_BIN ${CMAKE_BINARY_DIR}/src/core/)

set(CORE_SOURCE
        AnalogInputs.cpp  AnalogInputsPrivate.h  ChealiCharger2.cpp  eeprom.cpp  Program.cpp      ProgramData.h       ProgramDCcycle.h  Settings.cpp  Utils.cpp
        AnalogInputs.h    AnalogInputsTypes.h    ChealiCharger2.h    eeprom.h    ProgramData.cpp  ProgramDCcycle.cpp  Program.h         Settings.h    Utils.h
        AnalogInputsTypes.cpp
)

include_directories(${CORE_DIR_BIN})

CHEALI_ADD("CORE_SOURCE_FILES" "${CORE_SOURCE}")

include(${CORE_DIR}/strategy/strategy.cmake)
include(${CORE_DIR}/strategy/pid/pid.cmake)
include(${CORE_DIR}/calibration/calibration.cmake)
include(${CORE_DIR}/menus/menus.cmake)
include(${CORE_DIR}/screens/screens.cmake)
include(${CORE_DIR}/drivers/drivers.cmake)
include(${CORE_DIR}/strings/strings.cmake)
include(${CORE_DIR}/helper/helper.cmake)


