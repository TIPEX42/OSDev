#include "io/vga.h"

i8 *text = "hello";

int kmain(void)
{
	vga_init();
	vga_write("Hello, world!", 13);
	return (0);
}