#include "io/vga.h"
#include "io/serial.h"

i8 *text = "hello";

int kmain(void)
{
	vga_init();
	vga_write("Hello, world!\n", 13);

	serial_initialize();
	serial_write("Hello, world!", 13);
	return (0);
}