#include "io/io.h"
#include "io/vga.h"
#include "types.h"
#include "vga_colors.h"

#define VGA_WIDTH 80

#define VGA_HEIGHT 25
#define VGA_TOTAL_SIZE VGA_WIDTH * VGA_HEIGHT
#define VGA_COMMAND_PORT 0x3D4

#define VGA_DATA_PORT 0x3D5
#define VGA_HIGH_BYTE_COMMAND 14

#define VGA_LOW_BYTE_COMMAND 15
struct vga_screen
{
	u8 cursor_x;
	u8 cursor_y;
	u8 color;
};

static u8 *framebuffer = (u8 *) 0x000B8000;
static struct vga_screen screen;

void vga_init(void)
{
	screen.color = VGA_White;
	screen.cursor_x = 0;
	screen.cursor_x = 0;

	vga_clear_screen();
	vga_set_cursor_pos(0, 0);
}

void vga_write(i8 *c, u32 len)
{
	for (u32 i = 0; i < len; i++) {
		vga_putchar(c[i]);
	}
}

void vga_putchar(u8 c)
{
	if (c == '\n')
		vga_newline();
	else if (c == '\b')
		vga_backspace();
	else
	{
		vga_set_char_at(c, screen.cursor_x, screen.cursor_y);
		screen.cursor_x++;
		if (screen.cursor_x >= VGA_WIDTH)
			vga_newline();
		else
			vga_set_cursor_pos(screen.cursor_x, screen.cursor_y);
	}
}

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

void vga_clear_screen(void)
{
	for (u8 y = 0; y < VGA_HEIGHT; y++) {
		for (u8 x = 0; x < VGA_WIDTH; x++) {
			vga_set_char_at(0, x, y);
			vga_set_background_at(VGA_Black, x, y);
			vga_set_foreground_at(screen.color, x, y);
		}
	}
}

u8 vga_get_char_at(u8 x, u8 y)
{
	return framebuffer[(x + y * VGA_WIDTH) * 2];
}

void vga_set_char_at(u8 c, u8 x, u8 y)
{
	framebuffer[(x + y * VGA_WIDTH) * 2] = c;
}

void vga_set_foreground_at(u8 color, u8 x, u8 y)
{
	framebuffer[(x + y * VGA_WIDTH) * 2 + 1] = color & 0x0F;
}

void vga_set_background_at(u8 color, u8 x, u8 y)
{
	framebuffer[(x + y * VGA_WIDTH) * 2 + 1] = (color & 0x0F) << 4;
}

void vga_newline(void)
{
	screen.cursor_x = 0;
	screen.cursor_y++;
	if (screen.cursor_y >= VGA_HEIGHT)
		vga_scroll();
	vga_set_cursor_pos(screen.cursor_x, screen.cursor_y);
}

void vga_backspace(void)
{
	if (screen.cursor_x != 0)
		screen.cursor_x -= 1;
	else
	{
		if (screen.cursor_y != 0) {
			screen.cursor_x = VGA_WIDTH - 1;
			screen.cursor_y--;
		}
	}
	vga_set_char_at(0, screen.cursor_x, screen.cursor_y);
	vga_set_cursor_pos(screen.cursor_x, screen.cursor_y);
}

void vga_scroll(void)
{
	for (u8 y = 0; y < VGA_HEIGHT - 1; y++) {
		for (u8 x = 0; x < VGA_WIDTH; x++) {
			vga_set_char_at(vga_get_char_at(x, y + 1), x, y);
		}
	}
	for (u8 x = 0; x < VGA_WIDTH; x++) {
		vga_set_char_at(0, x, VGA_HEIGHT - 1);
	}
	screen.cursor_y--;
}
