#TODO: remove CoIDE dependency
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-T${CMAKE_SOURCE_DIR}/CoIDE/arm-gcc-link.ld")

#c type menus
#add_subdirectory(targets/AnalogInputsAnalyzer-50W)
add_subdirectory(targets/imaxB6-clone)
add_subdirectory(targets/imaxB6-80W)
add_subdirectory(targets/imaxB6-50W-BOA6)
