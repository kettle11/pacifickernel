/* Surely you will remove the processor conditionals and this comment
   appropriately depending on whether or not you use C++. */
#if !defined(__cplusplus)
#include <stdbool.h> /* C doesn't have booleans by default. */
#endif
#include <stddef.h>
#include <stdint.h>

#include "helper.c"

#include <system.h>
#include "events.h"

/* Hardware text mode color constants. */
enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};
 

/* The I/O ports */
#define FB_COMMAND_PORT         0x3D4
#define FB_DATA_PORT            0x3D5

/* The I/O port commands */
#define FB_HIGH_BYTE_COMMAND    14
#define FB_LOW_BYTE_COMMAND     15

/** fb_move_cursor:
 *  Moves the cursor of the framebuffer to the given position
 *
 *  @param pos The new position of the cursor
 */
void fb_move_cursor(unsigned short pos)
{
    outp(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    outp(FB_DATA_PORT,    ((pos >> 8) & 0x00FF));
    outp(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    outp(FB_DATA_PORT,    pos & 0x00FF);
}

// enable the cursor and set the cursor start and end scanlines
void enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
{
        outp(0x3D4, 0x0A);
        outp(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);
 
        outp(0x3D4, 0x0B);
        outp(0x3D5, (inb(0x3E0) & 0xE0) | cursor_end);
}

 /* void disable_cursor()
  * by Elad Ashkcenazi
  * year 2017
  */
 void disable_cursor()
 {
    outp(0x3D4, 0x0A); // LOW cursor shape port to vga INDEX register
    outp(0x3D5, 0x3f); //bits 6-7 must be 0 , if bit 5 set the cursor is disable  , bits 0-4 controll the cursor shape .
 }

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
	return fg | bg << 4;
}
 
static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
	return (uint16_t) uc | (uint16_t) color << 8;
}
 
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
 
size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;
 
void terminal_initialize(void) {
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_CYAN, VGA_COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xB8000;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}
 
void move_up()
{
	for (size_t y = 1; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			const size_t targetIndex = (y-1) * VGA_WIDTH + x;

			terminal_buffer[targetIndex] = terminal_buffer[index];
		}
	}

	for (size_t x = 0; x < VGA_WIDTH; x++) {
		terminal_buffer[x+VGA_WIDTH*(VGA_HEIGHT-1)] = vga_entry(' ', terminal_color);
	}

	--terminal_row;
}

void terminal_setcolor(uint8_t color) {
	terminal_color = color;
}
 
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void update_cursor()
{
	fb_move_cursor((terminal_row+1) * VGA_WIDTH + terminal_column);
}
 
void terminal_putchar(char c) {
	if (c == '\n')
	{
		++terminal_row;
		terminal_column = 0;

		if (terminal_row == VGA_HEIGHT)
		{
			move_up();
		}
	}
	else
	{
		terminal_putentryat(c, terminal_color, terminal_column, terminal_row);

		if (++terminal_column == VGA_WIDTH) {
			terminal_column = 0;
			if (++terminal_row == VGA_HEIGHT)
				move_up();
		}
	}

	update_cursor();
}

void terminal_removelastchar()
{
	if (terminal_row > 0 || terminal_column > 0)
	{
		--terminal_column;
		if (terminal_column < 0)
		{
			terminal_row -= 1;
			terminal_column = VGA_WIDTH - 1;
		}

		terminal_putentryat(' ', terminal_color, terminal_column, terminal_row);
	}

	update_cursor();
}

char * itoa( int value, char * str, int base )
{
    char * rc;
    char * ptr;
    char * low;
    // Check for supported base.
    if ( base < 2 || base > 36 )
    {
        *str = '\0';
        return str;
    }
    rc = ptr = str;
    // Set '-' for negative decimals.
    if ( value < 0 && base == 10 )
    {
        *ptr++ = '-';
    }
    // Remember where the numbers start.
    low = ptr;
    // The actual conversion.
    do
    {
        // Modulo is negative for negative value. This trick makes abs() unnecessary.
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + value % base];
        value /= base;
    } while ( value );
    // Terminating the string.
    *ptr-- = '\0';
    // Invert the numbers.
    while ( low < ptr )
    {
        char tmp = *low;
        *low++ = *ptr;
        *ptr-- = tmp;
    }
    return rc;
}



void terminal_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}
 
void terminal_write_char(const char data)
{
	terminal_putchar(data);
}

void terminal_writestring(const char* data) {
	terminal_write(data, strlen(data));
}

void terminal_writeint(int num) {
	char buffer[500];
	itoa(num, buffer, 10);
	terminal_writestring(buffer);
}

void terminal_writeintbase(int num, int base) {
	char buffer[500];
	itoa(num, buffer, base);

	if (base == 16)
	{
		terminal_writestring("0x");
	}
	
	terminal_writestring(buffer);
}

void handle_key_press(struct KeyboardEvent event)
{
	if (event.pressed)
	{
		switch (event.scancode)
		{
			case 0x0E: // Backspace
				terminal_removelastchar();
				break;
			case 0x2A: // left shift
				break;
			case 0x36: // right shift
				break;
			default:
				terminal_write_char(event.character);
		}
	}
}

void vga_setup()
{
	assign_key_press_handler(handle_key_press);

	enable_cursor(0,1);
	fb_move_cursor(100);
}