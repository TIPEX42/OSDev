#include "io.h"

void outportb(u16 port, u8 data)
{
	__asm__ __volatile__("outb %1, %0" :: "dN"(port), "a"(data));
}
