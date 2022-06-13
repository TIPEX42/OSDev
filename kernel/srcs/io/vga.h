#pragma once

#include "types.h"

void	vga_init(void);
void	vga_clear_screen(void);

void	vga_set_cursor_pos(u8 x, u8 y);
void	vga_set_cursor_index(u16 index);

void	vga_write(i8 *c, u32 len);
void	vga_putchar(u8 c);
u8		vga_get_char_at(u8 x, u8 y);
void	vga_set_char_at(u8 c, u8 x, u8 y);
void	vga_set_foreground_at(u8 color, u8 x, u8 y);
void	vga_set_background_at(u8 color, u8 x, u8 y);

void	vga_newline(void);
void	vga_backspace(void);

void 	vga_scroll(void);
