#ifndef ZEROBOT_HARDWARE_H_
#define ZEROBOT_HARDWARE_H_

#include "hal.h"

#define ARR_LEN(arr) (sizeof(arr) / sizeof(arr[0]))

typedef struct GPIODev
{
	GPIO_TypeDef * port;
	uint16_t pin;
} GPIODev;
extern GPIODev * GPIO_DEVS[];
extern const size_t N_GPIO_DEVS;

typedef struct EXTIDev
{
	GPIODev * gpio;
	void (*callback)(const struct EXTIDev * dev);
} EXTIDev;
extern EXTIDev * EXTI_DEVS[];
extern const size_t N_EXTI_DEVS;

typedef struct LEDDev
{
	GPIODev * gpio;
	bool active_high;
} LEDDev;
extern LEDDev * LED_DEVS[];
extern const size_t N_LED_DEVS;

typedef struct PWMDev
{
	TIM_HandleTypeDef *tim;
	uint32_t channel;
} PWMDev;
extern PWMDev * PWM_DEVS[];
extern const size_t N_PWM_DEVS;

typedef struct EncoderDev
{
	TIM_HandleTypeDef *tim;
} EncoderDev;
extern EncoderDev * ENCODER_DEVS[];
extern const size_t N_ENCODERS_DEVS;

typedef struct InputCaptureDev
{
	TIM_HandleTypeDef *tim;
	uint32_t channel;
	HAL_TIM_ActiveChannel active_channel;
	uint32_t start;
	uint32_t value;
} InputCaptureDev;
extern InputCaptureDev * INPUT_CAPTURE_DEVS[];
extern const size_t N_INPUT_CAPTURE_DEVS;

typedef struct UARTDev
{
	UART_HandleTypeDef *uart;
	uint8_t * rx_buffer;
	uint8_t * tx_buffer;
	void (*idle_callback)(const struct UARTDev * dev);
	void (*tc_callback)(const struct UARTDev * dev);
	void * args;
} UARTDev;
extern UARTDev * UART_DEVS[];
extern const size_t N_UART_DEVS;

typedef struct SPIDev
{
	SPI_HandleTypeDef *spi;
	GPIODev * cs;
} SPIDev;
extern SPIDev * SPI_DEVS[];
extern const size_t N_SPI_DEVS;

#endif /* ZEROBOT_HARDWARE_H_ */
