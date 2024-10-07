#ifndef RACK_H
#define RACK_H

#include <raylib.h>
#include <pthread.h>
#include "midi/midi_stream.h"
#include "rack/component.h"

typedef struct {
    int size;
    pthread_mutex_t lock;
    Component **components;
} Rack;

Rack *rack_init(int size);
Component *rack_component_init(Rack *rack);

void rack_mount(Rack *rack, Component *component, int x, int y);
void rack_mount_vec(Rack *rack, Component *component, Vector2 position);

void rack_unmount(Rack *rack, int x, int y);
void rack_unmount_vec(Rack *rack, Vector2 position);

void rack_next(Rack *rack, MidiStream *midi_stream, float *buffer, unsigned int buffer_size);

/* `ComponentAudioCallback`, `ComponentMidiCallback` and `ComponentStateDestructor` implementations.
Which means `Rack` is a `Component` as well. */
void rack_audio_callback(void *state, float *buffer, unsigned int buffer_size);
void rack_midi_callback(void *state, MidiStream *midi_stream);
void rack_state_destructor(void *state);

void rack_free(Rack *rack);

#endif
