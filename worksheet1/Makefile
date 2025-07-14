# Makefile for Assembly Programming Tasks
# Simple makefile following the task requirements

# Default rule - builds all targets
all: task1 task2

# Rule to build task1 executable
task1: task1.asm asm_io.asm driver.c
	nasm -f elf32 task1.asm -o task1.o
	nasm -f elf32 asm_io.asm -o asm_io.o
	gcc -m32 -o task1 task1.o asm_io.o driver.c

# Rule to build task2 executable  
task2: task2.asm asm_io.asm driver.c
	nasm -f elf32 task2.asm -o task2.o
	nasm -f elf32 asm_io.asm -o asm_io.o
	gcc -m32 -o task2 task2.o asm_io.o driver.c

# Clean rule to remove generated files
clean:
	rm -f *.o task1 task2
