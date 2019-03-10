#ifndef ZEROLINK_RING_BUFFER_H
#define ZEROLINK_RING_BUFFER_H

#ifdef __cplusplus
#include <cstddef>
#include <cstdint>
#include <cstring>
namespace zerolink
{
#else // __cplusplus
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#endif // __cplusplus

typedef struct
{
    uint8_t * data;
    size_t capacity;
    size_t reserved;
    volatile size_t head_index;
    volatile size_t tail_index;
} RingBuffer;

static inline void ringBufferInit(RingBuffer * buffer, uint8_t * data,
                                  size_t total, size_t reserved)
{
    buffer->data = data;
    buffer->capacity = total - reserved;
    buffer->reserved = reserved;
    buffer->head_index = 0;
    buffer->tail_index = 0;
}

static inline uint8_t * ringBufferData(RingBuffer * buffer)
{
    return buffer->data;
}

static inline void ringBufferResetIndex(RingBuffer * buffer)
{
    buffer->head_index = 0;
    buffer->tail_index = 0;
}

static inline size_t ringBufferCapacity(RingBuffer * buffer)
{
    return buffer->capacity;
}

static inline size_t ringBufferSize(RingBuffer * buffer)
{
    return (buffer->tail_index >= buffer->head_index) ?
           (buffer->tail_index - buffer->head_index) :
           (buffer->capacity - (buffer->head_index - buffer->tail_index));
}

static inline uint8_t * ringBufferRead(RingBuffer * buffer, size_t len)
{
    if ((buffer->head_index + len) > buffer->capacity)
    {
        memcpy(buffer->data + buffer->capacity, buffer->data,
               (buffer->head_index + len) - buffer->capacity);
    }
    return buffer->data + buffer->head_index;
}

static inline uint8_t * ringBufferWrite(RingBuffer * buffer)
{
    return buffer->data + buffer->tail_index;
}

static inline void ringBufferPop(RingBuffer * buffer, size_t len)
{
    buffer->head_index = (buffer->head_index + len) % buffer->capacity;
}

static inline void ringBufferPush(RingBuffer * buffer, size_t len)
{
    if ((buffer->tail_index + len) > buffer->capacity)
    {
        memcpy(buffer->data, buffer->data + buffer->capacity,
               (buffer->tail_index + len) - buffer->capacity);
    }
    buffer->tail_index = (buffer->tail_index + len) % buffer->capacity;
}

static inline void ringBufferSetHeadIndex(RingBuffer * buffer, size_t index)
{
    buffer->head_index = index;
}

static inline void ringBufferSetTailIndex(RingBuffer * buffer, size_t index)
{
    buffer->tail_index = index;
}

#ifdef __cplusplus
} // namespace zerolink
#endif // __cplusplus

#endif //ZEROLINK_RING_BUFFER_H
