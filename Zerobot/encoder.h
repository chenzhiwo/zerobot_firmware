#ifndef ZEROBOT_ENCODER_H_
#define ZEROBOT_ENCODER_H_

#include "hardware.h"

static inline void encoderInit(void)
{
	int i = 0;
	for (i = 0; i < N_ENCODERS_DEVS; i++)
	{
		HAL_TIM_Encoder_Start(ENCODER_DEVS[i]->tim, TIM_CHANNEL_ALL);
	}
}

static inline int16_t encoderGetCounter(const EncoderDev * dev)
{
	int16_t ret = 0;
	ret = __HAL_TIM_GET_COUNTER(dev->tim);
	__HAL_TIM_SET_COUNTER(dev->tim, 0);
	return ret;
}

#endif /* ZEROBOT_ENCODER_H_ */
