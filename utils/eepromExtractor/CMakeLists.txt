cmake_minimum_required(VERSION 2.6)


Project(eepromExtractor)

set(SOURCE_FILES 
    main.cpp
    8.1.5/extractor.cpp)

#set_property(SOURCE 8.1.5/extractor.cpp PROPERTY COMPILE_FLAGS "-fpack-struct")
add_executable(eepromExtractor ${SOURCE_FILES})




