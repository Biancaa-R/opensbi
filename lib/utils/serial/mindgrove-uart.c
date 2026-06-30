/*
	* @author Biancaa Ramesh <biancaa2210329@ssn.edu.in>
 */

#include <sbi/riscv_io.h>
#include <sbi/sbi_console.h>
#include <sbi_utils/serial/mindgrove-uart.h>

#define REG_BAUD	0x00
#define REG_TX		0x04
#define REG_RX		0x08
#define REG_STATUS	0x0C
#define REG_DELAY	0x10
#define REG_CONTROL	0x14
#define REG_INT_EN	0x18
#define REG_IQ_CYCLES	0x1C
#define REG_RX_THRES	0x20

#define UART_CAN_TAKE_INPUT 1 << 10
#define UART_OUTP_READY     1 << 9 
#define UART_RX_THRESHOLD   1 << 8
#define BREAK_ERROR	        1 << 7
#define FRAME_ERROR	        1 << 6
#define OVERRUN        	    1 << 5
#define PARITY_ERROR        1 << 4
#define UART_RX_FULL 	    1 << 3
#define UART_RX_NOT_EMPTY   1 << 2
#define UART_TX_FULL 	    1 << 1
#define UART_TX_EMPTY 	    1 << 0

static volatile void *uart_base;

void mindgrove_uart_putc(char ch)
{
	while((readw(uart_base + REG_STATUS) & UART_TX_FULL))
		;
	writeb(ch, uart_base + REG_TX);
}

int mindgrove_uart_getc(void)
{
	u16 status = readw(uart_base + REG_STATUS);
	if (status & UART_RX_NOT_EMPTY)
		return readb(uart_base + REG_RX);
	return -1;
}

static struct sbi_console_device mindgrove_console = {
	.name = "mindgrove_uart_opensbi's",
	.console_putc = mindgrove_uart_putc,
	.console_getc = mindgrove_uart_getc
};

int mindgrove_uart_init(unsigned long base, u32 in_freq, u32 baudrate)
{
	uart_base = (volatile void *)base;
	u16 baud = (u16)(in_freq/(16 * baudrate));
	writew(baud, uart_base + REG_BAUD);
	sbi_printf("Mindgrove UART init: base=%lx freq=%u baud=%u\n",
           base, in_freq, baudrate);

	sbi_console_set_device(&mindgrove_console);

	return 0;
}