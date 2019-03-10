#ifndef ZEROBOT_PARAMS_H_
#define ZEROBOT_PARAMS_H_

#include "stddef.h"
#include "stdint.h"

enum ParamType
{
	PARAM_INT = 0, PARAM_FLOAT
};

typedef union
{
	int i;
	float f;
} ParamValue;

typedef struct
{
	enum ParamType type;
	const char * name;
	void * ptr;
	ParamValue default_value;
} Param;

extern const Param * const PARAMS[];
extern const size_t N_PARAMS;

void paramsInit(void);

static inline enum ParamType paramGetType(const Param * param)
{
	return param->type;
}

static inline const char * paramGetName(const Param * param)
{
	return param->name;
}

static inline void paramResetValue(const Param * param)
{
	*(ParamValue *)param->ptr = param->default_value;
}

static inline void paramSetValue(const Param * param, ParamValue value)
{
	*(ParamValue *)param->ptr = value;
}

static inline void paramSetInt(const Param * param, int value)
{
	*(int32_t *) param->ptr = value;
}

static inline void paramSetFloat(const Param * param, float value)
{
	*(float *) param->ptr = value;
}

static inline uint32_t paramGetUInt(const Param * param)
{
	return *(uint32_t *) param->ptr;
}

static inline int32_t paramGetInt(const Param * param)
{
	return *(int32_t *) param->ptr;
}

static inline float paramGetFloat(const Param * param)
{
	return *(float *) param->ptr;
}

#endif /* ZEROBOT_PARAMS_H_ */
