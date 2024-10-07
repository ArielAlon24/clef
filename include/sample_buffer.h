#ifndef SAMPLE_BUFFER_H
#define SAMPLE_BUFFER_H

#include <pthread.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct {
    float *right;
    float *left;
    size_t size;
    size_t index;

    pthread_mutex_t lock;
} SampleBuffer;

/* The argument `size` is unique to each channel, so for `x` samples for the left channel
and `x` samples for the right channels, size should be set to `x`. */
SampleBuffer *sample_buffer_init(size_t size);

void sample_buffer_free(SampleBuffer *sample_buffer);

void sample_buffer_push(SampleBuffer *sample_buffer, const float *buffer, size_t buffer_size);

size_t sample_buffer_size(SampleBuffer *sample_buffer);

void sample_buffer_lock(SampleBuffer *sample_buffer);
void sample_buffer_unlock(SampleBuffer *sample_buffer);

/* `sample_buffer_[left | right] must be used with the `sample_buffer_[lock | unlock]` functions */
float *sample_buffer_left(SampleBuffer *sample_buffer);
float *sample_buffer_right(SampleBuffer *sample_buffer);

size_t _buffer_find(float *buffer, size_t size, float value, size_t start);

/* sample_buffer_find[left | right] must be used with the `sample_buffer_[lock | unlock]` functions */
size_t sample_buffer_find_left(SampleBuffer *sample_buffer, float value);
size_t sample_buffer_find_right(SampleBuffer *sample_buffer, float value);


#endif
