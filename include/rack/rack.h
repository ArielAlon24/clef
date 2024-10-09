#ifndef RACK_H
#define RACK_H

#include <raylib.h>
#include <pthread.h>
#include "midi/midi_stream.h"
#include "rack/component.h"

typedef struct {
    int size;
    pthread_mutex_t lock;
    Vector2 cursor;
    Component **components;
} Rack;

Rack *rack_init(int size);
Component *rack_component_init(Rack *rack);


void rack_mount(Rack *rack, Component *component);
void rack_mount_vec(Rack *rack, Component *component, Vector2 position);

void rack_unmount(Rack *rack);
void rack_unmount_vec(Rack *rack, Vector2 position);

void rack_next(Rack *rack, MidiStream *midi_stream, float *buffer, unsigned int buffer_size);

void rack_render(Rack *rack, Vector2 position, Vector2 size);

void rack_cursor_right(Rack *rack);
void rack_cursor_left(Rack *rack);
void rack_cursor_up(Rack *rack);
void rack_cursor_down(Rack *rack);

/* `ComponentAudioCallback`, `ComponentMidiCallback` and `ComponentStateDestructor` implementations.
Which means `Rack` is a `Component` as well. */
void rack_audio_callback(void *state, float *buffer, unsigned int buffer_size);
void rack_midi_callback(void *state, const MidiMessage *message, unsigned int count);
void rack_state_destructor(void *state);

void rack_free(Rack *rack);

#endif
