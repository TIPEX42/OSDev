#include "types.h"

#define Black 0;
#define Blue 1
#define Green 2
#define Cyan 3
#define Red 4
#define Magenta 5
#define Brown 6
#define LightGrey 7
#define DarkGrey 8
#define LightBlue 9
#define LightGreen 10
#define LightCyan 11
#define LightRed 12
#define LightMagenta 13
#define LightBrown 14
#define White 15

u8 *framebuffer = (u8 *) 0x000B8000;

void write_cell(u32 i, u8 c, u8 fg, u8 bg)
{
	framebuffer[i * 2] = c;
	framebuffer[i * 2 + 1] = ((fg & 0x0F) << 4) | (bg & 0x0F);
}

int kmain(void)
{
	write_cell(0, 'H', LightBlue, White);
	return (0);
}