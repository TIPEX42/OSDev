#pragma once

#include "types.h"

void	outportb(u16 port, u8 data);
u8		inportb(u16 port);

/*
 *  Waits a small and variable amount of time.
 *  Usefull for I/O port operations.
 */
inline static void io_wait(void)
{
	outportb(0x80, 0);
}
