#include "params.h"
#include "osal.h"
#include "msg_io.h"

extern const Param param_wheel0_pid_p;
extern const Param param_wheel0_pid_i;
extern const Param param_wheel0_pid_d;
extern const Param param_wheel0_pid_limit;

extern const Param param_wheel1_pid_p;
extern const Param param_wheel1_pid_i;
extern const Param param_wheel1_pid_d;
extern const Param param_wheel1_pid_limit;

extern const Param param_wheel2_pid_p;
extern const Param param_wheel2_pid_i;
extern const Param param_wheel2_pid_d;
extern const Param param_wheel2_pid_limit;

extern const Param param_wheel3_pid_p;
extern const Param param_wheel3_pid_i;
extern const Param param_wheel3_pid_d;
extern const Param param_wheel3_pid_limit;

const Param * const PARAMS[] =
{ 		&param_wheel0_pid_p, &param_wheel0_pid_i, &param_wheel0_pid_d, &param_wheel0_pid_limit,
		&param_wheel1_pid_p, &param_wheel1_pid_i, &param_wheel1_pid_d, &param_wheel1_pid_limit,
		&param_wheel2_pid_p, &param_wheel2_pid_i, &param_wheel2_pid_d, &param_wheel2_pid_limit,
		&param_wheel3_pid_p, &param_wheel3_pid_i, &param_wheel3_pid_d, &param_wheel3_pid_limit,
};

const size_t N_PARAMS = sizeof(PARAMS) / sizeof(PARAMS[0]);

void paramsThread(void const * arg);

osThreadDef(params_thread, paramsThread, osPriorityIdle, 0, 64);
osThreadId params_thread;

void paramSetCallback(void * payload)
{
	static MsgParamInfo param_info;
	MsgParam * param = payload;
	int id;
	id = param->id;

	if (param->action == MSG_PARAM_ACTION_GET)
	{
		if (id < N_PARAMS)
		{
			param_info.id = id;
			param_info.type = paramGetType(PARAMS[id]);
			if (PARAMS[id]->type == PARAM_INT)
			{
				param_info.i = paramGetInt(PARAMS[id]);
			}
			else if (PARAMS[id]->type == PARAM_FLOAT)
			{
				param_info.f = paramGetFloat(PARAMS[id]);
			}
			strcpy((char *) param_info.name, paramGetName(PARAMS[id]));

			msgSend(MSG_TOPIC_PARAM_INFO, &param_info,
					sizeof(param_info) - sizeof(param_info.name)
							+ strlen((char *) param_info.name) + 1);
		}
	}
	else if (param->action == MSG_PARAM_ACTION_SET)
	{
		if (id < N_PARAMS)
		{
			if (PARAMS[id]->type == PARAM_INT)
			{
				paramSetInt(PARAMS[id], param->i);
			}
			else if (PARAMS[id]->type == PARAM_FLOAT)
			{
				paramSetFloat(PARAMS[id], param->f);
			}

			param_info.id = id;
			param_info.type = paramGetType(PARAMS[id]);
			param_info.i = paramGetInt(PARAMS[id]);
			param_info.f = paramGetFloat(PARAMS[id]);
			strcpy((char *) param_info.name, paramGetName(PARAMS[id]));

			msgSend(MSG_TOPIC_PARAM_INFO, &param_info,
					sizeof(param_info) - sizeof(param_info.name)
							+ strlen((char *) param_info.name) + 1);
		}
	}
	else if (param->action == MSG_PARAM_ACTION_PULL)
	{
		osThreadResume(params_thread);
	}
}

void paramsInit(void)
{
	int i;
	for (i = 0; i < N_PARAMS; i++)
	{
		paramResetValue(PARAMS[i]);
	}

	params_thread = osThreadCreate(osThread(params_thread), NULL);
	msgSetCallback(MSG_TOPIC_PARAM, paramSetCallback);
}

void paramsThread(void const * arg)
{
	static MsgParamInfo param_info;
	int i;

	osThreadSetPriority(osThreadGetId(), osPriorityNormal);

	while (true)
	{
		for (i = 0; i < N_PARAMS; i++)
		{
			param_info.id = i;
			param_info.type = paramGetType(PARAMS[i]);
			param_info.i = paramGetInt(PARAMS[i]);
			param_info.f = paramGetFloat(PARAMS[i]);
			strcpy((char *) param_info.name, paramGetName(PARAMS[i]));
			msgSend(MSG_TOPIC_PARAM_INFO, &param_info,
					sizeof(param_info) - sizeof(param_info.name)
							+ strlen((char *) param_info.name) + 1);
			osDelay(1);
		}
		osThreadSuspend(osThreadGetId());
	}
}
