#include "io/vga.h"
#include "io/serial.h"
#include "system/gdt.h"

i8 *text = "hello";

int kmain(void)
{
	vga_init();
	vga_write("Hello, world!\n", 14);

	serial_initialize();
	serial_write("Hello, world!\n", 14);

	gdt_init();
	serial_write("gdt initialized!\n", 16);
	vga_write("gdt initialized!\n", 16);
	return (0);
}