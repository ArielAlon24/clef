#include <stdlib.h>
#include <assert.h>
#include "rack/rack.h"
#include "macros.h"

Rack *rack_init(int size) {
    Rack *rack = malloc(sizeof(Rack));
    assert(rack != NULL);

    rack->components = calloc(size * size, sizeof(Component *));
    assert(rack->components != NULL);
    rack->size = size;

    rack->cursor.x = 0;
    rack->cursor.y = 0;

    pthread_mutex_init(&rack->lock, NULL);

    return rack;
}

Component *rack_component_init(Rack *rack) {
    return component_init(rack_audio_callback, rack_midi_callback, rack_state_destructor, RED, rack);
}

void rack_mount(Rack *rack, Component *component) {
    return rack_mount_vec(rack, component, rack->cursor);
}

void rack_mount_vec(Rack *rack, Component *component, Vector2 position) {
    pthread_mutex_lock(&rack->lock);
    int index = rack->size * (int) position.x + (int) position.y;
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
    int index = rack->size * (int) position.x + (int) position.y;
    Component *component = rack->components[index];
    if (component != NULL) {
        component_free(component);
    }
    rack->components[index] = NULL;
    pthread_mutex_unlock(&rack->lock);
}

void rack_next(Rack *rack, MidiStream *midi_stream, float *buffer, unsigned int buffer_size) {
    pthread_mutex_lock(&rack->lock);
    Component *component;
    for (int i = 0; i < rack->size * rack->size; ++i) {
        component = rack->components[i];
        if (component != NULL) {
            component_next_midi(component, midi_stream);
            component_next_audio(component, buffer, buffer_size);
        }
    }
    pthread_mutex_unlock(&rack->lock);
}

void rack_render(Rack *rack, Vector2 position, Vector2 size) {
    Vector2 component_size = { size.x / (rack->size + 1), size.y / (rack->size + 1)};
    Vector2 padding = { component_size.x / (rack->size - 1), component_size.y / (rack->size - 1)};

    DrawRectangleV(position, size, BLACK);

    Component *component;
    Vector2 component_position;
    for (int i = 0; i < rack->size * rack->size; ++i) {
        component = rack->components[i];
        component_position.x = ((int) i / rack->size) * (component_size.x + padding.x) + position.x;
        component_position.y = ((int) i % rack->size) * (component_size.y + padding.y) + position.y;
        if (component != NULL) {
            component_render(component, component_position, component_size);
        } else {
            DrawRectangleV(component_position, component_size, DARKGRAY);
        }
    }

    Rectangle cursor_rectangle;
    cursor_rectangle.x = rack->cursor.x * (component_size.x + padding.x) + position.x;
    cursor_rectangle.y = rack->cursor.y * (component_size.y + padding.y) + position.y;
    cursor_rectangle.height = component_size.x;
    cursor_rectangle.width = component_size.y;
    DrawRectangleLinesEx(cursor_rectangle, 3, YELLOW);
}

void rack_cursor_right(Rack *rack) { rack->cursor.x = MIN(rack->size - 1, rack->cursor.x + 1); }
void rack_cursor_left(Rack *rack) { rack->cursor.x = MAX(0, rack->cursor.x - 1); }
void rack_cursor_up(Rack *rack) { rack->cursor.y = MAX(0, rack->cursor.y - 1); }
void rack_cursor_down(Rack *rack) { rack->cursor.y = MIN(rack->size - 1, rack->cursor.y + 1); }

void rack_audio_callback(void *state, float *buffer, unsigned int buffer_size) {
    Rack *rack = (Rack *)state;
    pthread_mutex_lock(&rack->lock);
    Component *component;
    for (int i = 0; i < rack->size * rack->size; ++i) {
        component = rack->components[i];
        if (component != NULL) {
            component_next_audio(component, buffer, buffer_size);
        }
    }
    pthread_mutex_unlock(&rack->lock);
}

void rack_midi_callback(void *state, MidiStream *midi_stream) {
    Rack *rack = (Rack *)state;
    pthread_mutex_lock(&rack->lock);
    Component *component;
    for (int i = 0; i < rack->size * rack->size; ++i) {
        component = rack->components[i];
        if (component != NULL) {
            component_next_midi(component, midi_stream);
        }
    }
    pthread_mutex_unlock(&rack->lock);
}

void rack_state_destructor(void *state) {
    rack_free((Rack *)state);
}

void rack_free(Rack *rack) {
    Component *component;
    for (int i = 0; i < rack->size * rack->size; ++i) {
        component = rack->components[i];
        if (component != NULL) {
            component_free(component);
        }
    }

    pthread_mutex_destroy(&rack->lock);

    free(rack->components);
    free(rack);
}