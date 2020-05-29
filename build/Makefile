
TOOLCHAIN = aarch64-none-elf
#TOOLCHAIN = arm-none-eabi
CC = $(TOOLCHAIN)-gcc
AS = $(TOOLCHAIN)-as
LD = $(TOOLCHAIN)-ld
OBJCOPY = $(TOOLCHAIN)-objcopy
GDB = $(TOOLCHAIN)-gdb


# Set any constants based on the raspberry pi model.  Version 1 has some differences to 2 and 3
CPU = cortex-a53
#CPU = cortex-a7

CFLAGS=  -fpic -ffreestanding  -g -std=gnu99
ASFLAGS = -mcpu=$(CPU) -fpic -ffreestanding
CSRCFLAGS= -O2 -Wall -Wextra
LFLAGS= -ffreestanding -O2 -nostdlib

# Location of the files
KER_SRC = ../src/kernel
KER_HEAD = ../include
COMMON_SRC = ../src/common
OBJ_DIR = objects
KERSOURCES = $(wildcard $(KER_SRC)/*.c)
KERSOURCES += $(wildcard $(KER_SRC)/$(ARCHDIR)/*.c)
COMMONSOURCES = $(wildcard $(COMMON_SRC)/*.c)
ASMSOURCES = $(wildcard $(KER_SRC)/*.S)
OBJECTS = $(patsubst $(KER_SRC)/%.c, $(OBJ_DIR)/%.o, $(KERSOURCES))
OBJECTS += $(patsubst $(COMMON_SRC)/%.c, $(OBJ_DIR)/%.o, $(COMMONSOURCES))
OBJECTS += $(patsubst $(KER_SRC)/%.S, $(OBJ_DIR)/%.o, $(ASMSOURCES))
HEADERS = $(wildcard $(KER_HEAD)/*.h)

IMG_NAME=kernel

build: $(OBJECTS) $(HEADERS)
	$(CC) -T linker.ld -o $(IMG_NAME).elf $(LFLAGS)  $(OBJECTS) -lgcc
	$(OBJCOPY) $(IMG_NAME).elf -O binary $(IMG_NAME).img
	qemu-system-aarch64 -M raspi3 -serial stdio -kernel kernel.elf
	
#qemu-system-arm -m 256 -M raspi2 -serial stdio -kernel kernel.elf

$(OBJ_DIR)/%.o: $(KER_SRC)/%.c
	
	$(CC) $(CFLAGS) -I$(KER_SRC) -I$(KER_HEAD) -c $< -o $@ $(CSRCFLAGS)

$(OBJ_DIR)/%.o: $(KER_SRC)/$(ARCHDIR)/%.c
	
	$(CC) $(CFLAGS) -I$(KER_SRC) -I$(KER_HEAD) -c $< -o $@ $(CSRCFLAGS)

$(OBJ_DIR)/%.o: $(KER_SRC)/%.S
	
	$(CC) $(ASFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(COMMON_SRC)/%.c
	
	$(CC) $(CFLAGS) -I$(KER_SRC) -I$(KER_HEAD) -c $< -o $@ $(CSRCFLAGS)

run: build

