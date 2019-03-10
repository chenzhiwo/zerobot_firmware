#ifndef ZEROBOT_MSG_IO_H_
#define ZEROBOT_MSG_IO_H_

#include "msg.h"

typedef uint8_t Channel;

typedef void (*MsgCallback)(void *);

void msgIOInit(void);

void msgSetCallback(Channel channel, MsgCallback callback);

void msgSend(Channel channel, void * payload, size_t len);

#endif /* ZEROBOT_MSG_IO_H_ */
