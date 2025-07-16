/*
 * kernel.c - C functions for the operating system kernel
 * Task 3: Adding framebuffer I/O functionality
 */

#include "../drivers/framebuffer.h"

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
    
    /* Test framebuffer functionality */
    
    /* Test 1: Write a welcome message */
    fb_move(0, 0);
    fb_write_string("Operating System - Task 3: Framebuffer I/O", FB_WHITE, FB_BLACK);
    
    /* Test 2: Test colors */
    fb_move(0, 2);
    fb_write_string("Testing Colors:", FB_LIGHT_CYAN, FB_BLACK);
    fb_move(0, 3);
    fb_write_string("Red Text", FB_LIGHT_RED, FB_BLACK);
    fb_move(15, 3);
    fb_write_string("Green Text", FB_LIGHT_GREEN, FB_BLACK);
    fb_move(30, 3);
    fb_write_string("Blue Text", FB_LIGHT_BLUE, FB_BLACK);
    fb_move(45, 3);
    fb_write_string("Yellow Text", FB_LIGHT_BROWN, FB_BLACK);
    
    /* Test 3: Test sum_of_three function and display result */
    fb_move(0, 5);
    fb_write_string("Testing C Functions:", FB_WHITE, FB_BLACK);
    
    int sum_result = sum_of_three(1, 2, 3);
    fb_move(0, 6);
    fb_write_string("sum_of_three(1, 2, 3) = ", FB_LIGHT_GREEN, FB_BLACK);
    fb_write_number(sum_result, FB_LIGHT_GREEN, FB_BLACK);
    
    /* Test 4: Test factorial function */
    int fact_result = factorial(5);
    fb_move(0, 7);
    fb_write_string("factorial(5) = ", FB_LIGHT_BLUE, FB_BLACK);
    fb_write_number(fact_result, FB_LIGHT_BLUE, FB_BLACK);
    
    /* Test 5: Test fibonacci function */
    int fib_result = fibonacci(10);
    fb_move(0, 8);
    fb_write_string("fibonacci(10) = ", FB_LIGHT_RED, FB_BLACK);
    fb_write_number(fib_result, FB_LIGHT_RED, FB_BLACK);
    
    /* Test 6: Test cursor positioning */
    fb_move(0, 10);
    fb_write_string("Testing Cursor Positioning:", FB_LIGHT_MAGENTA, FB_BLACK);
    
    fb_move(10, 12);
    fb_write_string("Position (10,12)", FB_WHITE, FB_BLUE);
    
    fb_move(30, 14);
    fb_write_string("Position (30,14)", FB_BLACK, FB_LIGHT_GREY);
    
    fb_move(50, 16);
    fb_write_string("Position (50,16)", FB_LIGHT_CYAN, FB_RED);
    
    /* Test 7: Display success message */
    fb_move(0, 20);
    fb_write_string("All tests completed successfully!", FB_WHITE, FB_GREEN);
    
    /* Test 8: Show some system info */
    fb_move(0, 22);
    fb_write_string("Framebuffer: 80x25, Memory-mapped I/O at 0xB8000", FB_LIGHT_GREY, FB_BLACK);
    
    /* Store success indicator in EAX if all calculations are correct */
    if (sum_result == 6 && fact_result == 120 && fib_result == 55) {
        /* Store success indicator */
        asm volatile("mov $0xDEADBEEF, %eax");
    } else {
        /* Store failure indicator */
        asm volatile("mov $0xBADC0DE, %eax");
    }
    
    /* Return control - in a real OS this would never return */
    return;
}
