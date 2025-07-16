# Operating Systems Development

This repository contains operating system development projects demonstrating low-level programming concepts.

## Project Structure

### worksheet1/
Assembly programming tasks demonstrating:
- Basic arithmetic operations
- Loops and conditionals 
- Array manipulation
- Build management with Makefiles

### worksheet2_part1/
Basic framebuffer I/O operating system:
- Static display with colored text
- VGA framebuffer operations
- C function integration
- Mathematical computations

### worksheet2_part2/
Interactive terminal operating system:
- Real-time keyboard input
- Interrupt-driven system
- Command processing (`help`, `echo`, `clear`, `version`)
- Full terminal experience

## Quick Start

Each project has its own README.md with detailed instructions.

### Test Assembly Programs (Worksheet 1)
```bash
cd worksheet1
make
```

### Test Basic OS (Worksheet 2 Part 1)
```bash
cd worksheet2_part1
make clean && make all && make run-curses
```

### Test Interactive OS (Worksheet 2 Part 2)
```bash
cd worksheet2_part2
make clean && make all && make run-curses
```

## Technologies Used

- **Assembly Language**: NASM for low-level programming
- **C Language**: Kernel and driver development
- **QEMU**: System emulation and testing
- **GRUB**: Bootloader for OS projects
- **Make**: Build automation

## Learning Objectives

- Low-level hardware programming
- Operating system fundamentals
- Interrupt handling and drivers
- Memory management
- Assembly-C integration
- Build system management
