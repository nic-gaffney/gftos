OUT_DIR = out
SRC = src
BUILD = build
TARGET = i686-elf

ISO = myos

ASM := $(sort $(shell find $(SRC) -name '*.s'))
C := $(sort $(shell find $(SRC) -name '*.c'))

INCLUDE_DIR = $(SRC)/include
INCLUDES := -I$(INCLUDE_DIR)

ASSEMBLER = nasm -felf32
CC = $(TARGET)-gcc

CFLAGS = $(INCLUDES) -std=gnu99 -ffreestanding -O2 -Wall -Wextra
TEST_FLAGS = -nographic


LINK_FILE = linker.ld
LDFLAGS = -O2 -nostdlib -lgcc -ffreestanding

OBJS =
C_OBJS := $(C:$(SRC)/%.c=$(BUILD)/%.o)
ASM_OBJS := $(ASM:$(SRC)/%.s=$(BUILD)/%.o)

CRTBEGIN_OBJ := $(shell $(CC) $(CFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ := $(shell $(CC) $(CFLAGS) -print-file-name=crtend.o)
CRTI_OBJ = $(BUILD)/boot/crti.o
CRTN_OBJ = $(BUILD)/boot/crtn.o

OBJS += $(CRTI_OBJ) $(CRTBEGIN_OBJ)
OBJS += $(C_OBJS) $(ASM_OBJS)
OBJS += $(CRTEND_OBJ) $(CRTN_OBJ)

INTERNAL_OBJS = $(CRTI_OBJ) $(ASM_OBJS) $(C_OBJS) $(CRTN_OBJ)
DEPS := $(OBJS:.o=.d)

.PHONY: all
all: $(OUT_DIR)/$(ISO).iso

run: all
	qemu-system-i386 -cdrom $(OUT_DIR)/$(ISO).iso


$(OUT_DIR)/$(ISO).iso : $(OUT_DIR)/isodir/boot/$(ISO).bin
	grub-mkrescue -o $@ $(OUT_DIR)/isodir

$(OUT_DIR)/isodir/boot/$(ISO).bin : $(OBJS)
	$(CC) -T $(SRC)/$(LINK_FILE) -o $@ $(LDFLAGS) $^

$(BUILD)/%.o: $(SRC)/%.s
	mkdir -p $(BUILD)
	mkdir -p $(@D)
	$(ASSEMBLER) $^ -o $@

$(BUILD)/%.o: $(SRC)/%.c
	mkdir -p $(BUILD)
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $^ -o $@


clean:
	rm -rf $(INTERNAL_OBJS)
	rm -rf $(OUT_DIR)/$(ISO).iso
	rm -rf $(OUT_DIR)/isodir/boot/$(ISO).bin
	rm -rf $(BUILD)
