# Makefile for Operating System Kernel - Part 2: Interactive Terminal
# Builds OS kernel with interrupt-driven keyboard input and terminal interface

# Compiler and tools
NASM = nasm
LD = ld
GCC = gcc
GENISOIMAGE = genisoimage
QEMU = qemu-system-i386

# Directories
SOURCE_DIR = source
DRIVERS_DIR = drivers
ISO_DIR = iso
GRUB_DIR = $(ISO_DIR)/boot/grub

# Files
LOADER_ASM = $(SOURCE_DIR)/loader.asm
LOADER_OBJ = $(SOURCE_DIR)/loader.o
KERNEL_C = $(SOURCE_DIR)/kernel.c
KERNEL_OBJ = $(SOURCE_DIR)/kernel.o
FRAMEBUFFER_C = $(DRIVERS_DIR)/framebuffer.c
FRAMEBUFFER_OBJ = $(DRIVERS_DIR)/framebuffer.o
IO_ASM = $(DRIVERS_DIR)/io.asm
IO_OBJ = $(DRIVERS_DIR)/io.o
INTERRUPTS_C = $(DRIVERS_DIR)/interrupts.c
INTERRUPTS_OBJ = $(DRIVERS_DIR)/interrupts.o
KEYBOARD_C = $(DRIVERS_DIR)/keyboard.c
KEYBOARD_OBJ = $(DRIVERS_DIR)/keyboard.o
PIC_C = $(DRIVERS_DIR)/pic.c
PIC_OBJ = $(DRIVERS_DIR)/pic.o
INTERRUPT_ASM_S = $(DRIVERS_DIR)/interrupt_asm.s
INTERRUPT_ASM_OBJ = $(DRIVERS_DIR)/interrupt_asm.o
INTERRUPT_HANDLERS_S = $(DRIVERS_DIR)/interrupt_handlers.s
INTERRUPT_HANDLERS_OBJ = $(DRIVERS_DIR)/interrupt_handlers.o
HARDWARE_INT_S = $(DRIVERS_DIR)/hardware_interrupt_enabler.s
HARDWARE_INT_OBJ = $(DRIVERS_DIR)/hardware_interrupt_enabler.o
LINKER_SCRIPT = $(SOURCE_DIR)/link.ld
KERNEL_ELF = kernel.elf
ISO_FILE = os.iso
LOG_FILE = logQ.txt

# Compiler flags for freestanding environment
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c

# Default target - builds everything
all: $(ISO_FILE)

# Build the kernel object file from assembly
$(LOADER_OBJ): $(LOADER_ASM)
	$(NASM) -f elf $(LOADER_ASM) -o $(LOADER_OBJ)

# Build the C kernel object file
$(KERNEL_OBJ): $(KERNEL_C)
	$(GCC) $(CFLAGS) $(KERNEL_C) -o $(KERNEL_OBJ)

# Build the framebuffer driver object file
$(FRAMEBUFFER_OBJ): $(FRAMEBUFFER_C)
	$(GCC) $(CFLAGS) $(FRAMEBUFFER_C) -o $(FRAMEBUFFER_OBJ)

# Build the I/O assembly object file
$(IO_OBJ): $(IO_ASM)
	$(NASM) -f elf $(IO_ASM) -o $(IO_OBJ)

# Build the interrupts C object file
$(INTERRUPTS_OBJ): $(INTERRUPTS_C)
	$(GCC) $(CFLAGS) $(INTERRUPTS_C) -o $(INTERRUPTS_OBJ)

# Build the keyboard C object file
$(KEYBOARD_OBJ): $(KEYBOARD_C)
	$(GCC) $(CFLAGS) $(KEYBOARD_C) -o $(KEYBOARD_OBJ)

# Build the PIC C object file
$(PIC_OBJ): $(PIC_C)
	$(GCC) $(CFLAGS) $(PIC_C) -o $(PIC_OBJ)

# Build the interrupt assembly object file
$(INTERRUPT_ASM_OBJ): $(INTERRUPT_ASM_S)
	$(NASM) -f elf $(INTERRUPT_ASM_S) -o $(INTERRUPT_ASM_OBJ)

# Build the interrupt handlers assembly object file
$(INTERRUPT_HANDLERS_OBJ): $(INTERRUPT_HANDLERS_S)
	$(NASM) -f elf $(INTERRUPT_HANDLERS_S) -o $(INTERRUPT_HANDLERS_OBJ)

# Build the hardware interrupt enabler assembly object file
$(HARDWARE_INT_OBJ): $(HARDWARE_INT_S)
	$(NASM) -f elf $(HARDWARE_INT_S) -o $(HARDWARE_INT_OBJ)

# Link the kernel executable (now includes all components)
$(KERNEL_ELF): $(LOADER_OBJ) $(KERNEL_OBJ) $(FRAMEBUFFER_OBJ) $(IO_OBJ) $(INTERRUPTS_OBJ) $(KEYBOARD_OBJ) $(PIC_OBJ) $(INTERRUPT_ASM_OBJ) $(INTERRUPT_HANDLERS_OBJ) $(HARDWARE_INT_OBJ) $(LINKER_SCRIPT)
	$(LD) -T $(LINKER_SCRIPT) -melf_i386 $(LOADER_OBJ) $(KERNEL_OBJ) $(FRAMEBUFFER_OBJ) $(IO_OBJ) $(INTERRUPTS_OBJ) $(KEYBOARD_OBJ) $(PIC_OBJ) $(INTERRUPT_ASM_OBJ) $(INTERRUPT_HANDLERS_OBJ) $(HARDWARE_INT_OBJ) -o $(KERNEL_ELF)

# Copy kernel to ISO directory structure
$(ISO_DIR)/boot/$(KERNEL_ELF): $(KERNEL_ELF)
	cp $(KERNEL_ELF) $(ISO_DIR)/boot/

# Create the ISO image
$(ISO_FILE): $(ISO_DIR)/boot/$(KERNEL_ELF) $(GRUB_DIR)/menu.lst $(GRUB_DIR)/stage2_eltorito
	$(GENISOIMAGE) -R \
		-b boot/grub/stage2_eltorito \
		-no-emul-boot \
		-boot-load-size 4 \
		-A os \
		-input-charset utf8 \
		-quiet \
		-boot-info-table \
		-o $(ISO_FILE) \
		$(ISO_DIR)

# Run the OS in QEMU emulator
run: $(ISO_FILE)
	@echo "Starting QEMU emulator in curses mode..."
	@echo "The kernel will:"
	@echo "  1. Initialize framebuffer and clear screen"
	@echo "  2. Test framebuffer I/O with colors and positioning"
	@echo "  3. Test C functions: sum_of_three, factorial, fibonacci"
	@echo "  4. Display results on screen using framebuffer"
	@echo ""
	@echo "IMPORTANT: QEMU is running in curses mode."
	@echo "To quit, open another terminal and run:"
	@echo "  telnet localhost 45454"
	@echo "  (qemu) quit"
	@echo ""
	$(QEMU) -nographic -boot d -cdrom $(ISO_FILE) -m 32 -d cpu -D $(LOG_FILE)

# Run the OS in QEMU emulator with curses mode for framebuffer testing
run-curses: $(ISO_FILE)
	@echo "Starting QEMU emulator in curses mode with telnet monitor..."
	@echo "The kernel will test framebuffer I/O functionality."
	@echo ""
	@echo "To quit QEMU:"
	@echo "  1. Open another terminal"
	@echo "  2. Run: telnet localhost 45454"
	@echo "  3. Type: quit"
	@echo ""
	$(QEMU) -curses -monitor telnet::45454,server,nowait -serial mon:stdio -boot d -cdrom $(ISO_FILE) -m 32 -d cpu -D $(LOG_FILE)

# Check if 0xCAFEBABE appears in the log (run this after stopping QEMU)
check-log:
	@if [ -f $(LOG_FILE) ]; then \
		echo "Searching for 0xCAFEBABE (initial value) in log file..."; \
		grep -i "cafebabe" $(LOG_FILE) > /dev/null && echo "✅ Found 0xCAFEBABE in EAX!" || echo "❌ No 0xCAFEBABE found."; \
		echo "Searching for 0xDEADBEEF (C function success) in log file..."; \
		grep -i "deadbeef" $(LOG_FILE) > /dev/null && echo "✅ SUCCESS: C functions executed correctly!" || echo "❌ C functions may have failed or not executed."; \
		echo "Searching for 0xBADC0DE (C function failure) in log file..."; \
		grep -i "badc0de" $(LOG_FILE) > /dev/null && echo "❌ FAILURE: C function calculations were incorrect!" || echo "✅ No failure indicator found."; \
	else \
		echo "Log file $(LOG_FILE) not found. Run 'make run' first."; \
	fi

# Clean up generated files
clean:
	rm -f $(LOADER_OBJ) $(KERNEL_OBJ) $(FRAMEBUFFER_OBJ) $(IO_OBJ) $(PIC_OBJ) $(INTERRUPTS_OBJ) $(KEYBOARD_OBJ) $(INTERRUPT_ASM_OBJ) $(INTERRUPT_HANDLERS_OBJ) $(HARDWARE_INTERRUPT_ENABLER_OBJ) $(KERNEL_ELF) $(ISO_FILE) $(LOG_FILE)
	rm -f $(ISO_DIR)/boot/$(KERNEL_ELF)

# Show directory structure
show-structure:
	@echo "Directory structure:"
	find . -type d | sort | sed 's/^/  /'
	@echo ""
	@echo "Key files:"
	find . -name "*.asm" -o -name "*.ld" -o -name "*.lst" -o -name "stage2_eltorito" | sort | sed 's/^/  /'

# Help target
help:
	@echo "Available targets for Worksheet 2 Part 2:"
	@echo "  all           - Build the complete OS ISO image (default)"
	@echo "  run           - Build and run the OS in QEMU emulator"
	@echo "  run-curses    - Run the OS in QEMU curses mode for interactive testing"
	@echo "  check-log     - Check execution results in log file"
	@echo "  clean         - Remove all generated files"
	@echo "  show-structure - Display the directory structure"
	@echo "  help          - Show this help message"
	@echo ""
	@echo "Part 2 Features:"
	@echo "  - Interrupt-driven keyboard input (IRQ 1)"
	@echo "  - Interactive terminal interface"
	@echo "  - Command parsing and execution"
	@echo "  - Input buffering with circular buffer"
	@echo ""
	@echo "Usage:"
	@echo "  make run-curses - Run with interactive terminal"
	@echo "  Commands: help, version, echo [text], clear"
	@echo ""
	@echo "To quit QEMU: telnet localhost 45454 then type 'quit'"

.PHONY: all run run-curses clean show-structure help check-log
