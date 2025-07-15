#ifndef INCLUDE_FRAMEBUFFER_H
#define INCLUDE_FRAMEBUFFER_H

/* Framebuffer color constants */
#define FB_BLACK         0
#define FB_BLUE          1
#define FB_GREEN         2
#define FB_CYAN          3
#define FB_RED           4
#define FB_MAGENTA       5
#define FB_BROWN         6
#define FB_LIGHT_GREY    7
#define FB_DARK_GREY     8
#define FB_LIGHT_BLUE    9
#define FB_LIGHT_GREEN   10
#define FB_LIGHT_CYAN    11
#define FB_LIGHT_RED     12
#define FB_LIGHT_MAGENTA 13
#define FB_LIGHT_BROWN   14
#define FB_WHITE         15

/* Framebuffer dimensions */
#define FB_WIDTH  80
#define FB_HEIGHT 25

/* Framebuffer API functions */

/**
 * Write a character with given foreground and background to position i in the framebuffer.
 *
 * @param i  The location in the framebuffer
 * @param c  The character
 * @param fg The foreground color
 * @param bg The background color
 */
void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg);

/**
 * Write a string with given foreground and background to the framebuffer.
 *
 * @param buf The string to write
 * @param fg  The foreground color
 * @param bg  The background color
 */
void fb_write(char *buf, unsigned char fg, unsigned char bg);

/**
 * Move the cursor to the given position.
 *
 * @param x The x coordinate (column)
 * @param y The y coordinate (row)
 */
void fb_move_cursor(unsigned short x, unsigned short y);

/**
 * Clear the framebuffer with given background color.
 *
 * @param bg The background color
 */
void fb_clear(unsigned char bg);

/**
 * Move cursor to given x, y position (2D API).
 *
 * @param x The x coordinate (column)
 * @param y The y coordinate (row)
 */
void fb_move(unsigned short x, unsigned short y);

/**
 * Write a string at current cursor position.
 *
 * @param str The string to write
 * @param fg  The foreground color
 * @param bg  The background color
 */
void fb_write_string(char *str, unsigned char fg, unsigned char bg);

/**
 * Write a number at current cursor position.
 *
 * @param num The number to write
 * @param fg  The foreground color
 * @param bg  The background color
 */
void fb_write_number(int num, unsigned char fg, unsigned char bg);

/**
 * Set the text color for subsequent writes.
 *
 * @param fg The foreground color
 * @param bg The background color
 */
void fb_set_color(unsigned char fg, unsigned char bg);

/**
 * Write a character at current cursor position and advance cursor.
 *
 * @param c  The character to write
 * @param fg The foreground color
 * @param bg The background color
 */
void fb_write_char(char c, unsigned char fg, unsigned char bg);

#endif /* INCLUDE_FRAMEBUFFER_H */
