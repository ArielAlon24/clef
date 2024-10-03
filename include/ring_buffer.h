#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stddef.h>

typedef struct {
    float *buffer;
    size_t size;
    size_t index;
} RingBuffer;

RingBuffer *ring_buffer_init(size_t size);

void ring_buffer_free(RingBuffer *ring_buffer);

void ring_buffer_insert(RingBuffer *ring_buffer, const float *buffer, size_t buffer_size);

void ring_buffer_print(RingBuffer *ring_buffer);

#endif
