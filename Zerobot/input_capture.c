#include "input_capture.h"

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	size_t i = 0;
	uint32_t channel = 0, value = 0;

	for (i = 0; i < N_INPUT_CAPTURE_DEVS; i++)
	{
		// Find out active channel.
		if ((INPUT_CAPTURE_DEVS[i]->tim == htim)
				&& (INPUT_CAPTURE_DEVS[i]->active_channel == htim->Channel))
		{
			channel = INPUT_CAPTURE_DEVS[i]->channel;
			if (INPUT_CAPTURE_DEVS[i]->start == 0)
			{
				// Capture started.
				INPUT_CAPTURE_DEVS[i]->start = HAL_TIM_ReadCapturedValue(htim,
						channel);
				// Ready to capture falling edge.
				TIM_RESET_CAPTUREPOLARITY(htim, channel);
				TIM_SET_CAPTUREPOLARITY(htim, channel, TIM_ICPOLARITY_FALLING);
			}
			else
			{
				// Capture finished.
				value = HAL_TIM_ReadCapturedValue(htim, channel);
				if (value < INPUT_CAPTURE_DEVS[i]->start)
				{
					INPUT_CAPTURE_DEVS[i]->value = htim->Init.Period
							- (INPUT_CAPTURE_DEVS[i]->start - value);
				}
				else
				{
					INPUT_CAPTURE_DEVS[i]->value = value
							- INPUT_CAPTURE_DEVS[i]->start;
				}
				INPUT_CAPTURE_DEVS[i]->start = 0;
				// Prepare for next capture.
				TIM_RESET_CAPTUREPOLARITY(htim, channel);
				TIM_SET_CAPTUREPOLARITY(htim, channel, TIM_ICPOLARITY_RISING);
			}
			return;
		}
	}
}
