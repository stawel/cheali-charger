

#SET(COPT "--std-sdcc11 --opt-code-size --stack-auto --model-large --parms-in-bank1")
#SET(COPT "--std-sdcc11 --opt-code-size --model-small --stack-auto")
SET(COPT "--std-sdcc11 --opt-code-size  --model-small --parms-in-bank1")
SET(COPT "--std-sdcc11 --opt-code-size --model-large --parms-in-bank1")
#SET(COPT "--std-sdcc11  --opt-code-size --model-small --parms-in-bank1") smallest progmem size (25594), not enought iram

SET(COPT "--std-sdcc11  --stack-auto --opt-code-size --model-small --parms-in-bank1") #progmem size 30114
SET(CMAKE_C_FLAGS "${COPT} -DSDCC_COMPILER --verbose -V")

#SET(LFLAGS  "--std-sdcc11 --opt-code-size --stack-auto  --use-stdout --model-large --xram-loc 0x1B --xram-size 768 --code-size 32768 -V")
SET(LFLAGS  "${OPT}  --use-stdout --xram-size 768 -V")
SET(CMAKE_EXE_LINKER_FLAGS ${LFLAGS})
SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS  ${LFLAGS})
SET(CMAKE_MODULE_LINKER_FLAGS ${LFLAGS})
SET(CMAKE_EXE_LINKER_FLAGS_INIT --model-large)


#TODO: sdcc ..
include_directories(/usr/local/share/sdcc/include/)