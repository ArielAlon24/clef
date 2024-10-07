#include "rack/component.h"
#include "midi/midi_stream.h"
#include <stdlib.h>
#include <assert.h>

Component *component_init(ComponentAudioCallback audio_callback,
                          ComponentMidiCallback midi_callback,
                          ComponentStateDestructor state_destructor, void *state) {
    Component *component = malloc(sizeof(Component));
    assert(component != NULL);

    component->audio_callback = audio_callback;
    component->midi_callback = midi_callback;
    component->state_destructor = state_destructor;
    component->state = state;

    return component;
}

void component_next_audio(Component *component, float *buffer, unsigned int buffer_size) {
    if (component->audio_callback != NULL) {
        component->audio_callback(component->state, buffer, buffer_size);
    }
}

void component_next_midi(Component *component, MidiStream *midi_stream) {
    if (component->midi_callback != NULL) {
        component->midi_callback(component->state, midi_stream);
    }
}

void component_free(Component *component) {
    component->state_destructor(component->state);
    free(component);
}
