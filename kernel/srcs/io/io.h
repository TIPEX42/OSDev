#pragma once

#include "types.h"

void outportb(u16 port, u8 data);

void vga_set_cursor_pos(u8 x, u8 y);
void vga_set_cursor_index(u16 index);
