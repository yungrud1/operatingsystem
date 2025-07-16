#include "framebuffer.h"

/* The framebuffer address */
#define FB_ADDRESS 0x000B8000

/* I/O port addresses for cursor control */
#define FB_COMMAND_PORT         0x3D4
#define FB_DATA_PORT            0x3D5

/* Commands for cursor control */
#define FB_HIGH_BYTE_COMMAND    14
#define FB_LOW_BYTE_COMMAND     15

/* Global variables for cursor position and color */
static unsigned short cursor_x = 0;
static unsigned short cursor_y = 0;
static unsigned char current_fg = FB_WHITE;
static unsigned char current_bg = FB_BLACK;

/* Assembly functions for port I/O */
extern void outb(unsigned short port, unsigned char data);
extern unsigned char inb(unsigned short port);

/**
 * Write a character with given foreground and background to position i in the framebuffer.
 */
void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg)
{
    char *fb = (char *) FB_ADDRESS;
    fb[i] = c;
    fb[i + 1] = ((bg & 0x0F) << 4) | (fg & 0x0F);
}

/**
 * Move the cursor to the given position using I/O ports.
 */
static void fb_move_cursor_internal(unsigned short pos)
{
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    outb(FB_DATA_PORT, ((pos >> 8) & 0x00FF));
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    outb(FB_DATA_PORT, pos & 0x00FF);
}

/**
 * Move the cursor to the given x, y position.
 */
void fb_move_cursor(unsigned short x, unsigned short y)
{
    if (x < FB_WIDTH && y < FB_HEIGHT) {
        cursor_x = x;
        cursor_y = y;
        fb_move_cursor_internal(y * FB_WIDTH + x);
    }
}

/**
 * Write a string with given foreground and background to the framebuffer.
 */
void fb_write(char *buf, unsigned char fg, unsigned char bg)
{
    unsigned int i = 0;
    
    while (buf[i] != '\0') {
        fb_write_cell((cursor_y * FB_WIDTH + cursor_x) * 2, buf[i], fg, bg);
        cursor_x++;
        
        if (cursor_x >= FB_WIDTH) {
            cursor_x = 0;
            cursor_y++;
            if (cursor_y >= FB_HEIGHT) {
                cursor_y = 0;  /* Simple wrap-around */
            }
        }
        i++;
    }
    
    /* Update hardware cursor */
    fb_move_cursor_internal(cursor_y * FB_WIDTH + cursor_x);
}

/**
 * Clear the framebuffer with given background color.
 */
void fb_clear(unsigned char bg)
{
    unsigned int i;
    
    for (i = 0; i < FB_WIDTH * FB_HEIGHT; i++) {
        fb_write_cell(i * 2, ' ', FB_BLACK, bg);
    }
    
    cursor_x = 0;
    cursor_y = 0;
    fb_move_cursor_internal(0);
}

/**
 * Move cursor to given x, y position (2D API).
 */
void fb_move(unsigned short x, unsigned short y)
{
    fb_move_cursor(x, y);
}

/**
 * Set the text color for subsequent writes.
 */
void fb_set_color(unsigned char fg, unsigned char bg)
{
    current_fg = fg;
    current_bg = bg;
}

/**
 * Write a character at current cursor position and advance cursor.
 */
void fb_write_char(char c, unsigned char fg, unsigned char bg)
{
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
        if (cursor_y >= FB_HEIGHT) {
            cursor_y = 0;  /* Simple wrap-around */
        }
    } else {
        fb_write_cell((cursor_y * FB_WIDTH + cursor_x) * 2, c, fg, bg);
        cursor_x++;
        
        if (cursor_x >= FB_WIDTH) {
            cursor_x = 0;
            cursor_y++;
            if (cursor_y >= FB_HEIGHT) {
                cursor_y = 0;  /* Simple wrap-around */
            }
        }
    }
    
    /* Update hardware cursor */
    fb_move_cursor_internal(cursor_y * FB_WIDTH + cursor_x);
}

/**
 * Write a string at current cursor position.
 */
void fb_write_string(char *str, unsigned char fg, unsigned char bg)
{
    fb_write(str, fg, bg);
}

/**
 * Convert integer to string and write it.
 */
void fb_write_number(unsigned int num, unsigned char fg, unsigned char bg)
{
    char buffer[12];  /* Enough for 32-bit integer */
    int i = 0;
    
    if (num == 0) {
        fb_write_char('0', fg, bg);
        return;
    }
    
    /* Convert number to string (reverse order) */
    while (num > 0) {
        buffer[i++] = '0' + (num % 10);
        num /= 10;
    }
    
    /* Write string in correct order */
    while (i > 0) {
        fb_write_char(buffer[--i], fg, bg);
    }
}

/**
 * Write a character with default colors (white on black)
 */
void fb_putchar(u8int c)
{
    fb_write_char(c, FB_WHITE, FB_BLACK);
}

/**
 * Move cursor to next line
 */
void fb_newline(void)
{
    cursor_x = 0;
    cursor_y++;
    if (cursor_y >= FB_HEIGHT) {
        cursor_y = 0;  /* Simple wrap-around */
    }
    fb_move_cursor_internal(cursor_y * FB_WIDTH + cursor_x);
}
