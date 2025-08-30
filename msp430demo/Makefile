#自己改路径！！！
all: MKDIR-BUILD TO-OBJECTS TO-OUT TO-ELF TO-HEX TO-BIN
###########################################################
# Toolchain
###########################################################
CC = $(MSPGCC_BIN_DIR)/msp430-elf-gcc
GDB = $(MSPGCC_BIN_DIR)/msp430-elf-gdb
SIZE = $(MSPGCC_BIN_DIR)/msp430-elf-size

###########################################################
# Directories
###########################################################
MSPGCC_ROOT_DIR = /data/home/houzhu/work/msp430-gcc-9.3.1.11/msp430-gcc-9.3.1.11_linux64
MSPGCC_BIN_DIR = $(MSPGCC_ROOT_DIR)/bin

SUPPORT_FILE_DIRECTORY = /data/home/houzhu/work/CODE/430mspdemo/Driver
BUILD_DIR = Build
SOURCES = $(wildcard ./Src/*.c)
C_INCLUDES = -IInc\
-I/data/home/houzhu/work/msp430-gcc-9.3.1.11/msp430-gcc-9.3.1.11_linux64/msp430-elf/include

###########################################################
# Flags
###########################################################
OBJECTS=$(BUILD_DIR)/430mspdemo.o
CFLAGS = -I $(SUPPORT_FILE_DIRECTORY) -mmcu=$(DEVICE) -O2 -g
LFLAGS = -L $(SUPPORT_FILE_DIRECTORY) -T $(DEVICE).ld


###########################################################
# DEVICE
###########################################################
DEVICE = msp430g2553



###########################################################
# Command
###########################################################


TO-OBJECTS:
	@echo "compiling..."
	$(CC) -I $(SUPPORT_FILE_DIRECTORY) $(C_INCLUDES) -L $(SUPPORT_FILE_DIRECTORY)  -mmcu=$(DEVICE) -o2 -g $(SOURCES) -o $(BUILD_DIR)/430mspdemo.o
	$(SIZE) -t $(OBJECTS)

TO-OUT: ${OBJECTS}
	@echo "To outFile..."
	$(CC) -nostartfiles $(CFLAGS) $(LFLAGS) $? -o $(BUILD_DIR)/$(DEVICE).out
	$(SIZE)	-t $(BUILD_DIR)/$(DEVICE).out
TO-ELF: $(OBJECTS)
	@echo "To elfFile..."
	$(CC) -nostartfiles $(CFLAGS) $(LFLAGS) $? -o $(BUILD_DIR)/$(DEVICE).elf
	$(SIZE) -t $(BUILD_DIR)/$(DEVICE).elf

TO-HEX: $(OBJECTS)
	@echo "To hexFile..."
	$(CC) -nostartfiles $(CFLAGS) $(LFLAGS) $? -o $(BUILD_DIR)/$(DEVICE).hex
	$(SIZE) -t $(BUILD_DIR)/$(DEVICE).hex

TO-BIN: $(OBJECTS)
	@echo "To binFile..."
	$(CC) -nostartfiles $(CFLAGS) $(LFLAGS) $? -o $(BUILD_DIR)/$(DEVICE).bin
	$(SIZE) -t $(BUILD_DIR)/$(DEVICE).bin

MKDIR-BUILD:
	mkdir Build
Debug: 
	$(GDB) $(BUILD_DIR)/$(DEVICE).elf
clean:
	-rm -fR $(BUILD_DIR)