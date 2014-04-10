

include(${CMAKE_SOURCE_DIR}/src/core/atmega32/CMakeLists.txt)

<<<<<<< HEAD:src/hardware/200W-generic/CMakeLists.txt
set(CURRENT_PATH ${CMAKE_SOURCE_DIR}/src/hardware/200W-generic/)
=======
set(CURRENT_PATH ${CMAKE_SOURCE_DIR}/src/hardware/GTPowerA6-10-generic/)
>>>>>>> d478938aba1843b84172d5e9c789a40f899c8fc7:src/hardware/GTPowerA6-10-generic/CMakeLists.txt

set(GTPOWERA6_SOURCE
    GTPowerA6-10.cpp
    GTPowerA6-10.h
    TimerOne.cpp
    TimerOne.h
    adc.cpp
    adc.h
)


set (GTPOWERA6_SOURCE_FILES "")
foreach (SOURCE ${GTPOWERA6_SOURCE})
    set(THIS_FILE THIS_FILE-NOTFOUND)
    find_file(THIS_FILE ${SOURCE} PATHS ${CURRENT_PATH} NO_DEFAULT_PATH)
#    message("append ${THIS_FILE}, ${SOURCE}")
    list (APPEND GTPOWERA6_SOURCE_FILES ${THIS_FILE})
endforeach()

include_directories(${CURRENT_PATH})

SET(ALL_SOURCE_FILES
    ${SOURCE_FILES}
    ${GTPOWERA6_SOURCE_FILES}
    ${CORE_SOURCE_FILES}
)

include(${CMAKE_SOURCE_DIR}/src/core/atmega32/CMakeListsPost.txt)

