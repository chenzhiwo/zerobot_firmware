#ifndef ZEROBOT_LED_H_
#define ZEROBOT_LED_H_

#include "hardware.h"
#include "gpio.h"

static inline void ledInit(void)
{
}

static inline void ledToggle(const LEDDev * dev)
{
	gpioToggle(dev->gpio);
}

static inline void ledOn(const LEDDev * dev)
{
	gpioWrite(dev->gpio, dev->active_high ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

static inline void ledOff(const LEDDev * dev)
{
	gpioWrite(dev->gpio, dev->active_high ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

#endif /* ZEROBOT_LED_H_ */
