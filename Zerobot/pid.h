#ifndef ZEROBOT_PID_H_
#define ZEROBOT_PID_H_

typedef struct PID
{
	float setpoint;
	float input;
	float output;
	float Kp;
	float Ki;
	float Kd;
	float errors[3];
	float limit;
} PID;

static inline void pidSetPoint(PID * pid, float setpoint)
{
	pid->setpoint = setpoint;
}

static inline void pidSetInput(PID * pid, float input)
{
	pid->input = input;
}

static inline void pidSetOutput(PID * pid, float output)
{
	pid->output = output;
}

static inline void pidSetLimit(PID * pid, float limit)
{
	pid->limit = limit;
}

static inline float pidGetPoint(PID * pid)
{
	return pid->setpoint;
}

static inline float pidGetInput(PID * pid)
{
	return pid->input;
}

static inline float pidGetOutput(PID * pid)
{
	return pid->output;
}

static inline void pidSetPID(PID * pid, float Kp, float Ki, float Kd)
{
	pid->Kp = Kp;
	pid->Ki = Ki;
	pid->Kd = Kd;
}

static inline void pidUpdate(PID * pid)
{
	pid->errors[2] = pid->errors[1];
	pid->errors[1] = pid->errors[0];
	pid->errors[0] = pid->setpoint - pid->input;
	float p = pid->Kp * (pid->errors[0] - pid->errors[1]);
	float i = pid->Ki * pid->errors[0];
	float d = pid->Kd
			* (pid->errors[0] - (2 * pid->errors[1]) + pid->errors[2]);
	pid->output += p + i + d;

	pid->output = pid->output < -pid->limit ? -pid->limit : pid->output;
	pid->output = pid->limit < pid->output ? pid->limit : pid->output;
}

#endif /* ZEROBOT_PID_H_ */
