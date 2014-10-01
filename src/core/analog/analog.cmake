

set(CURRENT_DIR ${CORE_DIR}/analog)

set(CORE_SOURCE
    AnalogInputs.cpp       AnalogInputsTypes.h  eeprom.cpp   Monitor.h          Program.cpp      ProgramDCcycle.cpp  Settings.cpp  SMPS.h
    AnalogInputs.h         Discharger.cpp       eeprom.h     PolarityCheck.cpp  ProgramData.cpp  ProgramDCcycle.h    Settings.h
    AnalogInputsPrivate.h  Discharger.h         Monitor.cpp  PolarityCheck.h    ProgramData.h    Program.h           SMPS.cpp
)

include_directories(${CURRENT_DIR})

CHEALI_FIND("CORE_SOURCE_FILES" "${CORE_SOURCE}" "${CURRENT_DIR}" )

include(${CURRENT_DIR}/strategy/strategy.cmake)


