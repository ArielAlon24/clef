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
    /* This is a `RACK_SIZE * RACK_SIZE` long array of Component pointers */
    Component **components;
} Rack;

Component *rack_init(Component *parent);
void rack_free(Component *self);
void rack_preview(Vector2 position, Vector2 size);

void rack_audio_callback(Component *self, float *buffer, size_t size);
void rack_midi_callback(Component *self, const MidiMessage *messages, size_t size, bool system);

void rack_render(Component * self, Vector2 position, Vector2 size);

void rack_mount(Component *self, Component *component);
void rack_unmount(Component *self);
Component *rack_current(Component *self);
void rack_move_cursor(Component *self, Vector2 delta);

extern ComponentMethods oscillator_methods = {
    .init = rack_init,
    .free = rack_free,
    .preview = rack_preview,
    .audio_callback = rack_audio_callback,
    .midi_callback = rack_midi_callback,
    .settings_render = NULL,
    .render = rack_render,
    .mount = rack_mount,
    .unmount = rack_unmount,
    .current = rack_current,
    .move_cursor = rack_move_cursor,
};

#endif
