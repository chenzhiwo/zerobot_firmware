#include "adc_tasks.h"
#include "board.h"
#include "osal.h"
#include "msg_io.h"

#define N_ADC_CHANNELS (4)

void adcThread(void const * arg);

extern ADC_HandleTypeDef hadc1;
uint16_t adc_values[N_ADC_CHANNELS];

osThreadDef(adc_thread, adcThread, osPriorityNormal, 0, 512);
osThreadId adc_thread;

void adcTasksInit(void)
{
	adc_thread = osThreadCreate(osThread(adc_thread), NULL);
}

void adcThread(void const * arg)
{
	static MsgADC adc;

	while (true)
	{
		HAL_ADC_Start_DMA(&hadc1, (uint32_t *) adc_values, N_ADC_CHANNELS);

		osDelay(200);

		for (int i = 0; i < N_ADC_CHANNELS; i++)
		{
			adc.adc[i] = adc_values[i];
		}
		msgSend(MSG_TOPIC_ADC, &adc, sizeof(adc));
	}
}
