/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2020 Vijai Kumar K <vijai@behindbytes.com>
 * Someone save me from the Fate of ophelia 
 * Author : Biancaa Ramesh <biancaa2210329@ssn.edu.in>
 *
 */

#include <sbi_utils/fdt/fdt_helper.h>
#include <sbi_utils/serial/fdt_serial.h>
#include <sbi_utils/serial/mindgrove-uart.h>

#define MINDGROVE_UART_BASE   0x00011300UL 
#define SECURE_IOT_UART_FREQ   50000000
#define SECURE_IOT_UART_BAUD  115200

static int serial_mindgrove_init(const void *fdt, int nodeoff,
			      const struct fdt_match *match)
{
	int rc;
	struct platform_uart_data uart = { 
		.addr = MINDGROVE_UART_BASE,
		.freq = SECURE_IOT_UART_FREQ,
		.baud = SECURE_IOT_UART_BAUD
	 };

	rc = fdt_parse_mindgrove_uart_node(fdt, nodeoff, &uart);
	//rc=0;
	/*Why this is okay: Since you provided default values in the struct platform_uart_data uart initialization, forcing rc=0 ensures that even if OpenSBI fails to find the reg or clock-frequency properties in your DTS, it will still try to initialize the UART using your hardcoded defaults (0x11300, etc.).
	Recommendation: This is a safe "brute force" method for early debugging on custom silicon.*/
	/*if (rc)
		return rc;*/
	if (rc) {
	/* fallback to hardcoded defaults */
	uart.addr = MINDGROVE_UART_BASE;
	uart.freq = SECURE_IOT_UART_FREQ;
	uart.baud = SECURE_IOT_UART_BAUD;
	}

	return mindgrove_uart_init(uart.addr, uart.freq, uart.baud);
}

static const struct fdt_match serial_mindgrove_match[] = {
	{ .compatible = "mindgrove,uart" },
	{ },
};

const struct fdt_driver fdt_serial_mindgrove = {
	.match_table = serial_mindgrove_match,
	.init = serial_mindgrove_init
};

__attribute__((used, section(".fdt_serial_drivers")))
static const struct fdt_driver *fdt_serial_mindgrove_ptr = &fdt_serial_mindgrove;

//FDT_SERIAL_DRIVER(fdt_serial_mindgrove);
