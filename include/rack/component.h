#ifndef COMPONENT_H
#define COMPONENT_H

#include "midi/midi_stream.h"
#include "texture_handler.h"
#include <raylib.h>
#include <stdbool.h>

typedef void (*ComponentAudioCallback)(void *state, float *buffer, unsigned int buffer_size);
typedef void (*ComponentMidiCallback)(void *state, const MidiMessage *messages, unsigned int count);

/* The Component struct, takes ownership of the component's state. Because of that it must know
how to destruct the state upon a `component_free` call. The functions `ComponentStateDestructor`
should be supplied and handle freeing the state correctly. */
typedef void (*ComponentStateDestructor)(void *state);

typedef enum {
    COMPONENT_RACK,
    COMPONENT_OSCILLATOR,
    _COMPONENT_TYPE_SIZE,
} ComponentType;

typedef struct {
    ComponentAudioCallback audio_callback;
    ComponentMidiCallback midi_callback;
    ComponentStateDestructor state_destructor;

    /* TODO: Specify `is_enterable` using a ComponentType enum or something (is_enterable sounds weird) */
    bool is_enterable;

    /* TODO: Create a callback for animating the component design */
    TextureKind texture_kind;

    void *state;
} Component;


Component *component_init(ComponentAudioCallback audio_callback,
                          ComponentMidiCallback midi_callback,
                          ComponentStateDestructor state_destructor, bool is_enterable, TextureKind texture_kind, void *state);

void component_next_audio(Component *component, float *buffer, unsigned int buffer_size);

void component_next_midi(Component *component, const MidiMessage *message, unsigned int count);

void component_render(Component *component, Vector2 position);

void component_free(Component *component);

bool component_is_enterable(Component *component);

#endif
