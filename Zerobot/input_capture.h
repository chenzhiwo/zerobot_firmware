#ifndef ZEROBOT_INPUT_CAPTURE_H_
#define ZEROBOT_INPUT_CAPTURE_H_

#include "hardware.h"

static inline void inputCaptureInit(void)
{
	// Initialize input captures.
	int i = 0;
	for (i = 0; i < N_INPUT_CAPTURE_DEVS; i++)
	{
		HAL_TIM_IC_Start_IT(INPUT_CAPTURE_DEVS[i]->tim, INPUT_CAPTURE_DEVS[i]->channel);
	}
}

static inline uint16_t inputCaptureGetValue(const InputCaptureDev * dev)
{
	return dev->value;
}

static inline void inputCaptureSetValue(InputCaptureDev * dev, uint16_t value)
{
	dev->value = value;
}


#endif /* ZEROBOT_INPUT_CAPTURE_H_ */
