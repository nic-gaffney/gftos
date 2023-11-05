OUT_DIR = out
SRC = src
BUILD = build
TARGET = i686-elf

ISO = myos

ASM := $(sort $(shell find $(SRC) -name '*.s'))
C := $(sort $(shell find $(SRC) -name '*.c'))

INCLUDE_DIR = $(SRC)/include
INCLUDES := -I$(INCLUDE_DIR)

ASSEMBLER = $(TARGET)-as
CC = $(TARGET)-gcc

CFLAGS = $(INCLUDES) -std=gnu99 -ffreestanding -O2 -Wall -Wextra
TEST_FLAGS = -nographic


LINK_FILE = linker.ld
LDFLAGS = -O2 -nostdlib -lgcc -ffreestanding

OBJS =
C_OBJS := $(C:$(SRC)/%.c=$(BUILD)/C/%.o)
ASM_OBJS := $(ASM:$(SRC)/%.s=$(BUILD)/ASM/%.o)

OBJS += $(C_OBJS)
OBJS += $(ASM_OBJS)
DEPS := $(OBJS:.o=.d)

.PHONY: all
all: $(OUT_DIR)/$(ISO).iso

run: all
	qemu-system-i386 -cdrom $(OUT_DIR)/$(ISO).iso


$(OUT_DIR)/$(ISO).iso : $(OUT_DIR)/isodir/boot/$(ISO).bin
	grub-mkrescue -o $@ $(OUT_DIR)/isodir

$(OUT_DIR)/isodir/boot/$(ISO).bin : $(OBJS)
	$(CC) -T $(SRC)/$(LINK_FILE) -o $@ $(LDFLAGS) $^

$(BUILD)/ASM/%.o: $(SRC)/%.s
	mkdir -p $(BUILD)
	mkdir -p $(BUILD)/ASM
	mkdir -p $(@D)
	$(ASSEMBLER) $^ -o $@

$(BUILD)/C/%.o: $(SRC)/%.c
	mkdir -p $(BUILD)
	mkdir -p $(BUILD)/C
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -rf $(BUILD)
	rm -rf $(OUT_DIR)/$(ISO).iso
	rm -rf $(OUT_DIR)/isodir/boot/$(ISO).bin
