#include "io/vga.h"
#include "io/serial.h"
#include "system/gdt.h"
#include "system/interrupt.h"
#include "io/pic.h"

i8 *text = "hello";

int kmain(void)
{
	vga_initialize();
	vga_write("Hello, world!\n", 14);

	serial_initialize();
	serial_write("Hello, world!\n", 14);

	gdt_initialize();
	serial_write("gdt initialized!\n", 17);
	vga_write("gdt initialized!\n", 17);

	idt_initialize();
	serial_write("idt initialized!\n", 17);
	vga_write("idt initialized!\n", 17);

	pic_initialize();
	serial_write("pic initialized!\n", 17);
	vga_write("pic initialized!\n", 17);
	__asm__("sti");
	return (0);
}