/*
 * kernel.c - C functions for the operating system kernel
 * Part 2: Interactive Keyboard Input and Terminal Interface
 */

#include "../drivers/framebuffer.h"
#include "../drivers/interrupts.h"
#include "../drivers/pic.h"
#include "../drivers/keyboard.h"

/* Function 1: sum_of_three as specified in the book */
int sum_of_three(int arg1, int arg2, int arg3) {
    return arg1 + arg2 + arg3;
}

/* Function 2: factorial - calculates factorial of a number */
int factorial(int n) {
    if (n <= 1) {
        return 1;
    }
    int result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

/* Function 3: fibonacci - calculates nth fibonacci number */
int fibonacci(int n) {
    if (n <= 0) {
        return 0;
    }
    if (n == 1) {
        return 1;
    }
    
    int a = 0, b = 1, result = 0;
    for (int i = 2; i <= n; i++) {
        result = a + b;
        a = b;
        b = result;
    }
    return result;
}

/* Main C function called from assembly */
void kmain(void) {
    /* Clear the screen with black background */
    fb_clear(FB_BLACK);
    
    /* Display welcome message */
    fb_move(0, 0);
    fb_write_string("Operating System - Part 2: Interactive Terminal", FB_WHITE, FB_BLACK);
    fb_move(0, 1);
    fb_write_string("Initializing keyboard and interrupt system...", FB_LIGHT_CYAN, FB_BLACK);
    
    /* Initialize interrupt system */
    interrupts_install_idt();
    
    /* Initialize PIC */
    pic_remap(32, 40);
    
    /* Enable interrupts */
    asm volatile("sti");
    
    /* Display ready message */
    fb_move(0, 3);
    fb_write_string("System ready! Type 'help' for available commands.", FB_LIGHT_GREEN, FB_BLACK);
    
    /* Start the terminal interface */
    terminal_main();
    
    /* Should never reach here */
    return;
}
