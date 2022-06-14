#pragma once

#include "types.h"

void	idt_initialize(void);
void	idt_set_entry(u32 entry, u32 func_address);
