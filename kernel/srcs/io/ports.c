#include "io.h"

void outportb(u16 port, u8 data)
{
	__asm__ __volatile__("outb %1, %0"	:
										: "dN"(port), "a"(data));
}

u8 inportb(u16 port)
{
	u8 data;
	__asm__ __volatile__("inb %1, %0"	: "=a"(data)
										: "dN"(port));
	return data;
}
