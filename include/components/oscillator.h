#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#include <stddef.h>
#include <stdbool.h>
#include "raylib.h"
#include "component_system/component.h"
#include "component_system/component_methods.h"

#define TWO_PI 6.283185307179586

typedef enum {
    OSCILLATOR_SINE,
    OSCILLATOR_SQUARE,
    OSCILLATOR_TRIANGLE,
    OSCILLATOR_SAWTOOTH,
    _OSCILLATOR_TYPE_SIZE,
} OscillatorType;

const char *oscillator_type_to_str(OscillatorType type);

typedef struct {
    Component component;
    OscillatorType type;
    float frequency;
    float amplitude; /* Value between 0 and 1 where 0 is no sound and 1 is full sound */
    float target_amplitude; /* After an amplitude change `amplitude` will slowly be `target_amplitude` to avoid clicks */
    float phase;
    float pan; /* Value between -1 and 1 where -1 is completely left and 1 is completely right */
} Oscillator;

Component *oscillator_init(Component *parent);
void oscillator_free(Component *self);
void oscillator_preview(Vector2 position, Vector2 size);

void oscillator_audio_callback(Component *self, float *buffer, size_t size);
void oscillator_midi_callback(Component *self, const MidiMessage *messages, size_t size, bool system);

void oscillator_settings_render(Component *self, Vector2 position, Vector2 size);
void oscillator_render(Component *self, Vector2 position, Vector2 size);

void _oscillator_sine_next(Oscillator *oscillator, float *buffer, size_t size);
void _oscillator_square_next(Oscillator *oscillator, float *buffer, size_t size);
void _oscillator_triangle_next(Oscillator *oscillator, float *buffer, size_t size);
void _oscillator_sawtooth_next(Oscillator *oscillator, float *buffer, size_t size);

extern ComponentMethods oscillator_methods;

#endif
