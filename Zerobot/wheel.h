#ifndef ZEROBOT_WHEEL_H_
#define ZEROBOT_WHEEL_H_

#include "hardware.h"
#include "gpio.h"
#include "pwm.h"
#include "encoder.h"
#include "arm_math.h"

typedef struct WheelDev {
	PWMDev * pwm;
	EncoderDev * encoder;
	GPIODev * dire_p;
	GPIODev * dire_n;
	float circumference;
	float pulse_per_round;
} WheelDev;

extern WheelDev * WHEEL_DEVS[];

extern const size_t N_WHEEL_DEVS;

static inline void wheelInit(void) {
}

static inline void wheelSetOutput(const WheelDev * dev, float output) {
	if (output > 0.0) {
		pwmSetDutyCycle(dev->pwm, output);
		gpioWrite(dev->dire_p, true);
		gpioWrite(dev->dire_n, false);
	} else {
		pwmSetDutyCycle(dev->pwm, -output);
		gpioWrite(dev->dire_p, false);
		gpioWrite(dev->dire_n, true);
	}
}

static inline float wheelGetSpeed(const WheelDev * dev, float detla_time) {
	return (dev->circumference * encoderGetCounter(dev->encoder))
			/ (dev->pulse_per_round * detla_time);
}

#endif /* ZEROBOT_WHEEL_H_ */
