#include <stdlib.h>
#include <assert.h>
#include "rack/rack.h"
#include "rack/component_handler.h"
#include "macros.h"
#include "texture_handler.h"
#include "constants/color.h"
#include "constants/size.h"

Rack *rack_init(Rack *parent) {
    Rack *rack = malloc(sizeof(Rack));
    assert(rack != NULL);

    rack->components = calloc(RACK_SIZE * RACK_SIZE, sizeof(Component *));
    assert(rack->components != NULL);

    rack->cursor.x = 0;
    rack->cursor.y = 0;

    rack->parent = parent;

    pthread_mutex_init(&rack->lock, NULL);

    return rack;
}

Component *rack_component_init(Rack *parent) {
    Rack *rack = rack_init(parent);
    return component_init(rack_audio_callback, rack_midi_callback, rack_state_destructor, true, TEXTURE_RACK_PREVIEW, rack);
}

void rack_mount(Rack *rack, Component *component) {
    return rack_mount_vec(rack, component, rack->cursor);
}

void rack_mount_vec(Rack *rack, Component *component, Vector2 position) {
    pthread_mutex_lock(&rack->lock);
    int index = RACK_SIZE * (int) position.x + (int) position.y;
    Component *previous = rack->components[index];
    if (previous != NULL) {
        component_free(previous);
    }
    rack->components[index] = component;
    pthread_mutex_unlock(&rack->lock);
}

void rack_unmount(Rack *rack) {
    return rack_unmount_vec(rack, rack->cursor);
}

void rack_unmount_vec(Rack *rack, Vector2 position) {
    pthread_mutex_lock(&rack->lock);
    int index = RACK_SIZE * (int) position.x + (int) position.y;
    Component *component = rack->components[index];
    if (component != NULL) {
        component_free(component);
    }
    rack->components[index] = NULL;
    pthread_mutex_unlock(&rack->lock);
}

Component *rack_get_component(Rack *rack) {
    return rack_get_component_vec(rack, rack->cursor);
}

Component *rack_get_component_vec(Rack *rack, Vector2 position) {
    pthread_mutex_lock(&rack->lock);
    int index = RACK_SIZE * (int) position.x + (int) position.y;
    Component *component = rack->components[index];
    pthread_mutex_unlock(&rack->lock);
    return component;
}

Rack *rack_get_parent(Rack *rack) { return rack->parent; }

void rack_next(Rack *rack, const MidiMessage *messages, size_t count, float *buffer, unsigned int buffer_size) {
    pthread_mutex_lock(&rack->lock);
    Component *component;

    for (int i = 0; i < RACK_SIZE * RACK_SIZE; ++i) {
        component = rack->components[i];
        if (component != NULL) {
            component_next_midi(component, messages, count);
            component_next_audio(component, buffer, buffer_size);
        }
    }

    pthread_mutex_unlock(&rack->lock);
}

void rack_render(Rack *rack, Vector2 position, Vector2 size) {
    Texture2D empty_cell_texture = texture_load(TEXTURE_EMPTY_CELL);
    Vector2 component_size = { size.x / (RACK_SIZE + 1), size.y / (RACK_SIZE + 1)};
    Vector2 padding = { component_size.x / (RACK_SIZE - 1), component_size.y / (RACK_SIZE - 1)};
    DrawRectangleV(position, size, COLOR_BLACK);

    Component *component;
    Vector2 component_position;
    for (int i = 0; i < RACK_SIZE * RACK_SIZE; ++i) {
        component = rack->components[i];
        component_position.x = ((int) i / RACK_SIZE) * (COMPONENT_DIMENSIONS.x + padding.x) + position.x;
        component_position.y = ((int) i % RACK_SIZE) * (COMPONENT_DIMENSIONS.y + padding.y) + position.y;
        if (component != NULL) {
            component_render(component, component_position);
        } else {
            DrawTextureV(empty_cell_texture, component_position, WHITE);
        }
    }

    Texture2D cursor_texture = texture_load(TEXTURE_CURSOR);
    Vector2 cursor_position;
    cursor_position.x = rack->cursor.x * (COMPONENT_DIMENSIONS.x + padding.x) + position.x;
    cursor_position.y = rack->cursor.y * (COMPONENT_DIMENSIONS.y + padding.y) + position.y;

    DrawTextureV(cursor_texture, cursor_position, WHITE);
}

void rack_cursor_right(Rack *rack) { rack->cursor.x = MIN(RACK_SIZE - 1, rack->cursor.x + 1); }
void rack_cursor_left(Rack *rack) { rack->cursor.x = MAX(0, rack->cursor.x - 1); }
void rack_cursor_up(Rack *rack) { rack->cursor.y = MAX(0, rack->cursor.y - 1); }
void rack_cursor_down(Rack *rack) { rack->cursor.y = MIN(RACK_SIZE - 1, rack->cursor.y + 1); }

void rack_audio_callback(void *state, float *buffer, unsigned int buffer_size) {
    Rack *rack = (Rack *)state;
    pthread_mutex_lock(&rack->lock);
    Component *component;
    for (int i = 0; i < RACK_SIZE * RACK_SIZE; ++i) {
        component = rack->components[i];
        if (component != NULL) {
            component_next_audio(component, buffer, buffer_size);
        }
    }
    pthread_mutex_unlock(&rack->lock);
}

void rack_midi_callback(void *state, const MidiMessage *messages, unsigned int count) {
    Rack *rack = (Rack *)state;
    pthread_mutex_lock(&rack->lock);
    Component *component;
    for (int i = 0; i < RACK_SIZE * RACK_SIZE; ++i) {
        component = rack->components[i];
        if (component != NULL) {
            component_next_midi(component, messages, count);
        }
    }
    pthread_mutex_unlock(&rack->lock);
}

void rack_state_destructor(void *state) {
    rack_free((Rack *)state);
}

void rack_free(Rack *rack) {
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

void rack_preview(Vector2 position) {
    Texture2D texture = texture_load(TEXTURE_RACK_PREVIEW);
    DrawTextureV(texture, position, WHITE);
}
