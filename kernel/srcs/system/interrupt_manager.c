#include "interrupt.h"
#include "io/vga.h"
#include "io/io.h"
#include "io/pic.h"

void load_idt(void);

void interrupt_handler_0x00();
void interrupt_handler_0x08();
void interrupt_handler_0x0B();
void interrupt_handler_0x0D();
void interrupt_handler_0x0E();
void interrupt_handler_0x21();

struct cpu_state
{
	u32 eax;
	u32 ebx;
	u32 ecx;
	u32 edx;
	u32 esi;
	u32 edi;
	u32 ebp;
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
} __attribute__((packed));

struct idt_entry idt_entries[256];
struct idt_desc idt_desc;

static void	ft_print(i32 nb, i32 modulo)
{
	char	letter;
	i32		result;

	letter = '0';
	result = 0;
	while (nb >= modulo)
	{
		nb = nb - modulo;
		result = result + 1;
	}
	letter = letter + result;
	vga_write(&letter, 1);
	if (modulo > 1)
		ft_print(nb, modulo / 10);
}

void	ft_putnbr_fd(i32 n)
{
	char	c;
	i32		modulo;

	if (n < 0)
	{
		vga_write("-", 1);
		n = n * -1;
	}
	if (n < 10)
	{
		c = '0' + n;
		vga_write(&c, 1);
	}
	else
	{
		modulo = 10;
		while (modulo <= n)
			modulo = modulo * 10;
		modulo = modulo / 10;
		ft_print(n, modulo);
	}
}

void idt_initialize(void)
{
	idt_desc.base = (u32)idt_entries;
	idt_desc.size = sizeof(struct idt_entry) * 256 - 1;

	idt_set_entry(0x00, (u32) interrupt_handler_0x00); // Divide by zero
	idt_set_entry(0x08, (u32) interrupt_handler_0x08); // Double fault
	idt_set_entry(0x0B, (u32) interrupt_handler_0x0B); // Segment not present
	idt_set_entry(0x0D, (u32) interrupt_handler_0x0D); // General protection fault
	idt_set_entry(0x0E, (u32) interrupt_handler_0x0E); // Page fault
	idt_set_entry(0x21, (u32) interrupt_handler_0x21);

	load_idt();
}

void idt_set_entry(u32 entry, u32 func_address)
{
	idt_entries[entry].offset_low = func_address & 0xFFFF;
	idt_entries[entry].offset_high = (func_address >> 16) & 0xFFFF;

	idt_entries[entry].segment_selector = 0x08;

	idt_entries[entry].infos = 0x0E;
	idt_entries[entry].infos |= 0x80;

	idt_entries[entry].unused = 0x0;
}

void idt_interrupt_handler(struct cpu_state cpu, int interrupt, int error_code)
{
	if (interrupt == 0x0)
	{
		__asm__("cli");
		vga_write("Division by zero\n", 17);
		while (1)
			;
	}
	if (interrupt == 0x08)
	{
		__asm__("cli");
		vga_write("Double fault\n", 13);
		while (1)
			;
	}
	if (interrupt == 0x0B)
	{
		__asm__("cli");
		vga_write("Segment not present\n", 20);
		while (1)
			;
	}
	if (interrupt == 0x0D)
	{
		__asm__("cli");
		vga_write("General protection fault\n", 25);
		while (1)
			;
	}
	if (interrupt == 0x0E)
	{
		__asm__("cli");
		vga_write("Page fault\n", 11);
		while (1)
			;
	}
	(void)cpu;
	if (interrupt == 0x21)
	{
		i8 scan_code = (i8) inportb(0x60);
		vga_write(&scan_code, 1);
		pic_send_EOI(1);
	}
	else
		vga_write("wrong it\n", 9);
}
