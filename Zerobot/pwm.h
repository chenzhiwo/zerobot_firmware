#ifndef ZEROBOT_PWM_H_
#define ZEROBOT_PWM_H_

#include "hardware.h"

static inline void pwmInit(void)
{
	int i = 0;
	for(i = 0; i < N_PWM_DEVS; i++)
	{
		HAL_TIM_PWM_Start(PWM_DEVS[i]->tim, PWM_DEVS[i]->channel);
	}
}

static inline void pwmSetDutyCycle(const PWMDev * dev, float duty_cycle)
{
	uint16_t value = duty_cycle * __HAL_TIM_GET_AUTORELOAD(dev->tim);
	value = value > __HAL_TIM_GET_AUTORELOAD(dev->tim) ? __HAL_TIM_GET_AUTORELOAD(dev->tim) : value;
	__HAL_TIM_SET_COMPARE(dev->tim, dev->channel, value);
}

#endif /* ZEROBOT_PWM_H_ */
