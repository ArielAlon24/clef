#ifndef COMPONENT_H
#define COMPONENT_H

#include "midi/midi_stream.h"
#include <raylib.h>

typedef void (*ComponentAudioCallback)(void *state, float *buffer, unsigned int buffer_size);
typedef void (*ComponentMidiCallback)(void *state, const MidiMessage *messages, unsigned int count);

/* The Component struct, takes ownership of the component's state. Because of that it must know
how to destruct the state upon a `component_free` call. The functions `ComponentStateDestructor`
should be supplied and handle freeing the state correctly. */
typedef void (*ComponentStateDestructor)(void *state);

typedef struct {
    ComponentAudioCallback audio_callback;
    ComponentMidiCallback midi_callback;
    ComponentStateDestructor state_destructor;

    /* TODO: Currently using a single color to draw a component, this will later be a sprite. */
    Color color;

    void *state;
} Component;

Component *component_init(ComponentAudioCallback audio_callback,
                          ComponentMidiCallback midi_callback,
                          ComponentStateDestructor state_destructor, Color color, void *state);

void component_next_audio(Component *component, float *buffer, unsigned int buffer_size);

void component_next_midi(Component *component, const MidiMessage *message, unsigned int count);

void component_render(Component *component, Vector2 position, Vector2 size);

void component_free(Component *component);

#endif
