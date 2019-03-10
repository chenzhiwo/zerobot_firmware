#include "ranging_tasks.h"
#include "input_capture.h"
#include "osal.h"
#include "msg_io.h"
#include "gpio.h"

extern GPIODev gpio_trigger;

void rangingThread(void const * arg);

osThreadDef(ranging_thread, rangingThread, osPriorityIdle, 0, 128);
osThreadId ranging_thread;

void rangingTasksInit(void)
{
	ranging_thread = osThreadCreate(osThread(ranging_thread), NULL);
}

void rangingThread(void const * arg)
{
	static MsgRange range;
	int i = 0;

	osThreadSetPriority(osThreadGetId(), osPriorityNormal);

	while (true)
	{
		// Ranging trigger.
		gpioWrite(&gpio_trigger, true);
		osDelay(1);
		gpioWrite(&gpio_trigger, false);

		osDelay(199);
		range.tick = osKernelSysTick();

		for (i = 0; i < N_INPUT_CAPTURE_DEVS; i++)
		{
			range.data[i] = inputCaptureGetValue(INPUT_CAPTURE_DEVS[i]);
		}

		msgSend(MSG_TOPIC_RANGE, &range, sizeof(range));
	}
}
