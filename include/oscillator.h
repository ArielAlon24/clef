#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#include <stddef.h>

typedef enum {
    OSCILLATOR_SINE,
    OSCILLATOR_SQUARE,
    OSCILLATOR_TRIANGLE,
    OSCILLATOR_SAWTOOTH,
} OscillatorType;

typedef struct {
    OscillatorType type;
    float frequency;
    float amplitude; /* Value between 0 and 1 where 0 is no sound and 1 is full sound */
    float phase;
} Oscillator;

Oscillator *oscillator_init(OscillatorType type, float frequency, float amplitude);

/* The next function will _add_ each oscillator sample to it's place in the buffer,
it assumes the given buffer has valid values and is not uninitialized. */
void oscillator_next(Oscillator *oscillator, float *buffer, size_t frame_count);

/* This are private implementations of each oscillator type, they are used in `oscillator_next` */
void _oscillator_sine_next(Oscillator *oscillator, float *buffer, size_t frame_count);
void _oscillator_square_next(Oscillator *oscillator, float *buffer, size_t frame_count);
void _oscillator_triangle_next(Oscillator *oscillator, float *buffer, size_t frame_count);
void _oscillator_sawtooth_next(Oscillator *oscillator, float *buffer, size_t frame_count);

void oscillator_free(Oscillator *oscillator);

#endif
