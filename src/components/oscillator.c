#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include "constants/color.h"
#include "macros.h"
#include "midi/midi_stream.h"
#include "audio_engine.h"
#include "texture_handler.h"
#include "components/oscillator.h"
#include "component_system/component.h"
#include "component_system/component_methods.h"
#include "font_handler.h"

const char *oscillator_type_to_str(OscillatorType type) {
    switch (type) {
        case OSCILLATOR_SINE:
            return "Sine";
        case OSCILLATOR_SQUARE:
            return "Square";
        case OSCILLATOR_TRIANGLE:
            return "Triangle";
        case OSCILLATOR_SAWTOOTH:
            return "Sawtooth";
        default:
            UNREACHABLE
    }
}

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
    oscillator->pan = 0.0f;

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
        default:
            UNREACHABLE;
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
                switch (message.data.one) {
                    case KEY_LEFT_BRACKET:
                        oscillator->frequency -= 50;
                        break;
                    case KEY_RIGHT_BRACKET:
                        oscillator->frequency += 50;
                        break;
                    case KEY_C:
                        oscillator->type = (oscillator->type + 1) % _OSCILLATOR_TYPE_SIZE;
                        break;
                    case KEY_R:
                        oscillator->pan = MAX(oscillator->pan - 0.1, -1.0f);
                        break;
                    case KEY_L:
                        oscillator->pan = MIN(oscillator->pan + 0.1, 1.0f);
                        break;
                }
        }
    }
}


void pan_meter_render(float pan, Vector2 position, Vector2 size) {
    Rectangle rectangle = {.x = position.x, .y = position.y, .height = size.y, .width = size.x};
    DrawRectangleLinesEx(rectangle, 1, COLOR_GRAY);
    Vector2 rec_position, rec_size;
    if (IS_BETWEEN(pan, -0.05f, 0.05f)) {
        rec_position.x = position.x + size.x / 2 - 1 ;
        rec_position.y = position.y + 1;
        rec_size.x = (int)rectangle.width % 2 == 0 ? 2 : 1;
        rec_size.y = size.y - 2;
    } else if (pan > 0.0f) {
        rec_position.x = position.x + size.x / 2;
        rec_position.y = position.y + 1;
        rec_size.x = pan * (rectangle.width - 2) / 2;
        rec_size.y = size.y - 2;
    } else {
        rec_position.x = position.x + size.x / 2 + pan * (rectangle.width - 2) / 2;
        rec_position.y = position.y + 1;
        rec_size.x = - pan * (rectangle.width - 2) / 2;
        rec_size.y = size.y - 2;
    }
    DrawRectangleV(rec_position, rec_size, COLOR_WHITE);
}



/* TODO: TOO MANY NUMBERS!*/
void oscillator_settings_render(Component* self, Vector2 position, Vector2 size) {
    float y0 = position.y + FONT_HEIGHT_S + FONT_HEIGHT_S / 3;
    float x0 = position.x + FONT_WIDTH_S * 2;
    Vector2 text_position = {x0, y0};
    font_write_s("* OSCILLATOR", text_position, COLOR_GRAY);

    Oscillator *oscillator = (Oscillator *)self;
    static char frequency[25];
    snprintf(frequency, 25, "Frequency %.2fHz", oscillator->frequency);

    text_position.y += FONT_HEIGHT_S * 2;
    font_write_s(frequency, text_position, COLOR_WHITE);

    static char type[25];
    snprintf(type, 20, "Type      %s", oscillator_type_to_str(oscillator->type));
    text_position.y += FONT_HEIGHT_S * 2;
    font_write_s(type, text_position, COLOR_WHITE);

    static char amplitude[25];
    snprintf(amplitude, 20, "Amplitude %.1f%%", oscillator->amplitude * 100);
    text_position.y += FONT_HEIGHT_S * 2;
    font_write_s(amplitude, text_position, COLOR_WHITE);

    text_position.y = y0 + FONT_HEIGHT_S * 2;
    text_position.x = position.x + (int) size.x / 2 + FONT_WIDTH_S * 2;
    font_write_s("Pan", text_position, COLOR_WHITE);
    text_position.x += 6 * FONT_WIDTH_S;
    Vector2 pan_meter_size = {size.x - 1 - text_position.x + position.x - FONT_WIDTH_S * 2, FONT_HEIGHT_S};
    pan_meter_render(oscillator->pan, text_position, pan_meter_size);
}

void oscillator_render(Component *self, Vector2 position, Vector2 size) {
    return oscillator_preview(position, size);
}

void _oscillator_sine_next(Oscillator *oscillator, float *buffer, size_t size) {
    float pan_angle = (oscillator->pan + 1) * (M_PI / 4);
    float left_scale = cosf(pan_angle);
    float right_scale = sinf(pan_angle);

    for (size_t i = 0; i < size * 2; i += 2) {
        float sample = oscillator->amplitude * sinf(oscillator->phase);
        buffer[i] += sample * left_scale;
        buffer[i + 1] += sample * right_scale;

        oscillator->phase += (TWO_PI * oscillator->frequency) / SAMPLE_RATE;
        if (oscillator->phase > TWO_PI) { oscillator->phase -= TWO_PI; }
    }
}

void _oscillator_square_next(Oscillator *oscillator, float *buffer, size_t size) {
    for (size_t i = 0; i < size * 2; i += 2) {
        float sample = (sinf(oscillator->phase) >= 0.0f) ? oscillator->amplitude : -oscillator->amplitude;
        buffer[i] += sample;
        buffer[i + 1] += sample;

        oscillator->phase += (TWO_PI * oscillator->frequency) / SAMPLE_RATE;
        if (oscillator->phase > TWO_PI) { oscillator->phase -= TWO_PI; }
    }
}

void _oscillator_triangle_next(Oscillator *oscillator, float *buffer, size_t size) { NOT_IMPLEMENTED }
void _oscillator_sawtooth_next(Oscillator *oscillator, float *buffer, size_t size) { NOT_IMPLEMENTED }

ComponentMethods oscillator_methods = {
    .init = oscillator_init,
    .free = oscillator_free,
    .preview = oscillator_preview,
    .audio_callback = oscillator_audio_callback,
    .midi_callback = oscillator_midi_callback,
    .settings_render = oscillator_settings_render,
    .render = oscillator_render,
    .rack_render = NULL,
    .mount = NULL,
    .unmount = NULL,
    .current = NULL,
    .move_cursor = NULL,
};
