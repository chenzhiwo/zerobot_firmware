#include "app.h"
#include "board.h"
#include "osal.h"
#include "led.h"
#include "params.h"
#include "msg_io.h"
#include "adc_tasks.h"
#include "motion_tasks.h"
#include "ranging_tasks.h"

#define HTU_TIMEOUT 500
#define HTU_ADDR (0x40 << 1)
#define HTU_TRIG_TEMP_HOLD 0xe3
#define HTU_TRIG_HUMI_HOLD 0xe5
#define HTU_TRIG_TEMP_NO_HOLD 0xf3
#define HTU_TRIG_HUMI_NO_HOLD 0xf5
#define HTU_SOFT_RESET 0xfe

extern I2C_HandleTypeDef hi2c2;

void appInit(void)
{
	boardInit();
	msgIOInit();
	paramsInit();
//	adcTasksInit();
	motionTasksInit();
//	rangingTasksInit();
}

void loopTaskEntry(void const * arg)
{
//	HAL_StatusTypeDef status;
//	uint8_t buffer[4];
//	uint16_t raw;
//	static MsgAir air;
//
//	osThreadSetPriority(osThreadGetId(), osPriorityLow);
//
//	osDelay(500);
//	status = HAL_I2C_Mem_Write(&hi2c2, HTU_ADDR, HTU_SOFT_RESET,
//	I2C_MEMADD_SIZE_8BIT, NULL, 0, HTU_TIMEOUT);
//
//	while (true) {
//		osDelay(500);
//
//		status = HAL_I2C_Mem_Read(&hi2c2, HTU_ADDR, HTU_TRIG_TEMP_HOLD,
//		I2C_MEMADD_SIZE_8BIT, buffer, 3, HTU_TIMEOUT);
//		if (status != HAL_OK) {
//			continue;
//		}
//
//		raw = ((uint16_t) buffer[0] << 8) + buffer[1];
//		raw &= ~(uint16_t) 0b11;
//		air.tempture = (raw * 175.72 / 65536.0) - 46.85;
//
//		osDelay(500);
//
//		status = HAL_I2C_Mem_Read(&hi2c2, HTU_ADDR, HTU_TRIG_HUMI_HOLD,
//		I2C_MEMADD_SIZE_8BIT, buffer, 3, HTU_TIMEOUT);
//
//		if (status != HAL_OK) {
//			continue;
//		}
//
//		raw = ((uint16_t) buffer[0] << 8) + buffer[1];
//		raw &= ~(uint16_t) 0b1111;
//		air.humidity = (raw * 125.0 / 65536.0) - 6.0;
//
//		msgSend(MSG_TOPIC_AIR, &air, sizeof(air));
//
//		ledToggle(LED_DEVS[0]);
//	}

	osThreadSetPriority(osThreadGetId(), osPriorityLow);

	for (;;)
	{
		osDelay(500);
		ledToggle(LED_DEVS[0]);
	}
}
