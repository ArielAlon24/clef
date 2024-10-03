#include "ring_buffer.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

RingBuffer *ring_buffer_init(size_t size) {
    RingBuffer *ring_buffer = malloc(sizeof(RingBuffer));
    assert(ring_buffer != NULL);
    float *buffer = malloc(size * sizeof(float));
    assert(buffer != NULL);

    ring_buffer->size = size;
    ring_buffer->index = 0;
    ring_buffer->buffer = buffer;

    return ring_buffer;
}

void ring_buffer_free(RingBuffer *ring_buffer) {
    free(ring_buffer->buffer);
    free(ring_buffer);
}

/* TODO: Test if memcpy improves the performance */
void ring_buffer_insert(RingBuffer *ring_buffer, const float *buffer, size_t buffer_size) {
    for (int i = 0; i < buffer_size; ++i) {
        ring_buffer->buffer[ring_buffer->index] = buffer[i];
        ring_buffer->index = (ring_buffer->index + 1) % ring_buffer->size;
    }
}

void ring_buffer_print(RingBuffer *ring_buffer) {
    printf("[");
    for (int i = 0; i < ring_buffer->size; ++i) {
        printf("%.1f", ring_buffer->buffer[i]);
        if (i != ring_buffer->size - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}
