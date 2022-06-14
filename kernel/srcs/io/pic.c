#include "io.h"

//https://wiki.osdev.org/PIC#What_does_the_8259_PIC_do.3F
//https://wiki.osdev.org/APIC
//TODO: Check for APIC support

#define PIC_MASTER_COMMAND_PORT	0x20
#define PIC_MASTER_DATA_PORT	0x21
#define PIC_SLAVE_COMMAND_PORT	0xA0
#define PIC_SLAVE_DATA_PORT		0xA1

#define PIC_EOI					0x20	// End-of-interrupt command code

/* reinitialize the PIC controllers, giving them specified vector offsets
   rather than 8h and 70h, as configured by default */

#define ICW1_ICW4	0x01		/* ICW4 (not) needed */
#define ICW1_SINGLE	0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	0x10		/* Initialization - required! */

#define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM	0x10		/* Special fully nested (not) */

static void pic_remap(int master_offset, int slave_offset);
//static void pic_send_EOI(u8 irq);
static void pic_set_mask(u8 irq_line);
static void pic_clear_mask(u8 irq_line);

/**
 *  Initialize the PIC
 */
void pic_initialize(void)
{
	pic_remap(0x20, 0x28);

	// Disabling all IRQs at first
	for (u8 i = 0; i < 16; i++) {
		pic_set_mask(i);
		io_wait();
	}

	// Only enable the Keyboard IRQ for now
	pic_clear_mask(1);
}

/**
 *  Remaps the pic and reset it's settings
 *
 * 	@param master_offset	Interrupts offset for master PIC
 * 	@param slave_offset		Interrupts offset for slave PIC
 */
static void pic_remap(int master_offset, int slave_offset)
{
	outportb(PIC_MASTER_COMMAND_PORT, ICW1_INIT | ICW1_ICW4);	// starts the initialization sequence (in cascade mode)
	io_wait();
	outportb(PIC_SLAVE_COMMAND_PORT, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outportb(PIC_MASTER_DATA_PORT, master_offset);				// ICW2: Master PIC vector offset
	io_wait();
	outportb(PIC_SLAVE_DATA_PORT, slave_offset);				// ICW2: Slave PIC vector offset
	io_wait();
	outportb(PIC_MASTER_DATA_PORT, 0x4);					// ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	io_wait();
	outportb(PIC_SLAVE_DATA_PORT, 0x2);					// ICW3: tell Slave PIC its cascade identity (0000 0010)
	io_wait();

	outportb(PIC_MASTER_DATA_PORT, ICW4_8086);
	io_wait();
	outportb(PIC_SLAVE_DATA_PORT, ICW4_8086);
	io_wait();

	outportb(PIC_MASTER_DATA_PORT, 0x0);					// restore saved masks.
	outportb(PIC_SLAVE_DATA_PORT, 0x0);
}

/**
 *  Enable handling of PIC's irq 'irq_line'
 *
 *  @param irq_line	The irq to enable
 */
static void pic_set_mask(u8 irq_line)
{
	u16 port;
	u8 value;

	if(irq_line < 8) {
		port = PIC_MASTER_DATA_PORT;
	} else {
		port = PIC_SLAVE_DATA_PORT;
		irq_line -= 8;
	}

	value = inportb(port) | (1 << irq_line);
	outportb(port, value);
}

/**
 *  Disable handling of PIC's irq 'irq_line'
 *
 *  @param irq_line	The irq to disable
 */
static void pic_clear_mask(u8 irq_line)
{
	u16 port;
	u8 value;

	if(irq_line < 8) {
		port = PIC_MASTER_DATA_PORT;
	} else {
		port = PIC_SLAVE_DATA_PORT;
		irq_line -= 8;
	}

	value = inportb(port) & ~(1 << irq_line);
	outportb(port, value);
}

void pic_send_EOI(u8 irq)
{
	if(irq >= 8)
		outportb(PIC_SLAVE_COMMAND_PORT,PIC_EOI);
	outportb(PIC_MASTER_COMMAND_PORT,PIC_EOI);
}
