#include "config.h"

#if defined(CONFIG_UART_ENABLE)

#include <assert.h>
#include <stdint.h>

#include "stm32f1xx_hal_uart.h"

UART_HandleTypeDef UartHandle;

void uart_init(uint32_t baudrate)
{
  HAL_UART_MspInit(NULL);

  UartHandle.Instance        = USART1;
  UartHandle.Init.BaudRate   = baudrate;
  UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits   = UART_STOPBITS_1;
  UartHandle.Init.Parity     = UART_PARITY_NONE;
  UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode       = UART_MODE_TX_RX;
  HAL_UART_Init(&UartHandle);
}

void uart_putc(uint8_t data)
{
	HAL_UART_Transmit(&UartHandle, (uint8_t *)&data, 1, 0xFFFF);
}


#endif