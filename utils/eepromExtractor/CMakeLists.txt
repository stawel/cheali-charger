cmake_minimum_required(VERSION 2.6)


Project(eepromExtractor)

set(SOURCE_FILES 
    main.cpp
    8.1.5/extractor.cpp)

#set_property(SOURCE 8.1.5/extractor.cpp PROPERTY COMPILE_FLAGS "-fpack-struct")
set(CMAKE_CXX_FLAGS "-std=c++0x -g -O0 ${CMAKE_CXX_FLAGS}")

add_executable(eepromExtractor ${SOURCE_FILES})




