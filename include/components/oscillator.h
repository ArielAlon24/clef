#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#include <stddef.h>
#include "rack/component.h"

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

Component *oscillator_init(OscillatorType type, float frequency, float amplitude);

/* This is the ComponentAudioCallback implementation */
void oscillator_next(void *state, float *buffer, unsigned int frame_count);

/* This are private implementations of each oscillator type, they are used in `oscillator_next` */
void _oscillator_sine_next(Oscillator *oscillator, float *buffer, unsigned int frame_count);
void _oscillator_square_next(Oscillator *oscillator, float *buffer, unsigned int frame_count);
void _oscillator_triangle_next(Oscillator *oscillator, float *buffer, unsigned int frame_count);
void _oscillator_sawtooth_next(Oscillator *oscillator, float *buffer, unsigned int frame_count);

/* This is the ComponentStateDestructor implementation */
void oscillator_free(void *state);

#endif
