
FLASH_PGM = NU_M051x
LDS := ./target.ld

# Output dir
BIN_DIR := bin
OBJ_DIR := obj
TCL_DIR := ../tcl

COLON   := "

##Linux
#
CROSS_PATH    := $(COLON)/usr/bin
CROSS_COMPILE := $(CROSS_PATH)/arm-none-eabi-
##

##Win
#
#CROSS_PATH    := $(COLON)V:\CooCox\GNU Tools ARM Embedded\4.8 2014q2\bin
#CROSS_COMPILE := $(CROSS_PATH)\arm-none-eabi-
#
##

CC := $(CROSS_COMPILE)gcc$(COLON)
LD := $(CROSS_COMPILE)ld$(COLON)
OBJCOPY := $(CROSS_COMPILE)objcopy$(COLON)
OBJDUMP := $(CROSS_COMPILE)objdump$(COLON)
READELF := $(CROSS_COMPILE)readelf$(COLON)

RM    := rm -rf   
MKDIR := mkdir
COPY  := cp 

OPTFLAG = -Os

CFLAGS = -mcpu=cortex-m0 -mthumb
CFLAGS += $(OPTFLAG) -Wall
CFLAGS += -fPIC -fPIE -msingle-pic-base -mpic-register=9

LDFLAGS = -mcpu=cortex-m0 -mthumb
LDFLAGS += $(OPTFLAG) -nostartfiles
#LDFLAGS += -Wl,-Map=$(FLASH_PGM).map,--cref

SRCS := FlashPgm.c

C_OBJECTS := $(patsubst %.c,%.o,$(SRCS)) 

TARGET_ELF :=  $(BIN_DIR)/$(FLASH_PGM).elf
C_OBJECTS_EXT := $(addprefix $(OBJ_DIR)/, $(C_OBJECTS))
C_DEPS_EXT := $(patsubst %.o, %.d, $(C_OBJECTS_EXT))


all:  $(OBJ_DIR) $(BIN_DIR)  $(TARGET_ELF)
$(BIN_DIR):
	@echo
	mkdir $@
$(OBJ_DIR):
	mkdir $@

$(TARGET_ELF) : $(C_OBJECTS_EXT)
	$(CC) $(LDFLAGS) -T$(LDS) -o $@ $^
	$(OBJDUMP) -S $@ >  $(@:.elf=.src)
#	$(OBJCOPY) -O binary $@ > $(@:.elf=$(TCL_DIR)/.bin)
	$(OBJCOPY) -O binary $@ $(@:.elf=.bin)
	$(COPY) $(@:.elf=.bin) $(TCL_DIR)
#	$(OBJDUMP) -D $@ >  $(@:.elf=.dis)
#	$(READELF) -a $@ >  $(@:.elf=.info)

$(OBJ_DIR)/%.o :%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-$(RM) $(OBJ_DIR)/*.o $(BIN_DIR)/*.bin $(BIN_DIR)/*.elf $(BIN_DIR)/*.src $(TCL_DIR)/*.bin
#	-$(RM) $(BIN_DIR)/*.map $(BIN_DIR)/*.dis $(BIN_DIR)/*.info
	
