#ifndef ENVELOPE_H
#define ENVELOPE_H

#include "component_system/component_methods.h"
#include "components/rack.h"

typedef struct {
    Rack rack;
    float attack;
    float decay;
    float sustain;
    float release;
    float amplitude;
} Envelope;

Component *envelope_init(Component *parent);
void envelope_free(Component *self);
void envelope_preview(Vector2 position, Vector2 size);

void envelope_audio_callback(Component *self, float *buffer, size_t size);
void envelope_midi_callback(Component *self, const MidiMessage *messages, size_t size, bool system);

/* void envelope_rack_render(Component *self, Vector2 position, Vector2 size); */
void envelope_render(Component *self, Vector2 position, Vector2 size);


extern ComponentMethods envelope_methods;

#endif
