# Basic vars
OUT_DIR = out
SRC = src
BUILD = build
BOCHS = bochs
TESTS = tests
TEST = $(TESTS)/debug.rc
BOCHSRC = $(TESTS)/bochsrc
TARGET = i686-elf
ISO = gftos
DIST = $(ISO).tgz

# Define source files
ASM := $(sort $(shell find $(SRC) -name '*.s'))
C := $(sort $(shell find $(SRC) -name '*.c'))

# Includes
INCLUDE_DIR = $(SRC)/include
INCLUDES := -I$(INCLUDE_DIR)

# Compiler / assembler
ASSEMBLER = nasm -felf32
CC = $(TARGET)-gcc

CFLAGS = $(INCLUDES) -std=gnu99 -ffreestanding -O2 -Wall -Wextra
TEST_FLAGS = -nographic

# Linker info
LINK_FILE = linker.ld
LDFLAGS = -O2 -nostdlib -lgcc -ffreestanding

# Objects
OBJS =
C_OBJS := $(C:$(SRC)/%.c=$(BUILD)/%.o)
ASM_OBJS := $(ASM:$(SRC)/%.s=$(BUILD)/%.o)

# CRTI/CRTN objects
CRTBEGIN_OBJ := $(shell $(CC) $(CFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ := $(shell $(CC) $(CFLAGS) -print-file-name=crtend.o)
CRTI_OBJ = $(BUILD)/boot/crti.o
CRTN_OBJ = $(BUILD)/boot/crtn.o

# Add objects in order
OBJS += $(CRTI_OBJ) $(CRTBEGIN_OBJ)
OBJS += $(C_OBJS) $(ASM_OBJS)
OBJS += $(CRTEND_OBJ) $(CRTN_OBJ)

# Objects we make & dependencies
INTERNAL_OBJS = $(CRTI_OBJ) $(ASM_OBJS) $(C_OBJS) $(CRTN_OBJ)
DEPS := $(OBJS:.o=.d)

# Compile DB
COMPDBS := $(C_OBJS:.o=.json)

.PHONY: all
# Build gftos
all: $(OUT_DIR)/$(ISO).iso

.PHONY: run
# Run gftos in qemu
run: all
	@qemu-system-i386 -cdrom $(OUT_DIR)/$(ISO).iso
	@echo "QEMU \t$(OUT_DIR)/$(ISO).iso"

.PHONY: test
# Generate logfiles using bochs
test: all
	@mkdir -p $(BOCHS)
	@bochs -f $(BOCHSRC) -q -rc $(TEST) > /dev/null 2>&1
	@echo "BOCHS \t$(TEST)"

.PHONY: clean
# Clean the build environment
clean:
	@rm -rf $(BOCHS)
	@echo "RM \t$(BOCHS)"
	@rm -rf $(INTERNAL_OBJS)
	@for F in $(INTERNAL_OBJS);	\
	do echo "RM \t$$F"; done
	@rm -rf $(OUT_DIR)/$(ISO).iso
	@echo "RM \t$(OUT_DIR)/$(ISO).iso"
	@rm -rf $(OUT_DIR)/isodir/boot/$(ISO).bin
	@echo "RM \t$(OUT_DIR)/isodir/boot/$(ISO).bin"
	@rm -rf $(BUILD)
	@echo "RM \t$(BUILD)"
	@rm -rf $(DIST)
	@echo "RM \t$(DIST)"

.PHONY: compdb
# Generate JSON compilation database
compdb: $(BUILD)/compile_commands.json

.PHONY: dist
# Pack the source into a tar file
dist: clean
	@tar zcvf $(DIST) * >/dev/null
	@echo "TAR \t$(DIST)"

# https://stackoverflow.com/a/35730928
.PHONY: help
# Show this help.
help:
	@awk '/^#/{c=substr($$0,3);next}c&&/^[[:alpha:]][[:alnum:]_-]+:/{print substr($$1,1,index($$1,":")),c}1{c=0}' $(MAKEFILE_LIST) | column -s: -t

# ISO recipe (requires .bin)
$(OUT_DIR)/$(ISO).iso : $(OUT_DIR)/isodir/boot/$(ISO).bin
	@grub-mkrescue -o $@ $(OUT_DIR)/isodir 2>/dev/null
	@echo "GRUB \t$<"

# .bin Recipe (requires objects)
$(OUT_DIR)/isodir/boot/$(ISO).bin : $(OBJS)
	@$(CC) -T $(SRC)/$(LINK_FILE) -o $@ $(LDFLAGS) $^
	@echo "LD \t$(SRC)/$(LINK_FILE)"

# ASM object recipes
$(BUILD)/%.o: $(SRC)/%.s
	@mkdir -p $(BUILD)
	@mkdir -p $(@D)
	@$(ASSEMBLER) $^ -o $@
	@echo "AS \t$<"

# C object recipes
$(BUILD)/%.o: $(SRC)/%.c
	@mkdir -p $(BUILD)
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c $^ -o $@
	@echo "CC \t$<"

# COMPDB recipe (looks at all compdbs)
$(BUILD)/compile_commands.json: $(COMPDBS)
	@mkdir -p $(@D)
	@printf "[\n" > $@
	@sed -e '$$s/$$/,/' -s $(COMPDBS) | sed -e '$$s/,$$//' -e 's/^/    /' >> $@
	@printf "]\n" >> $@
	@for F in $(COMPDBS); \
	do echo "COMPDB \t$$F"; done;

# JSON recipe (looks at C files)
$(BUILD)/%.json: $(SRC)/%.c
	@mkdir -p $(@D)
	@printf "\
	{\n\
	   \"directory\": \"$(CURDIR)\",\n\
	   \"command\": \"$(CC) $(CFLAGS)  -c $< -o $(basename $@).o\",\n\
	   \"file\": \"$<\"\n\
	}\n" > $@
