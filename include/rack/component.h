#ifndef COMPONENT_H
#define COMPONENT_H

#include "midi_stream.h"

typedef void (*ComponentAudioCallback)(void *state, float *buffer, unsigned int buffer_size);
typedef void (*ComponentMidiCallback)(void *state, MidiStream *midi_stream);

/* The Component struct, takes ownership of the component's state. Because of that it must know
how to destruct the state upon a `component_free` call. The functions `ComponentStateDestructor`
should be supplied and handle freeing the state correctly. */
typedef void (*ComponentStateDestructor)(void *state);

typedef struct {
    ComponentAudioCallback audio_callback;
    ComponentMidiCallback midi_callback;
    ComponentStateDestructor state_destructor;

    void *state;
} Component;

Component *component_init(ComponentAudioCallback audio_callback,
                          ComponentMidiCallback midi_callback,
                          ComponentStateDestructor state_destructor, void *state);

void component_next(Component *component, MidiStream *midi_stream, float *buffer, unsigned int buffer_size);

void component_free(Component *component);

#endif
