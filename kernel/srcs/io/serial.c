#include "serial.h"
#include "io.h"

// I/O ports
#define SERIAL_COM1_BASE 0x3F8 // Com1 base port

#define SERIAL_DATA_PORT(base) (base)
#define SERIAL_FIFO_COMMAND_PORT(base) (base + 2)
#define SERIAL_LINE_COMMAND_PORT(base) (base + 3)
#define SERIAL_MODEM_COMMAND_PORT(base) (base + 4)
#define SERIAL_LINE_STATUS_PORT(base) (base + 5)

// I/O commands

// Tells the serial port to expect the first highest 8 bits on the data port,
// then the 8 lowest bits
#define SERIAL_LINE_ENABLE_DLAB 0x80

/**
 *  Initializes the serial I/O device of port com1
 */
void serial_initialize()
{
	serial_configure_baud_rate(SERIAL_COM1_BASE, 1);
	serial_configure_line(SERIAL_COM1_BASE);
	serial_configure_modem(SERIAL_COM1_BASE);
	serial_configure_buffer(SERIAL_COM1_BASE);
}

/**
 *  Write the bytes buffer 'c' of len 'len' to the serial port com1
 *
 *  @param c	The byte buffer
 *  @param len	The len of the buffer
 */
 void serial_write(i8 *c, u32 len)
{
	for (u32 i = 0; i < len; i++) {
		while (!serial_is_buffer_empty(SERIAL_COM1_BASE))
			;
		outportb(SERIAL_DATA_PORT(SERIAL_COM1_BASE), c[i]);
	}
}

/**
 *  Sets the speed of the communication to the serial port.
 *  Default speed is 115200 bits/s.
 *  The argument divisor helps adjusting this rate by dividing it.
 *
 *  @param com		The com port to configure
 *  @param divisor	The divisor
 */
void serial_configure_baud_rate(u16 com, u16 divisor)
{
	outportb(SERIAL_LINE_COMMAND_PORT(com), SERIAL_LINE_ENABLE_DLAB);
	outportb(SERIAL_DATA_PORT(com), (divisor >> 8) & 0x00FF);
	outportb(SERIAL_DATA_PORT(com), divisor & 0x00FF);
}

/**
 *   Configures the line of the given serial port.
 *   The port is set to have a data length of 8 bits,
 *   no parity bits, one stop bit and break control disabled.
 *
 *   @param com  The serial port to configure
 */
void serial_configure_line(u16 com)
{
	/* Bit:     | 7 | 6 | 5 4 3 | 2 | 1 0 |
	 * Content: | d | b | prty  | s | dl  |
	 * Value:   | 0 | 0 | 0 0 0 | 0 | 1 1 | = 0x03
	 */
	outportb(SERIAL_LINE_COMMAND_PORT(com), 0x03);
}

/**
 *  Configures the FIFO buffer of the serial port.
 *
 *  @param com  The serial port to configure
*/
void serial_configure_buffer(u16 com)
{
	/* Bit:     | 7 6 | 5  | 4 |  3  |  2  |  1  | 0 |
	* Content: | lvl | bs | r | dma | clt | clr | e |
	* Value:   | 1 1 | 0 | 0 |  0  |  1  |  1  | 1 | = 0xC7
	*/
	outportb(SERIAL_FIFO_COMMAND_PORT(com), 0xC7);
}


/**
 *  Configures the modem of the serial port.
 *
 *  @param com  The serial port to configure
*/
void serial_configure_modem(u16 com)
{
	/*Bit:     | 7 | 6 | 5  | 4  | 3   | 2   | 1   | 0   |
	* Content: | r | r | af | lb | ao2 | ao1 | rts | dtr |
	* Value:   | 0 | 0 | 0 |  0  |  0  |  1  | 1 | = 0x03
	*/
	outportb(SERIAL_MODEM_COMMAND_PORT(com), 0x03);
}

/**
 *  Checks whether the transmit FIFO queue is empty or not for the given COM
 *  port.
 *
 *  @param  com The COM port
 *  @return 0 if the transmit FIFO queue is not empty
 *          1 if the transmit FIFO queue is empty
 */
bool serial_is_buffer_empty(u16 com)
{
	return inportb(SERIAL_LINE_STATUS_PORT(com)) & 0x20;
}
