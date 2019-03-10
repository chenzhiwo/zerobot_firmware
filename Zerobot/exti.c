#include "exti.h"

void extiIRQHandler(uint16_t gpio_pin)
{
	size_t i = 0;
	for (i = 0; i < N_EXTI_DEVS; i++)
	{
		if (gpio_pin == EXTI_DEVS[i]->gpio->pin)
		{
			if (EXTI_DEVS[i]->callback != NULL)
			{
				EXTI_DEVS[i]->callback(EXTI_DEVS[i]);
			}
		}
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	extiIRQHandler(GPIO_Pin);
}
