

set(CURRENT_DIR ${CORE_DIR}/analog/strategy)

set(CORE_SOURCE
    Balancer.cpp       DeltaChargeStrategy.cpp   SimpleDischargeStrategy.cpp  StorageStrategy.cpp  TheveninChargeStrategy.cpp     TheveninDischargeStrategy.h
    Balancer.h         DeltaChargeStrategy.h     SimpleDischargeStrategy.h    StorageStrategy.h    TheveninChargeStrategy.h       Thevenin.h
    DelayStrategy.cpp  SimpleChargeStrategy.cpp  StartInfoStrategy.cpp        Strategy.cpp         Thevenin.cpp                   TheveninMethod.cpp
    DelayStrategy.h    SimpleChargeStrategy.h    StartInfoStrategy.h          Strategy.h           TheveninDischargeStrategy.cpp  TheveninMethod.h
)

include_directories(${CURRENT_DIR})

CHEALI_FIND("CORE_SOURCE_FILES" "${CORE_SOURCE}" "${CURRENT_DIR}" )



