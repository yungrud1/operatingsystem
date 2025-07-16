#include "interrupts.h"
#include "pic.h"
#include "io.h"
#include "framebuffer.h"
#include "keyboard.h"

#define INTERRUPTS_DESCRIPTOR_COUNT 256 
#define INTERRUPTS_KEYBOARD 33 
#define INPUT_BUFFER_SIZE 256

u8int input_buffer[INPUT_BUFFER_SIZE];
u8int buffer_index = 0;
u8int buffer_read_index = 0;
u16int buffer_size = 0;

struct IDTDescriptor idt_descriptors[INTERRUPTS_DESCRIPTOR_COUNT];
struct IDT idt;

u32int BUFFER_COUNT = 0;
// Input buffer functions
void add_to_buffer(u8int c) {
    if (buffer_size < INPUT_BUFFER_SIZE) {
        input_buffer[buffer_index] = c;
        buffer_index = (buffer_index + 1) % INPUT_BUFFER_SIZE;
        buffer_size++;
    }
}

u8int getc() {
    if (buffer_size == 0) {
        return 0; // Buffer empty
    }
    
    u8int c = input_buffer[buffer_read_index];
    buffer_read_index = (buffer_read_index + 1) % INPUT_BUFFER_SIZE;
    buffer_size--;
    return c;
}

void readline(char* buffer, u32int max_length) {
    u32int index = 0;
    u8int c;
    
    while (index < max_length - 1) {
        // Wait for input (in a real system, this would be handled differently)
        while ((c = getc()) == 0) {
            // Wait for character
        }
        
        if (c == '\n' || c == '\r') {
            break;
        } else if (c == '\b') {
            if (index > 0) {
                index--;
                // Don't call fb_backspace here - it's already handled in interrupt handler
            }
        } else {
            buffer[index] = c;
            index++;
            // Don't display here - it's already displayed in interrupt handler
        }
    }
    
    buffer[index] = '\0'; // Null terminate
}

void interrupts_init_descriptor(s32int index, u32int address)
{
	idt_descriptors[index].offset_high = (address >> 16) & 0xFFFF; // offset bits 0..15
	idt_descriptors[index].offset_low = (address & 0xFFFF); // offset bits 16..31

	idt_descriptors[index].segment_selector = 0x08; // The second (code) segment selector in GDT: one segment is 64b.
	idt_descriptors[index].reserved = 0x00; // Reserved.

	/*
	   Bit:     | 31              16 | 15 | 14 13 | 12 | 11     10 9 8   | 7 6 5 | 4 3 2 1 0 |
	   Content: | offset high        | P  | DPL   | S  | D and  GateType | 0 0 0 | reserved
		P	If the handler is present in memory or not (1 = present, 0 = not present). Set to 0 for unused interrupts or for Paging.
		DPL	Descriptor Privilige Level, the privilege level the handler can be called from (0, 1, 2, 3).
		S	Storage Segment. Set to 0 for interrupt gates.
		D	Size of gate, (1 = 32 bits, 0 = 16 bits).
	*/
	idt_descriptors[index].type_and_attr =	(0x01 << 7) |			// P
						(0x00 << 6) | (0x00 << 5) |	// DPL
						0xe;				// 0b1110=0xE 32-bit interrupt gate
}

void interrupts_install_idt()
{
	
	interrupts_init_descriptor(INTERRUPTS_KEYBOARD, (u32int) interrupt_handler_33);


	idt.address = (s32int) &idt_descriptors;
	idt.size = sizeof(struct IDTDescriptor) * INTERRUPTS_DESCRIPTOR_COUNT;
	load_idt((s32int) &idt);

	/*pic_remap(PIC_PIC1_OFFSET, PIC_PIC2_OFFSET);*/
	pic_remap(PIC_1_OFFSET, PIC_2_OFFSET);

    // Unmask keyboard interrupt (IRQ1)
    outb(0x21, inb(0x21) & ~(1 << 1));
}


/* Interrupt handlers ********************************************************/

void interrupt_handler(__attribute__((unused)) struct cpu_state cpu, u32int interrupt, __attribute__((unused)) struct stack_state stack) {
    u8int input;
    u8int ascii;
    
    switch (interrupt) {
        case INTERRUPTS_KEYBOARD:
            while ((inb(0x64) & 1)) {
                input = keyboard_read_scan_code();
                // Only process if it's not a break code (key press, not release)
                if (!(input & 0x80)) {
                    if (input <= KEYBOARD_MAX_ASCII) {
                        ascii = keyboard_scan_code_to_ascii(input);
                        if (ascii != 0) {
                            // Handle display and buffer management
                            if (ascii == '\b') {
                                // Handle backspace - remove from buffer and display
                                if (buffer_size > 0) {
                                    // Remove last character from buffer
                                    if (buffer_index == 0) {
                                        buffer_index = INPUT_BUFFER_SIZE - 1;
                                    } else {
                                        buffer_index--;
                                    }
                                    buffer_size--;
                                    fb_backspace();
                                }
                            } else if (ascii == '\n') {
                                // Handle newline
                                add_to_buffer(ascii);
                                fb_newline();
                            } else {
                                // Add character to input buffer and display
                                add_to_buffer(ascii);
                                fb_putchar(ascii);
                            }
                        }
                    }
                }
            }
            pic_acknowledge(interrupt);
            break;
        default:
            break;
    }
}

// Terminal implementation
#define MAX_COMMAND_LENGTH 128
#define MAX_ARGS_LENGTH 100

struct command {
    const char* name;
    void (*function)(char* args);
};

// Command implementations
void cmd_echo(char* args) {
    if (args && *args) {
        while (*args) {
            fb_putchar(*args);
            args++;
        }
    }
    fb_newline();
}

void cmd_clear(char* args) {
    (void)args; // Unused parameter
    fb_clear(FB_BLACK);
    // Cursor position is handled internally by framebuffer
}

void cmd_help(char* args) {
    (void)args; // Unused parameter
    fb_write_string("Available commands:\n", FB_WHITE, FB_BLACK);
    fb_write_string("  echo [text] - Display the provided text\n", FB_WHITE, FB_BLACK);
    fb_write_string("  clear       - Clear the screen\n", FB_WHITE, FB_BLACK);
    fb_write_string("  help        - Show this help message\n", FB_WHITE, FB_BLACK);
    fb_write_string("  version     - Display OS version\n", FB_WHITE, FB_BLACK);
    // Cursor position is handled internally by framebuffer
}

void cmd_version(char* args) {
    (void)args; // Unused parameter
    fb_write_string("MyOS v1.0 - Operating System with Keyboard Input\n", FB_WHITE, FB_BLACK);
    fb_write_string("Built with interrupt-driven keyboard support\n", FB_WHITE, FB_BLACK);
    // Cursor position is handled internally by framebuffer
}

// Command table
struct command commands[] = {
    {"echo", cmd_echo},
    {"clear", cmd_clear},
    {"help", cmd_help},
    {"version", cmd_version},
    {0, 0} // End marker
};

// String utility functions
u32int string_length(const char* str) {
    u32int len = 0;
    while (str[len]) len++;
    return len;
}

int string_compare(const char* str1, const char* str2) {
    while (*str1 && *str2 && *str1 == *str2) {
        str1++;
        str2++;
    }
    return *str1 - *str2;
}

void string_copy(char* dest, const char* src, u32int max_len) {
    u32int i = 0;
    while (i < max_len - 1 && src[i]) {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

// Find first space in string, return index or -1 if not found
s32int find_space(const char* str) {
    s32int i = 0;
    while (str[i]) {
        if (str[i] == ' ') return i;
        i++;
    }
    return -1;
}

void process_command(char* input) {
    // Skip leading spaces
    while (*input == ' ') input++;
    
    // Check for empty input
    if (*input == '\0') return;
    
    // Find command and arguments
    s32int space_pos = find_space(input);
    char command[MAX_COMMAND_LENGTH];
    char* args = 0;
    
    if (space_pos >= 0) {
        // Command with arguments
        string_copy(command, input, space_pos + 1);
        command[space_pos] = '\0';
        args = input + space_pos + 1;
        // Skip leading spaces in arguments
        while (*args == ' ') args++;
    } else {
        // Command without arguments
        string_copy(command, input, MAX_COMMAND_LENGTH);
        args = 0;
    }
    
    // Look up command
    u32int i = 0;
    while (commands[i].name) {
        if (string_compare(command, commands[i].name) == 0) {
            commands[i].function(args);
            return;
        }
        i++;
    }
    
    // Unknown command
    fb_write_string("Unknown command: ", FB_LIGHT_RED, FB_BLACK);
    fb_write_string(command, FB_LIGHT_RED, FB_BLACK);
    fb_write_string("\nType 'help' for available commands.\n", FB_WHITE, FB_BLACK);
    // Cursor position is handled internally by framebuffer
}

void show_prompt() {
    fb_write_string("myos> ", FB_LIGHT_GREEN, FB_BLACK);
    // Cursor position is handled internally by framebuffer
}

void terminal_main() {
    char input[MAX_COMMAND_LENGTH];
    
    // Initialize terminal
    cmd_clear(0);
    fb_write_string("Welcome to MyOS Terminal!\n", FB_LIGHT_CYAN, FB_BLACK);
    fb_write_string("Type 'help' for available commands.\n\n", FB_WHITE, FB_BLACK);
    // Cursor position is handled internally by framebuffer
    
    while (1) {
        show_prompt();
        readline(input, MAX_COMMAND_LENGTH);
        fb_newline();
        process_command(input);
    }
}
