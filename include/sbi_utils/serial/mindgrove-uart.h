/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2025 M. KAPIL SHYAM <kapil@mindgrovetech.in>
 */

#ifndef __SERIAL_MINDGROVE_UART_H__
#define __SERIAL_MINDGROVE_UART_H__

#include <sbi/sbi_types.h>

int mindgrove_uart_init(unsigned long base, u32 in_freq, u32 baudrate);

#endif