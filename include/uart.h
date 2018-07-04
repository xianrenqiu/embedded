#ifndef __UART_H__
#define __UART_H__

#include "config.h"

#if defined(CONFIG_UART_ENABLE)

#include <stdint.h>

void uart_init(uint32_t baudrate);
void uart_putc(uint8_t data);

#endif

#endif /* __UART_H__ */
