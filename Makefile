TOOLCHAIN_PATH := ./i686-elf-tools/bin
CC := $(TOOLCHAIN_PATH)/i686-elf-gcc
LD := $(TOOLCHAIN_PATH)/i686-elf-gcc
AS := $(TOOLCHAIN_PATH)/i686-elf-as
# Directories
SRC_DIR := src
INCLUDE_DIR := includes
BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/obj
ISO_DIR := $(BUILD_DIR)/boot
GRUB_DIR := $(ISO_DIR)/grub
# Flags
CFLAGS := -std=gnu99 -ffreestanding -O2 -Wall -Wextra \
					-I$(INCLUDE_DIR)
LDFLAGS := -ffreestanding -O2 -nostdlib -lgcc
# Output
KERNEL_BIN := $(BUILD_DIR)/patience_os.bin
ISO_FILE := $(BUILD_DIR)/patience_os.iso
# Sources
C_SOURCES := $(shell find $(SRC_DIR) -name "*.c")
ASM_SOURCES := $(shell find $(SRC_DIR) -name "*.s")
# Objects
C_OBJECTS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(C_SOURCES))
ASM_OBJECTS := $(patsubst $(SRC_DIR)/%.s, $(OBJ_DIR)/%.o, $(ASM_SOURCES))
OBJECTS := $(C_OBJECTS) $(ASM_OBJECTS)
OBJ_DIRS := $(sort $(dir $(OBJECTS)))
# Default target
all: $(ISO_FILE)
# Create necessary directiories
$(OBJ_DIRS):
	@mkdir -p $@
# Compile C sources
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIRS)
	@echo "Compiling C source $<"
	@$(CC) $(CFLAGS) -c $< -o $@
# Assemble assembly sources
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.s | $(OBJ_DIRS)
	@echo "Assembling ASM source $<"
	@$(AS) $< -o $@
# Link the kernel
$(KERNEL_BIN): $(OBJECTS)
	@echo "Linking the kernel"
	@mkdir -p $(BUILD_DIR)
	@$(LD) -T linker.ld -o $@ $(LDFLAGS) $(OBJECTS)
	@echo "Checking multiboot compliance..."
	@if grub2-file --is-x86-multiboot $@; then \
		echo "Multiboot compliance pass."; \
	else \
		echo "Error: File is not multiboot compliant."; \
		exit 1; \
	fi
# Create ISO
$(ISO_FILE): $(KERNEL_BIN)
	@echo "Creating ISO Image..."
	@mkdir -p $(GRUB_DIR)
	@cp $(KERNEL_BIN) $(ISO_DIR)/patience_os.bin
	@cp grub.cfg $(GRUB_DIR)/grub.cfg
	@grub2-mkrescue -o $@ $(BUILD_DIR)
	@echo "Build successful: $(ISO_FILE)"
# Run in QEMU
run: $(ISO_FILE)
	qemu-system-i386 -cdrom $(ISO_FILE)
# Run in QEMU with debug options
debug: $(ISO_FILE)
	qemu-system-i386 -cdrom $(ISO_FILE) -serial stdio -display curses
# Clean up
clean:
	@echo "Cleaning build files..."
	@rm -rf $(BUILD_DIR)
# Generate compile_commands.json
compile_commands:
	@echo "Generating compile_commands.json..."
	@rm -f compile_commands.json
	@echo "[" > compile_commands.json
	@for source in $(C_SOURCES); do \
		echo "  {" >> compile_commands.json; \
		echo "    \"directory\": \"$$(pwd)\"," >> compile_commands.json; \
		echo "    \"command\": \"$(CC) $(CFLAGS) -c $$source -o $${source/.c/.o}\"," >> compile_commands.json; \
		echo "    \"file\": \"$$source\"" >> compile_commands.json; \
		if [ "$$source" != "$$(echo $(C_SOURCES) | awk '{print $$NF}')" ]; then \
			echo "  }," >> compile_commands.json; \
		else \
			echo "  }" >> compile_commands.json; \
		fi; \
	done
	@echo "]" >> compile_commands.json
	@echo "compile_commands.json generated."
help:
	@echo "PatienceOS Makefile"
	@echo "Available targets:"
	@echo "  all              - Build the kernel and create ISO (default)"
	@echo "  run              - Build and run in QEMU"
	@echo "  debug            - Build and run in QEMU with debug output"
	@echo "  clean            - Remove all build files"
	@echo "  compile_commands - Generate compile_commands.json for LSP"
	@echo "  help             - Show this help message"
.PHONY: all clean run debug help compile_commands
