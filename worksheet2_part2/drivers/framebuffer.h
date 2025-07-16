#ifndef INCLUDE_FRAMEBUFFER_H
#define INCLUDE_FRAMEBUFFER_H

#include "type.h"

// VGA framebuffer dimensions
#define FB_WIDTH 80
#define FB_HEIGHT 25

// VGA framebuffer colors
typedef enum {
    FB_BLACK = 0,
    FB_BLUE = 1,
    FB_GREEN = 2,
    FB_CYAN = 3,
    FB_RED = 4,
    FB_MAGENTA = 5,
    FB_BROWN = 6,
    FB_LIGHT_GREY = 7,
    FB_DARK_GREY = 8,
    FB_LIGHT_BLUE = 9,
    FB_LIGHT_GREEN = 10,
    FB_LIGHT_CYAN = 11,
    FB_LIGHT_RED = 12,
    FB_LIGHT_MAGENTA = 13,
    FB_LIGHT_BROWN = 14,
    FB_WHITE = 15
} fb_color;

// Legacy color constants for backward compatibility
#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define BROWN 6
#define LIGHT_GREY 7
#define DARK_GREY 8
#define LIGHT_BLUE 9
#define LIGHT_GREEN 10
#define LIGHT_CYAN 11
#define LIGHT_RED 12
#define LIGHT_MAGENTA 13
#define LIGHT_BROWN 14
#define WHITE 15

// Framebuffer functions
void fb_clear(unsigned char bg);
void fb_move(unsigned short pos_x, unsigned short pos_y);
void fb_write_string(char *str, unsigned char fg, unsigned char bg);
void fb_write_number(unsigned int num, unsigned char fg, unsigned char bg);
void fb_putc(char c, unsigned char fg, unsigned char bg);
void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg);
void fb_write_char(char c, unsigned char fg, unsigned char bg);
void fb_putchar(u8int c);
void fb_newline(void);
void fb_backspace(void);

#endif /* INCLUDE_FRAMEBUFFER_H */
