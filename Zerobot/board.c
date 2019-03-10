#include "board.h"
#include "main.h"
#include "hardware.h"
#include "gpio.h"
#include "exti.h"
#include "led.h"
#include "pwm.h"
#include "encoder.h"
#include "input_capture.h"
#include "uart.h"
#include "spi.h"
#include "wheel.h"
#include "nrf.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim8;
extern TIM_HandleTypeDef htim12;

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

extern SPI_HandleTypeDef hspi1;

// GPIO
GPIODev gpio_dir0_p = { .port = DIR0_P_GPIO_Port, .pin = DIR0_P_Pin };

GPIODev gpio_dir0_n = { .port = DIR0_N_GPIO_Port, .pin = DIR0_N_Pin };

GPIODev gpio_dir1_p = { .port = DIR1_P_GPIO_Port, .pin = DIR1_P_Pin };

GPIODev gpio_dir1_n = { .port = DIR1_N_GPIO_Port, .pin = DIR1_N_Pin };

GPIODev gpio_dir2_p = { .port = DIR2_P_GPIO_Port, .pin = DIR2_P_Pin };

GPIODev gpio_dir2_n = { .port = DIR2_N_GPIO_Port, .pin = DIR2_N_Pin };

GPIODev gpio_dir3_p = { .port = DIR3_P_GPIO_Port, .pin = DIR3_P_Pin };

GPIODev gpio_dir3_n = { .port = DIR3_N_GPIO_Port, .pin = DIR3_N_Pin };

GPIODev gpio_led0 = { .port = LED0_GPIO_Port, .pin = LED0_Pin };

GPIODev gpio_led1 = { .port = LED1_GPIO_Port, .pin = LED1_Pin };

GPIODev gpio_nrf_ce = { .port = NRF_CE_GPIO_Port, .pin = NRF_CE_Pin };

GPIODev gpio_nrf_cs = { .port = NRF_CS_GPIO_Port, .pin = NRF_CS_Pin };

GPIODev gpio_key0 = { .port = KEY0_GPIO_Port, .pin = KEY0_Pin };

GPIODev gpio_key1 = { .port = KEY1_GPIO_Port, .pin = KEY1_Pin };

GPIODev gpio_nrf_irq = { .port = NRF_IRQ_GPIO_Port, .pin = NRF_IRQ_Pin };

GPIODev gpio_trigger = { .port = TRIGGER_GPIO_Port, .pin = TRIGGER_Pin };

GPIODev * GPIO_DEVS[] = { &gpio_dir0_p, &gpio_dir0_n, &gpio_dir1_p,
		&gpio_dir1_n, &gpio_dir2_p, &gpio_nrf_ce, &gpio_nrf_cs, &gpio_key0,
		&gpio_key1, &gpio_nrf_irq, &gpio_trigger };

const size_t N_GPIO_DEVS = ARR_LEN(GPIO_DEVS);

// EXTI
EXTIDev exti_key0 = { .gpio = &gpio_key0 };

EXTIDev exti_key1 = { .gpio = &gpio_key1 };

EXTIDev exti_nrf_irq = { .gpio = &gpio_nrf_irq };

EXTIDev * EXTI_DEVS[] = { &exti_key0, &exti_key1, &exti_nrf_irq };

const size_t N_EXTI_DEVS = ARR_LEN(EXTI_DEVS);

// LED
LEDDev led0 = { .gpio = &gpio_led0, .active_high = true };

LEDDev led1 = { .gpio = &gpio_led1, .active_high = true };

LEDDev * LED_DEVS[] = { &led0, &led1 };

const size_t N_LED_DEVS = ARR_LEN(LED_DEVS);

// PWM
PWMDev pwm0 = { .tim = &htim1, .channel = TIM_CHANNEL_1 };

PWMDev pwm1 = { .tim = &htim1, .channel = TIM_CHANNEL_2 };

PWMDev pwm2 = { .tim = &htim1, .channel = TIM_CHANNEL_3 };

PWMDev pwm3 = { .tim = &htim1, .channel = TIM_CHANNEL_4 };

PWMDev * PWM_DEVS[] = { &pwm0, &pwm1, &pwm2, &pwm3 };

const size_t N_PWM_DEVS = ARR_LEN(PWM_DEVS);

// Encoder
EncoderDev encoder0 = { .tim = &htim2 };
EncoderDev encoder1 = { .tim = &htim3 };
EncoderDev encoder2 = { .tim = &htim4 };
EncoderDev encoder3 = { .tim = &htim8 };

EncoderDev * ENCODER_DEVS[] = { &encoder0, &encoder1, &encoder2, &encoder3 };

const size_t N_ENCODERS_DEVS = ARR_LEN(ENCODER_DEVS);

// Input Capture
InputCaptureDev input_capture0 = { .tim = &htim5, .channel = TIM_CHANNEL_1,
		.active_channel = HAL_TIM_ACTIVE_CHANNEL_1 };

InputCaptureDev input_capture1 = { .tim = &htim5, .channel = TIM_CHANNEL_3,
		.active_channel = HAL_TIM_ACTIVE_CHANNEL_3 };

InputCaptureDev input_capture2 = { .tim = &htim5, .channel = TIM_CHANNEL_4,
		.active_channel = HAL_TIM_ACTIVE_CHANNEL_4 };

InputCaptureDev input_capture3 = { .tim = &htim12, .channel = TIM_CHANNEL_1,
		.active_channel = HAL_TIM_ACTIVE_CHANNEL_1 };

InputCaptureDev * INPUT_CAPTURE_DEVS[] = { &input_capture0, &input_capture1,
		&input_capture2, &input_capture3 };

const size_t N_INPUT_CAPTURE_DEVS = ARR_LEN(INPUT_CAPTURE_DEVS);

// UART
UARTDev uart1 = { .uart = &huart1 };

UARTDev uart2 = { .uart = &huart2 };

UARTDev * UART_DEVS[] = { &uart1, &uart2 };

const size_t N_UART_DEVS = ARR_LEN(UART_DEVS);

// SPI
SPIDev nrf_spi = { .spi = &hspi1, .cs = &gpio_nrf_cs };

SPIDev * SPI_DEVS[] = { &nrf_spi };

const size_t N_SPI_DEVS = ARR_LEN(SPI_DEVS);

// Wheel
WheelDev wheel0 = { .pwm = &pwm0, .encoder = &encoder0, .dire_p = &gpio_dir0_p,
		.dire_n = &gpio_dir0_n, .circumference = M_PI * 0.115,
		.pulse_per_round = 1216.0};

WheelDev wheel1 = { .pwm = &pwm1, .encoder = &encoder1, .dire_p = &gpio_dir1_p,
		.dire_n = &gpio_dir1_n, .circumference = M_PI * 0.115,
		.pulse_per_round = 1216.0};

WheelDev wheel2 = { .pwm = &pwm2, .encoder = &encoder2, .dire_p = &gpio_dir2_p,
		.dire_n = &gpio_dir2_n, .circumference = M_PI * 0.115,
		.pulse_per_round = 1216.0};

WheelDev wheel3 = { .pwm = &pwm3, .encoder = &encoder3, .dire_p = &gpio_dir3_p,
		.dire_n = &gpio_dir3_n, .circumference = M_PI * 0.115,
		.pulse_per_round = 1216.0};

WheelDev * WHEEL_DEVS[] = { &wheel0, &wheel1, &wheel2, &wheel3 };

const size_t N_WHEEL_DEVS = ARR_LEN(WHEEL_DEVS);

// NRF
NRFDev nrf = { .spi = &nrf_spi, .irq = &exti_nrf_irq, .ce = &gpio_nrf_ce };

NRFDev * NRF_DEVS[] = { &nrf };

const size_t N_NRF_DEVS = ARR_LEN(NRF_DEVS);

void boardInit(void) {
	gpioInit();
	extiInit();
	ledInit();
	pwmInit();
	encoderInit();
//	inputCaptureInit();
	uartInit();
//	spiInit();
	wheelInit();
//	nrfInit();
}
