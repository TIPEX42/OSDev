#include "interrupt.h"
#include "types.h"

void load_idt(void);

struct cpu_state
{
	u32 eax;
	u32 ebx;
	u32 ecx;
	u32 edx;
	u32 esi;
	u32 edi;
	u32 ebp;
	u32 eip;
	u32 esp;
} __attribute__((packed));

struct stack_state {
	u32 error_code;
	u32 eip;
	u32 cs;
	u32 eflags;
} __attribute__((packed));

struct idt_entry
{
	u16 offset_low;
	u16 segment_selector;
	u8 unused;
	u8 infos;
	u16 offset_high;
} __attribute__((packed));

struct idt_desc
{
	u16 size;
	u32 base;
};

struct idt_entry idt_entries[256];
struct idt_desc idt_desc;

void idt_initialize(void)
{
	idt_desc.base = (u32)idt_entries;
	idt_desc.size = sizeof(struct idt_entry) * 256 - 1;
	load_idt();
}

void interrupt_handler(struct cpu_state cpu, struct stack_state stack, int interrupt)
{
	(void)cpu;
	(void)stack;
	(void)interrupt;
}
