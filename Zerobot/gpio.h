#ifndef ZEROBOT_GPIO_H_
#define ZEROBOT_GPIO_H_

#include "hardware.h"

static inline void gpioInit(void)
{
}

static inline void gpioToggle(const GPIODev * dev)
{
	HAL_GPIO_TogglePin(dev->port, dev->pin);
}

static inline void gpioWrite(const GPIODev * dev, bool state)
{
	HAL_GPIO_WritePin(dev->port, dev->pin,
			state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

#endif /* ZEROBOT_GPIO_H_ */
