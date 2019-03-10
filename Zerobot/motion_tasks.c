#include "motion_tasks.h"
#include "board.h"
#include "osal.h"
#include "msg_io.h"
#include "wheel.h"
#include "pid.h"
#include "params.h"

#define N_WHEELS 4

void wheelsThread(void const * arg);

osThreadDef(wheels_thread, wheelsThread, osPriorityIdle, 0, 512);
osThreadId wheels_thread;

PID wheels_pid[N_WHEELS] =
{ 0 };

const Param param_wheel0_pid_p =
{ .type = PARAM_FLOAT, .name = "wheel0_pid_p", .ptr = &wheels_pid[0].Kp,
		.default_value =
		{ .f = 0.2 } };

const Param param_wheel0_pid_i =
{ .type = PARAM_FLOAT, .name = "wheel0_pid_i", .ptr = &wheels_pid[0].Ki,
		.default_value =
		{ .f = 0.1 } };

const Param param_wheel0_pid_d =
{ .type = PARAM_FLOAT, .name = "wheel0_pid_d", .ptr = &wheels_pid[0].Kd,
		.default_value =
		{ .f = 0.0 } };

const Param param_wheel0_pid_limit =
{ .type = PARAM_FLOAT, .name = "wheel0_pid_limit", .ptr = &wheels_pid[0].limit,
		.default_value =
		{ .f = 1.0 } };

const Param param_wheel1_pid_p =
{ .type = PARAM_FLOAT, .name = "wheel1_pid_p", .ptr = &wheels_pid[1].Kp,
		.default_value =
		{ .f = 0.2 } };
const Param param_wheel1_pid_i =
{ .type = PARAM_FLOAT, .name = "wheel1_pid_i", .ptr = &wheels_pid[1].Ki,
		.default_value =
		{ .f = 0.1 } };
const Param param_wheel1_pid_d =
{ .type = PARAM_FLOAT, .name = "wheel1_pid_d", .ptr = &wheels_pid[1].Kd,
		.default_value =
		{ .f = 0.0 } };
const Param param_wheel1_pid_limit =
{ .type = PARAM_FLOAT, .name = "wheel1_pid_limit", .ptr = &wheels_pid[1].limit,
		.default_value =
		{ .f = 1.0 } };

const Param param_wheel2_pid_p =
{ .type = PARAM_FLOAT, .name = "wheel2_pid_p", .ptr = &wheels_pid[2].Kp,
		.default_value =
		{ .f = 0.2 } };
const Param param_wheel2_pid_i =
{ .type = PARAM_FLOAT, .name = "wheel2_pid_i", .ptr = &wheels_pid[2].Ki,
		.default_value =
		{ .f = 0.1 } };
const Param param_wheel2_pid_d =
{ .type = PARAM_FLOAT, .name = "wheel2_pid_d", .ptr = &wheels_pid[2].Kd,
		.default_value =
		{ .f = 0.0 } };
const Param param_wheel2_pid_limit =
{ .type = PARAM_FLOAT, .name = "wheel2_pid_limit", .ptr = &wheels_pid[2].limit,
		.default_value =
		{ .f = 1.0 } };

const Param param_wheel3_pid_p =
{ .type = PARAM_FLOAT, .name = "wheel3_pid_p", .ptr = &wheels_pid[3].Kp,
		.default_value =
		{ .f = 0.2 } };
const Param param_wheel3_pid_i =
{ .type = PARAM_FLOAT, .name = "wheel3_pid_i", .ptr = &wheels_pid[3].Ki,
		.default_value =
		{ .f = 0.1 } };
const Param param_wheel3_pid_d =
{ .type = PARAM_FLOAT, .name = "wheel3_pid_d", .ptr = &wheels_pid[3].Kd,
		.default_value =
		{ .f = 0.0 } };
const Param param_wheel3_pid_limit =
{ .type = PARAM_FLOAT, .name = "wheel3_pid_limit", .ptr = &wheels_pid[3].limit,
		.default_value =
		{ .f = 1.0 } };

void msgWheelCallback(void *payload)
{
	MsgWheel * wheel = (MsgWheel*) payload;
	int i;
	for (i = 0; i < N_WHEELS; i++)
	{
		pidSetPoint(&wheels_pid[i], wheel->data[i]);
	}
}

void msgTwistCallback(void *payload)
{
	const static float ab = 0.5f;
	static float speed[4] =
	{ 0 };

	MsgTwist * twist = (MsgTwist*) payload;

	speed[0] = twist->linear_x - twist->linear_y - twist->angular_z * ab;
	speed[1] = twist->linear_x + twist->linear_y + twist->angular_z * ab;
	speed[2] = twist->linear_x - twist->linear_y + twist->angular_z * ab;
	speed[3] = twist->linear_x + twist->linear_y - twist->angular_z * ab;

	for (int i = 0; i < N_WHEELS; i++)
	{
		pidSetPoint(&wheels_pid[i], speed[i]);
	}
}

void motionTasksInit(void)
{
	msgSetCallback(MSG_TOPIC_WHEEL, msgWheelCallback);
	msgSetCallback(MSG_TOPIC_TWIST, msgTwistCallback);

	wheels_thread = osThreadCreate(osThread(wheels_thread), NULL);
}

void wheelsThread(void const * arg)
{
	static MsgEncoder encoder =
	{ };

	int i = 0;
	uint32_t last_tick = 0, tick = 0;
	float delta_time;

	osThreadSetPriority(osThreadGetId(), osPriorityNormal);

//	Testing.
//	osDelay(100);
//	while(true)
//	{
//		for(int i = 0; i < N_WHEELS; i++)
//		{
//			wheelSetOutput(WHEEL_DEVS[i], 0.5);
//			osDelay(1000);
//
//			wheelSetOutput(WHEEL_DEVS[i], -0.5);
//			osDelay(1000);
//
//			wheelSetOutput(WHEEL_DEVS[i], 0);
//			osDelay(1000);
//		}
//	}

	while (true)
	{
		osDelay(25);
		tick = osKernelSysTick();
		delta_time = ((float) (tick - last_tick)) / osKernelSysTickFrequency;
		last_tick = tick;

		// Wheel control.
		for (i = 0; i < N_WHEELS; i++)
		{
			pidSetInput(&wheels_pid[i],
					wheelGetSpeed(WHEEL_DEVS[i], delta_time));

			pidUpdate(&wheels_pid[i]);

			wheelSetOutput(WHEEL_DEVS[i], pidGetOutput(&wheels_pid[i]));

			// Suspend motor.
			if (pidGetPoint(&wheels_pid[i]) == 0.0
					&& pidGetInput(&wheels_pid[i]) == 0.0)
			{
				pidSetOutput(&wheels_pid[i], 0.0);
			}
		}

		// Speed feedback.
		encoder.duration = delta_time;
		for (i = 0; i < N_WHEELS; i++)
		{
			encoder.data[i] = pidGetInput(&wheels_pid[i]);
		}
		msgSend(MSG_TOPIC_ENCODER, &encoder, sizeof(encoder));
	}
}
