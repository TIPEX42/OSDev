#include "io.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_TOTAL_SIZE VGA_WIDTH * VGA_HEIGHT

#define VGA_COMMAND_PORT 0x3D4
#define VGA_DATA_PORT 0x3D5

#define VGA_HIGH_BYTE_COMMAND 14
#define VGA_LOW_BYTE_COMMAND 15

void vga_set_cursor_pos(u8 x, u8 y)
{
	vga_set_cursor_index(x + y * VGA_WIDTH);
}

void vga_set_cursor_index(u16 index)
{
	if (index >= VGA_TOTAL_SIZE)
		return ;

	outportb(VGA_COMMAND_PORT, VGA_HIGH_BYTE_COMMAND);
	outportb(VGA_DATA_PORT, (index >> 8) & 0x00FF);
	outportb(VGA_COMMAND_PORT, VGA_LOW_BYTE_COMMAND);
	outportb(VGA_DATA_PORT, index & 0x00FF);
}