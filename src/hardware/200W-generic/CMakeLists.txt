

include(${CMAKE_SOURCE_DIR}/src/core/CMakeLists.txt)

set(THIS_FILE_PATH ${CMAKE_SOURCE_DIR}/src/hardware/200W-generic/)

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
    find_file(THIS_FILE ${SOURCE} PATHS ${THIS_FILE_PATH} NO_DEFAULT_PATH)
#    message("append ${THIS_FILE}, ${SOURCE}")
    list (APPEND GTPOWERA6_SOURCE_FILES ${THIS_FILE})
endforeach()

include_directories(${THIS_FILE_PATH})

SET(ALL_SOURCE_FILES
    ${SOURCE_FILES}
    ${GTPOWERA6_SOURCE_FILES}
    ${CORE_SOURCE_FILES}
)

include(${CMAKE_SOURCE_DIR}/src/core/CMakeListsPost.txt)

