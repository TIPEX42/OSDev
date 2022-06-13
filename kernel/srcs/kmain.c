#include "io/vga.h"

i8 *text = "hello";

int kmain(void)
{
	vga_init();
	for (u32 i = 0; i < 160; i++)
		vga_write("Hello, world!\n", 13);
	return (0);
}