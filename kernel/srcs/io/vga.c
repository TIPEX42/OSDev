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

static void vga_set_char_at(u8 c, u8 x, u8 y);
static void vga_set_foreground_at(u8 color, u8 x, u8 y);
static void vga_set_background_at(u8 color, u8 x, u8 y);

static void vga_set_cursor_index(u16 index);

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

static void vga_set_char_at(u8 c, u8 x, u8 y)
{
	framebuffer[(x + y * VGA_WIDTH) * 2] = c;
}

static void vga_set_foreground_at(u8 color, u8 x, u8 y)
{
	framebuffer[(x + y * VGA_WIDTH) * 2 + 1] = color & 0x0F;
}

static void vga_set_background_at(u8 color, u8 x, u8 y)
{
	framebuffer[(x + y * VGA_WIDTH) * 2 + 1] = (color & 0x0F) << 4;
}
