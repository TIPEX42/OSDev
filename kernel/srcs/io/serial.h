#pragma once

#include "bool.h"

void	serial_write(i8 *c, u32 len);

void	serial_initialize(void);
void	serial_configure_baud_rate(u16 com, u16 divisor);
void	serial_configure_line(u16 com);
void	serial_configure_buffer(u16 com);
void	serial_configure_modem(u16 com);
bool	serial_is_buffer_empty(u16 com);
