#include "rack/component.h"
#include "midi/midi_stream.h"
#include "texture_handler.h"
#include <stdlib.h>
#include <assert.h>
#include <raylib.h>
#include <stdbool.h>

Component *component_init(ComponentAudioCallback audio_callback,
                          ComponentMidiCallback midi_callback,
                          ComponentStateDestructor state_destructor, bool is_enterable, TextureKind texture_kind, void *state) {
    Component *component = malloc(sizeof(Component));
    assert(component != NULL);

    component->audio_callback = audio_callback;
    component->midi_callback = midi_callback;
    component->state_destructor = state_destructor;
    component->is_enterable = is_enterable;
    component->texture_kind = texture_kind;
    component->state = state;

    return component;
}

void component_next_audio(Component *component, float *buffer, unsigned int buffer_size) {
    if (component->audio_callback != NULL) {
        component->audio_callback(component->state, buffer, buffer_size);
    }
}

void component_next_midi(Component *component, const MidiMessage *messages, unsigned int count) {
    if (component->midi_callback != NULL) {
        component->midi_callback(component->state, messages, count);
    }
}

void component_render(Component *component, Vector2 position) {
    DrawTextureV(texture_load(component->texture_kind), position, WHITE);
}

void component_free(Component *component) {
    component->state_destructor(component->state);
    free(component);
}

bool component_is_enterable(Component *component) { return component->is_enterable; }
