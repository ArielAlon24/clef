#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include "components/oscillator.h"
#include "rack/component.h"
#include "macros.h"
#include "midi/midi_stream.h"
#include "audio_engine.h"
#include "texture_handler.h"
#include "rack/component_handler.h"

Component *oscillator_init() {
    Oscillator *oscillator = malloc(sizeof(Oscillator));
    assert(oscillator != NULL);

    oscillator->type = OSCILLATOR_SINE;
    oscillator->frequency = 440.0;
    oscillator->amplitude = 0.5;
    oscillator->phase = 0.0f;

    Component *component = component_init(oscillator_audio_callback, oscillator_midi_callback, oscillator_free, false, TEXTURE_OSCILLATOR, oscillator);

    return component;
}

void oscillator_audio_callback(void *state, float *buffer, unsigned int frame_count) {
    Oscillator *oscillator = (Oscillator *)state;
    /* If frequency is 0Hz, mute the oscillator */
    if (oscillator->frequency == 0.0f) return;

    switch (oscillator->type) {
        case OSCILLATOR_SINE:
            return _oscillator_sine_next(oscillator, buffer, frame_count);
        case OSCILLATOR_SQUARE:
            return _oscillator_square_next(oscillator, buffer, frame_count);
        case OSCILLATOR_TRIANGLE:
            return _oscillator_triangle_next(oscillator, buffer, frame_count);
        case OSCILLATOR_SAWTOOTH:
            return _oscillator_sawtooth_next(oscillator, buffer, frame_count);
    }
}

void oscillator_midi_callback(void *state, const MidiMessage *messages, unsigned int count) {
    Oscillator *oscillator = (Oscillator *)state;
    MidiMessage message;
    for (int i = 0; i < count; ++i) {
        switch (messages[i].type) {
            case MIDI_MESSAGE_START:
                oscillator->phase = 0.0f;
                break;
        }
    }
}

void _oscillator_sine_next(Oscillator *oscillator, float *buffer, unsigned int frame_count) {
    for (unsigned int i = 0; i < frame_count * 2; i += 2) {
        float sample = oscillator->amplitude * sinf(oscillator->phase);
        buffer[i] += sample;
        buffer[i + 1] += sample;
        buffer[i + 1] *= -1;

        oscillator->phase += (TWO_PI * oscillator->frequency) / SAMPLE_RATE;
        if (oscillator->phase > TWO_PI) { oscillator->phase -= TWO_PI; }
    }
}

void _oscillator_square_next(Oscillator *oscillator, float *buffer, unsigned int frame_count) {
    for (unsigned int i = 0; i < frame_count * 2; i += 2) {
        float sample = (sinf(oscillator->phase) >= 0.0f) ? oscillator->amplitude : -oscillator->amplitude;
        buffer[i] = sample;
        buffer[i + 1] = sample;

        oscillator->phase += (TWO_PI * oscillator->frequency) / SAMPLE_RATE;
        if (oscillator->phase > TWO_PI) { oscillator->phase -= TWO_PI; }
    }
}
void _oscillator_triangle_next(Oscillator *oscillator, float *buffer, unsigned int frame_count) { NOT_IMPLEMENTED }
void _oscillator_sawtooth_next(Oscillator *oscillator, float *buffer, unsigned int frame_count) { NOT_IMPLEMENTED }

void oscillator_free(void *state) {
    free((Oscillator *)state);
}

void oscillator_preview(Vector2 position) {
    Texture2D texture = texture_load(TEXTURE_OSCILLATOR);
    DrawTextureV(texture, position, WHITE);
}
