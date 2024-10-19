#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include "macros.h"
#include "midi/midi_stream.h"
#include "audio_engine.h"
#include "texture_handler.h"
#include "components/oscillator.h"
#include "component_system/component.h"
#include "component_system/component_methods.h"

Component *oscillator_init(Component *parent) {
    Oscillator *oscillator = malloc(sizeof(Oscillator));
    assert(oscillator != NULL);

    oscillator->component.type = COMPONENT_OSCILLATOR;
    oscillator->component.is_container = false;
    oscillator->component.parent = parent;

    oscillator->type = OSCILLATOR_SINE;
    oscillator->frequency = 440.0f;
    oscillator->amplitude = 0.2f;
    oscillator->phase = 0.0f;

    return (Component *)oscillator;
}

void oscillator_free(Component *self) {
    free((Oscillator *)self);
}

void oscillator_preview(Vector2 position, Vector2 size) {
    Texture2D texture = texture_load(TEXTURE_OSCILLATOR);
    DrawTextureV(texture, position, WHITE);
}

void oscillator_audio_callback(Component *self, float *buffer, size_t size) {
    Oscillator *oscillator = (Oscillator *)self;

    /* If frequency is 0Hz, mute the oscillator */
    if (oscillator->frequency == 0.0f) return;

    switch (oscillator->type) {
        case OSCILLATOR_SINE:
            return _oscillator_sine_next(oscillator, buffer, size);
        case OSCILLATOR_SQUARE:
            return _oscillator_square_next(oscillator, buffer, size);
        case OSCILLATOR_TRIANGLE:
            return _oscillator_triangle_next(oscillator, buffer, size);
        case OSCILLATOR_SAWTOOTH:
            return _oscillator_sawtooth_next(oscillator, buffer, size);
    }
}

void oscillator_midi_callback(Component *self, const MidiMessage *messages, size_t size, bool system) {
    Oscillator *oscillator = (Oscillator *)self;
    MidiMessage message;
    for (int i = 0; i < size; ++i) {
        message = messages[i];
        switch (message.type) {
            case MIDI_MESSAGE_START:
                oscillator->phase = 0.0f;
                break;
            case MIDI_MESSAGE_SYSEX:
                if (message.data.one == 1) {
                    oscillator->frequency -= 50;
                } else if (message.data.one == 2) {
                    oscillator->frequency += 50;
                }
                break;
        }
    }
}

void _oscillator_sine_next(Oscillator *oscillator, float *buffer, size_t size) {
    for (size_t i = 0; i < size * 2; i += 2) {
        float sample = oscillator->amplitude * sinf(oscillator->phase);
        buffer[i] += sample;
        buffer[i + 1] += sample;
        buffer[i + 1] *= -1;

        oscillator->phase += (TWO_PI * oscillator->frequency) / SAMPLE_RATE;
        if (oscillator->phase > TWO_PI) { oscillator->phase -= TWO_PI; }
    }
}

void _oscillator_square_next(Oscillator *oscillator, float *buffer, size_t size) {
    for (size_t i = 0; i < size * 2; i += 2) {
        float sample = (sinf(oscillator->phase) >= 0.0f) ? oscillator->amplitude : -oscillator->amplitude;
        buffer[i] = sample;
        buffer[i + 1] = sample;

        oscillator->phase += (TWO_PI * oscillator->frequency) / SAMPLE_RATE;
        if (oscillator->phase > TWO_PI) { oscillator->phase -= TWO_PI; }
    }
}

void _oscillator_triangle_next(Oscillator *oscillator, float *buffer, size_t size) { NOT_IMPLEMENTED }
void _oscillator_sawtooth_next(Oscillator *oscillator, float *buffer, size_t size) { NOT_IMPLEMENTED }

void oscillator_render(Component *self, Vector2 position, Vector2 size) {
    return oscillator_preview(position, size);
}

ComponentMethods oscillator_methods = {
    .init = oscillator_init,
    .free = oscillator_free,
    .preview = oscillator_preview,
    .audio_callback = oscillator_audio_callback,
    .midi_callback = oscillator_midi_callback,
    .settings_render = NULL,
    .render = oscillator_render,
    .rack_render = NULL,
    .mount = NULL,
    .unmount = NULL,
    .current = NULL,
    .move_cursor = NULL,
};
