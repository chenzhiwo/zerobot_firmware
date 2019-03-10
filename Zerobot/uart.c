#include "uart.h"

bool uartRecvStart(const UARTDev * dev, uint8_t * buffer, size_t len)
{
	// Enable idle interrupt.
	__HAL_UART_ENABLE_IT(dev->uart, UART_IT_IDLE);
	return HAL_UART_Receive_DMA(dev->uart, buffer, len) == HAL_OK;
}

bool uartSendAsync(const UARTDev * dev, uint8_t * buffer, size_t len)
{
	return HAL_UART_Transmit_DMA(dev->uart, buffer, len) == HAL_OK;
}

// Deal with IDEL interrupt.
// Should be called before HAL_UART_IRQHandler().
void HAL_UART_ExtraIRQHandler(UART_HandleTypeDef * huart)
{
	uint32_t it_flag_idle = __HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE);
	uint32_t it_source_idle = __HAL_UART_GET_IT_SOURCE(huart, UART_IT_IDLE);
	if ((it_flag_idle != RESET) && (it_source_idle != RESET))
	{
		size_t i = 0;
		__HAL_UART_CLEAR_IDLEFLAG(huart);
		for (i = 0; i < N_UART_DEVS; i++)
		{
			if (huart == UART_DEVS[i]->uart)
			{
				if (UART_DEVS[i]->idle_callback != NULL)
				{
					UART_DEVS[i]->idle_callback(UART_DEVS[i]);
				}
			}
		}
	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef * huart)
{
	size_t i = 0;
	for (i = 0; i < N_UART_DEVS; i++)
	{
		if (huart == UART_DEVS[i]->uart)
		{
			if (UART_DEVS[i]->tc_callback != NULL)
			{
				UART_DEVS[i]->tc_callback(UART_DEVS[i]);
			}
		}
	}
}
