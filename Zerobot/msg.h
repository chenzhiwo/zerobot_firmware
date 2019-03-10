// Copyright 2018 chengzhi-chen
#ifndef ZEROBOT_DRIVER_MSG_H
#define ZEROBOT_DRIVER_MSG_H

#pragma pack(push)
#pragma pack(1)

#ifdef __cplusplus
#include <cstddef>
#include <cstdint>
#include <cstring>
#else  // __cplusplus
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#endif  // __cplusplus

#if defined(__GNUC__)
#define MSG_PACKED __attribute__((aligned(1)))
#elif defined(__CC_ARM)
#define MSG_PACKED __packed
#elif defined(_MSC_VER)
#define MSG_PACKED
#endif

typedef struct MSG_PACKED
{
  uint16_t id;
  uint16_t type;
  int32_t i;
  float f;
  uint8_t name[32];
}
MsgParamInfo;

typedef struct MSG_PACKED
{
  uint32_t tick;
  int32_t data[4];
}
MsgRange;

typedef struct MSG_PACKED
{
  float duration;
  float data[4];
}
MsgEncoder;

typedef struct MSG_PACKED
{
  float linear_x;
  float linear_y;
  float angular_z;
}
MsgTwist;

typedef struct MSG_PACKED
{
  uint8_t level;
  uint8_t info[32];
}
MsgLog;

typedef struct MSG_PACKED
{
  uint32_t tick;
  float position_x;
  float position_y;
  float orientation_z;
  float linear_x;
  float linear_y;
  float angular_z;
}
MsgOdometry;

typedef struct MSG_PACKED
{
  float data[4];
}
MsgWheel;

typedef struct MSG_PACKED
{
  uint16_t action;
  uint16_t id;
  int32_t i;
  float f;
}
MsgParam;

typedef struct MSG_PACKED
{
  int32_t adc[4];
}
MsgADC;

typedef struct MSG_PACKED
{
  float tempture;
  float humidity;
}
MsgAir;

typedef struct MSG_PACKED
{
}
MsgTopic;

typedef union MSG_PACKED
{
  MsgParamInfo param_info;
  MsgRange range;
  MsgEncoder encoder;
  MsgTwist twist;
  MsgLog log;
  MsgOdometry odometry;
  MsgWheel wheel;
  MsgParam param;
  MsgTopic topic;
}
MsgBuffer;

static const uint8_t MSG_PARAM_INFO_NAME_COUNT = 32;
static const uint8_t MSG_RANGE_DATA_COUNT = 4;
static const uint8_t MSG_ENCODER_DATA_COUNT = 4;
static const uint8_t MSG_LOG_DEBUG = 0;
static const uint8_t MSG_LOG_INFO = 1;
static const uint8_t MSG_LOG_WARN = 2;
static const uint8_t MSG_LOG_ERROR = 3;
static const uint8_t MSG_LOG_FATAL = 4;
static const uint8_t MSG_LOG_INFO_COUNT = 32;
static const uint8_t MSG_WHEEL_DATA_COUNT = 4;
static const uint16_t MSG_PARAM_ACTION_GET = 0;
static const uint16_t MSG_PARAM_ACTION_SET = 1;
static const uint16_t MSG_PARAM_ACTION_PULL = 2;
static const uint8_t MSG_TOPIC_LOG = 0;
static const uint8_t MSG_TOPIC_PARAM = 1;
static const uint8_t MSG_TOPIC_PARAM_INFO = 2;
static const uint8_t MSG_TOPIC_WHEEL = 3;
static const uint8_t MSG_TOPIC_ENCODER = 4;
static const uint8_t MSG_TOPIC_RANGE = 5;
static const uint8_t MSG_TOPIC_CMD_VEL = 6;
static const uint8_t MSG_TOPIC_ODOM = 7;
static const uint8_t MSG_TOPIC_TWIST = 8;
static const uint8_t MSG_TOPIC_ADC = 9;
static const uint8_t MSG_TOPIC_AIR = 10;
static const uint8_t MSG_TOPIC_COUNT = 11;

#pragma pack(pop)

#endif  // ZEROBOT_DRIVER_MSG_H
