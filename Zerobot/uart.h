#ifndef ZEROBOT_UART_H_
#define ZEROBOT_UART_H_

#include "hardware.h"

static inline void uartInit(void)
{
}

bool uartRecvStart(const UARTDev * dev, uint8_t * buffer, size_t len);

bool uartSendAsync(const UARTDev * dev, uint8_t * buffer, size_t len);

static inline size_t uartRecvIndex(const UARTDev * dev)
{
	return dev->uart->RxXferSize - __HAL_DMA_GET_COUNTER(dev->uart->hdmarx);
}

#endif /* ZEROBOT_UART_H_ */
