#ifndef RACK_H
#define RACK_H

#include <stddef.h>
#include <pthread.h>
#include <stdbool.h>
#include "raylib.h"
#include "component_system/component.h"
#include "component_system/component_methods.h"

typedef struct {
    Component component;
    pthread_mutex_t lock;
    Vector2 cursor;
    Component *parent;

    float pan;
    float target_pan;

    float amplitude;
    float target_amplitude;

    /* This is a `RACK_SIZE * RACK_SIZE` long array of Component pointers */
    Component **components;
} Rack;

Component *rack_init(Component *parent);
void rack_free(Component *self);
void rack_preview(Vector2 position, Vector2 size);

void rack_audio_callback(Component *self, float *buffer, size_t size);
void rack_midi_callback(Component *self, const MidiMessage *messages, size_t size, bool system);

void rack_rack_render(Component *self, Vector2 position, Vector2 size);
void rack_settings_render(Component *self, Vector2 position, Vector2 size);
void rack_render(Component *self, Vector2 position, Vector2 size);

void rack_mount(Component *self, Component *component);
void rack_unmount(Component *self);
Component *rack_current(Component *self);
void rack_move_cursor(Component *self, Vector2 delta);

void _rack_midi_callback(Rack* rack, const MidiMessage *messages, size_t size);

void _rack_mount_vec(Rack *rack, Component *component, Vector2 position);
void _rack_unmount_vec(Rack *rack, Vector2 position);
Component *_rack_current_vec(Rack *rack, Vector2 position);

extern ComponentMethods rack_methods;

#endif
