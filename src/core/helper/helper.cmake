

set(CURRENT_DIR ${CORE_DIR}/helper)

set(CORE_SOURCE BalancePortAnalyzer.cpp
)

include_directories(${CURRENT_DIR})

CHEALI_FIND("CORE_SOURCE_FILES" "${CORE_SOURCE}" "${CURRENT_DIR}" )



