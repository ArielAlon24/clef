#include <stdlib.h>
#include <assert.h>
#include <stddef.h>
#include "components/rack.h"
#include "component_system/component.h"
#include "component_system/component_methods.h"
#include "component_system/component_system.h"
#include "macros.h"
#include "texture_handler.h"
#include "constants/color.h"
#include "constants/size.h"

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
}

void rack_midi_callback(Component *self, const MidiMessage *messages, size_t size, bool system) {
    /* If MIDI is not system wide, ignore it */
    if (!system) return;
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

void rack_render(Component *self, Vector2 position, Vector2 size) {
    return rack_preview(position, size);
}

void rack_rack_render(Component *self, Vector2 position, Vector2 size) {
    Rack *rack = (Rack *)self;
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
            component_render(component, component_position, COMPONENT_DIMENSIONS);
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
    .settings_render = NULL,
    .rack_render = rack_rack_render,
    .render = rack_render,
    .mount = rack_mount,
    .unmount = rack_unmount,
    .current = rack_current,
    .move_cursor = rack_move_cursor,
};