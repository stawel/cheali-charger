

set(CORE_SOURCE
    Balancer.cpp             DeltaChargeStrategy.h  SimpleChargeStrategy.cpp     SMPS.h                 Strategy.cpp                TheveninDischargeStrategy.cpp
    Balancer.h               Discharger.cpp         SimpleChargeStrategy.h       StartInfoStrategy.cpp  Strategy.h                  TheveninDischargeStrategy.h
    DelayStrategy.cpp        Discharger.h           SimpleDischargeStrategy.cpp  StartInfoStrategy.h    TheveninChargeStrategy.cpp  Thevenin.h
    DelayStrategy.h          Monitor.cpp            SimpleDischargeStrategy.h    StorageStrategy.cpp    TheveninChargeStrategy.h    TheveninMethod.cpp
    DeltaChargeStrategy.cpp  Monitor.h              SMPS.cpp                     StorageStrategy.h      Thevenin.cpp                TheveninMethod.h
)

CHEALI_ADD("CORE_SOURCE_FILES" "${CORE_SOURCE}")



