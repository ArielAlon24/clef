#include <stdlib.h>
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include "components/rack.h"
#include "component_system/component.h"
#include "component_system/component_methods.h"
#include "component_system/component_system.h"
#include "macros.h"
#include "texture_handler.h"
#include "constants/color.h"
#include "constants/size.h"
#include "font_handler.h"
#include "widgets/pan_meter.h"

Component *rack_init(Component *parent) {
    Rack *rack = malloc(sizeof(Rack));
    assert(rack != NULL);

    rack->component.is_container = true;
    rack->component.type = COMPONENT_RACK;
    rack->component.parent = parent;

    rack->components = calloc(RACK_SIZE * RACK_SIZE, sizeof(Component *));
    assert(rack->components != NULL);

    rack->cursor.x = 0;
    rack->cursor.y = 0;
    rack->parent = parent;

    rack->amplitude = 1.0f;
    rack->target_amplitude = 1.0f;
    rack->pan = 0.0f;
    rack->target_pan = 0.0f;

    pthread_mutex_init(&rack->lock, NULL);

    return (Component *)rack;
}

void rack_free(Component *self) {
    Rack *rack = (Rack *)self;
    Component *component;
    for (int i = 0; i < RACK_SIZE * RACK_SIZE; ++i) {
        component = rack->components[i];
        if (component != NULL) {
            component_free(component);
        }
    }

    pthread_mutex_destroy(&rack->lock);

    free(rack->components);
    free(rack);
}

void rack_preview(Vector2 position, Vector2 size) {
    Texture2D texture = texture_load(TEXTURE_RACK_PREVIEW);
    DrawTextureV(texture, position, WHITE);
}

void rack_audio_callback(Component *self, float *buffer, size_t size) {
    Rack *rack = (Rack *)self;
    pthread_mutex_lock(&rack->lock);
    Component *component;
    for (int i = 0; i < RACK_SIZE * RACK_SIZE; ++i) {
        component = rack->components[i];
        if (component != NULL) {
            component_audio_callback(component, buffer, size);
        }
    }
    pthread_mutex_unlock(&rack->lock);

    float amplitude_step = (rack->target_amplitude - rack->amplitude) / size;
    float pan_step = (rack->target_pan - rack->pan) / size;

    for (size_t i = 0; i < size * 2; i += 2) {
        rack->amplitude = CLAMP(rack->amplitude + amplitude_step, 0.0f, 1.0f);
        rack->pan = CLAMP(rack->pan + pan_step, -1.0f, 1.0f);

        buffer[i] *= PAN_L_SCALE(rack->pan) * rack->amplitude;
        buffer[i + 1] *= PAN_R_SCALE(rack->pan) * rack->amplitude;
    }
}

void rack_midi_callback(Component *self, const MidiMessage *messages, size_t size, bool system) {
    /* If MIDI is not system wide, ignore it */
    if (!system) return _rack_midi_callback((Rack *)self, messages, size);
    Rack *rack = (Rack *)self;
    pthread_mutex_lock(&rack->lock);
    Component *component;
    for (int i = 0; i < RACK_SIZE * RACK_SIZE; ++i) {
        component = rack->components[i];
        if (component != NULL) {
            component_midi_callback(component, messages, size, system);
        }
    }
    pthread_mutex_unlock(&rack->lock);
}

/* TODO: Create `rack_settings_render` */
void rack_settings_render(Component *self, Vector2 position, Vector2 size) {
    Rack *rack = (Rack *)self;
    float y0 = position.y + FONT_HEIGHT_S + FONT_HEIGHT_S / 3;
    float x0 = position.x + FONT_WIDTH_S * 2;
    Vector2 text_position = {x0, y0};
    font_write_s("* RACK", text_position, COLOR_GRAY);

    static char amplitude[25];
    snprintf(amplitude, 20, "Amplitude %.1f%%", rack->target_amplitude * 100);
    text_position.y += FONT_HEIGHT_S * 2;
    font_write_s(amplitude, text_position, COLOR_WHITE);

    text_position.y = y0 + FONT_HEIGHT_S * 2;
    text_position.x = position.x + (int) size.x / 2 + FONT_WIDTH_S * 2;
    font_write_s("Pan", text_position, COLOR_WHITE);
    text_position.x += 6 * FONT_WIDTH_S;
    Vector2 pan_meter_size = {size.x - 1 - text_position.x + position.x - FONT_WIDTH_S * 2, FONT_HEIGHT_S};
    pan_meter_render(rack->pan, text_position, pan_meter_size);
}

void rack_render(Component *self, Vector2 position, Vector2 size) {
    return rack_preview(position, size);
}

void rack_rack_render(Component *self, Vector2 position, Vector2 size) {
    Rack *rack = (Rack *)self;
    Texture2D empty_cell_texture = texture_load(TEXTURE_CELL);
    Vector2 padding = { COMPONENT_DIMENSIONS.x / (RACK_SIZE - 1), COMPONENT_DIMENSIONS.y / (RACK_SIZE - 1)};
    DrawRectangleV(position, size, COLOR_BLACK);

    Component *component;
    Vector2 component_position;
    for (int i = 0; i < RACK_SIZE * RACK_SIZE; ++i) {
        component = rack->components[i];
        component_position.x = ((int) i / RACK_SIZE) * (COMPONENT_DIMENSIONS.x + padding.x) + position.x;
        component_position.y = ((int) i % RACK_SIZE) * (COMPONENT_DIMENSIONS.y + padding.y) + position.y;
        if (component != NULL) {
            component_render(component, component_position, COMPONENT_DIMENSIONS);
        } else {
            DrawTextureV(empty_cell_texture, component_position, WHITE);
        }
    }

    Texture2D cursor_texture = texture_load(TEXTURE_CURSOR);
    Vector2 cursor_position;
    cursor_position.x = rack->cursor.x * (COMPONENT_DIMENSIONS.x + padding.x) + position.x - 1;
    cursor_position.y = rack->cursor.y * (COMPONENT_DIMENSIONS.y + padding.y) + position.y - 1;

    DrawTextureV(cursor_texture, cursor_position, WHITE);
}

void rack_mount(Component *self, Component *component) {
    Rack *rack = (Rack *)self;
    return _rack_mount_vec(rack, component, rack->cursor);
}

void _rack_mount_vec(Rack *rack, Component *component, Vector2 position) {
    pthread_mutex_lock(&rack->lock);
    int index = RACK_SIZE * (int) position.x + (int) position.y;
    Component *previous = rack->components[index];
    if (previous != NULL) {
        component_free(previous);
    }
    rack->components[index] = component;
    pthread_mutex_unlock(&rack->lock);
}

void rack_unmount(Component *self) {
    Rack *rack = (Rack *)self;
    return _rack_unmount_vec(rack, rack->cursor);
}

void _rack_midi_callback(Rack *rack, const MidiMessage *messages, size_t size) {
    MidiMessage message;
    FOREACH(message, messages, size) {
        switch (message.type) {
            case MIDI_MESSAGE_SYSEX:
                switch (message.data.one) {
                    case KEY_R:
                        rack->target_pan = MAX(rack->target_pan - 0.1f, -1.0f);
                        break;
                    case KEY_L:
                        rack->target_pan = MIN(rack->pan + 0.1f, 1.0f);
                        break;
                    case KEY_EQUAL:
                        rack->target_amplitude = MIN(rack->target_amplitude + 0.05f, 1.0f);
                        break;
                    case KEY_MINUS:
                        rack->target_amplitude = MAX(rack->target_amplitude - 0.05f, 0.0f);
                        break;
                }
        }
    }
}

void _rack_unmount_vec(Rack *rack, Vector2 position) {
    pthread_mutex_lock(&rack->lock);
    int index = RACK_SIZE * (int) position.x + (int) position.y;
    Component *component = rack->components[index];
    if (component != NULL) {
        component_free(component);
    }
    rack->components[index] = NULL;
    pthread_mutex_unlock(&rack->lock);
}

Component *rack_current(Component *self) {
    Rack *rack = (Rack *)self;
    return _rack_current_vec(rack, rack->cursor);
}

Component *_rack_current_vec(Rack *rack, Vector2 position) {
    pthread_mutex_lock(&rack->lock);
    int index = RACK_SIZE * (int) position.x + (int) position.y;
    Component *component = rack->components[index];
    pthread_mutex_unlock(&rack->lock);
    return component;
}

void rack_move_cursor(Component *self, Vector2 delta) {
    Rack *rack = (Rack *)self;
    rack->cursor.x = CLAMP(rack->cursor.x + delta.x, 0, RACK_SIZE - 1);
    rack->cursor.y = CLAMP(rack->cursor.y + delta.y, 0, RACK_SIZE - 1);
}

ComponentMethods rack_methods = {
    .init = rack_init,
    .free = rack_free,
    .preview = rack_preview,
    .audio_callback = rack_audio_callback,
    .midi_callback = rack_midi_callback,
    .settings_render = rack_settings_render,
    .rack_render = rack_rack_render,
    .render = rack_render,
    .mount = rack_mount,
    .unmount = rack_unmount,
    .current = rack_current,
    .move_cursor = rack_move_cursor,
};
