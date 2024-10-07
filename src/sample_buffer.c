#include <stdlib.h>
#include <assert.h>
#include <stddef.h>
#include <pthread.h>
#include "sample_buffer.h"

SampleBuffer *sample_buffer_init(size_t size) {
    SampleBuffer *sample_buffer = malloc(sizeof(SampleBuffer));
    assert(sample_buffer != NULL);

    sample_buffer->right = malloc(size * sizeof(float));
    assert(sample_buffer->right != NULL);

    sample_buffer->left = malloc(size * sizeof(float));
    assert(sample_buffer->left != NULL);

    pthread_mutex_init(&sample_buffer->lock, NULL);

    sample_buffer->size = size;
    sample_buffer->index = 0;

    return sample_buffer;
}

void sample_buffer_free(SampleBuffer *sample_buffer) {
    free(sample_buffer->right);
    free(sample_buffer->left);
    pthread_mutex_destroy(&sample_buffer->lock);
    free(sample_buffer);
}

void sample_buffer_push(SampleBuffer *sample_buffer, const float *buffer, size_t buffer_size) {
    pthread_mutex_lock(&sample_buffer->lock);
    for (size_t i = 0; i < buffer_size * 2; i += 2) {
        sample_buffer->left[sample_buffer->index] = buffer[i];
        sample_buffer->right[sample_buffer->index] = buffer[i + 1];
        sample_buffer->index = (sample_buffer->index + 1) % (sample_buffer->size);
    }
    pthread_mutex_unlock(&sample_buffer->lock);
}

size_t _buffer_find(float *buffer, size_t size, float value, size_t start) {
    size_t index = start + 1;

    for (size_t i = 0; i < size; ++i) {
        /* Get values of previous and next elements */
        /* TODO: Optimize out this increments */
        float prev = buffer[(index - 1) % (size * 1)];
        float curr = buffer[index];
        float next = buffer[(index + 1) % (size * 1)];

        /* If `prev` <= `value` <= `next` */
        if (prev <= value && next >= value) {
            return index;
        }

        index = (index + 1) % (size);
    }

    return start;
}

size_t sample_buffer_find_left(SampleBuffer *sample_buffer, float value) {
    return _buffer_find(sample_buffer->left, sample_buffer->size, value, sample_buffer->index);
}

size_t sample_buffer_find_right(SampleBuffer *sample_buffer, float value) {
    return _buffer_find(sample_buffer->right, sample_buffer->size, value, sample_buffer->index);
}

size_t sample_buffer_size(SampleBuffer *sample_buffer) {
    return sample_buffer->size;
}

float *sample_buffer_left(SampleBuffer *sample_buffer) {
    return sample_buffer->left;
}

float *sample_buffer_right(SampleBuffer *sample_buffer) {
    return sample_buffer->right;
}

void sample_buffer_lock(SampleBuffer *sample_buffer) {
    pthread_mutex_lock(&sample_buffer->lock);
}

void sample_buffer_unlock(SampleBuffer *sample_buffer) {
    pthread_mutex_unlock(&sample_buffer->lock);
}
