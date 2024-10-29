#include "components/envelope.h"
#include "components/rack.h"
#include "component_system/component_system.h"
#include "constants/size.h"
#include "texture_handler.h"
#include "audio_engine.h"
#include <assert.h>
#include <stdlib.h>

Component *envelope_init(Component *parent) {
    Envelope *envelope = malloc(sizeof(Envelope));

    envelope->rack.component.is_container = true;
    envelope->rack.component.type = COMPONENT_ENVELOPE;
    envelope->rack.component.parent = parent;

    envelope->rack.components = calloc(RACK_SIZE * RACK_SIZE, sizeof(Component *));
    assert(envelope->rack.components != NULL);

    envelope->rack.cursor.x = 0;
    envelope->rack.cursor.y = 0;
    envelope->rack.parent = parent;

    pthread_mutex_init(&envelope->rack.lock, NULL);

    envelope->attack = 0.0f;
    envelope->decay = 0.0f;
    envelope->sustain = 0.0f;
    envelope->release = 0.0f;

    return (Component *)envelope;
}
void envelope_free(Component *self) {
    Envelope *envelope = (Envelope *)self;
    Component *component;
    for (int i = 0; i < RACK_SIZE * RACK_SIZE; ++i) {
        component = envelope->rack.components[i];
        if (component != NULL) {
            component_free(component);
        }
    }

    pthread_mutex_destroy(&envelope->rack.lock);
    free(envelope->rack.components);
    free(envelope);
}

void envelope_preview(Vector2 position, Vector2 size) {
    Texture2D texture = texture_load(TEXTURE_ENVELOPE);
    DrawTextureV(texture, position, WHITE);
}

void envelope_audio_callback(Component *self, float *buffer, size_t size) {
    Envelope *envelope = (Envelope *)self;

    float milliseconds = 1000.0 * (float)size / (float)(SAMPLE_RATE * 1000);

    for (size_t i = 0; i < size; ++i) {
        buffer[i] *= envelope->amplitude;
    }
}
void envelope_midi_callback(Component *self, const MidiMessage *messages, size_t size, bool system) {}

void envelope_render(Component *self, Vector2 position, Vector2 size) {
    return envelope_preview(position, size);
}

void envelope_settings_render(Component *self, Vector2 position, Vector2 size) {}

ComponentMethods envelope_methods = {
    .init = envelope_init,
    .free = envelope_free,
    .preview = envelope_preview,
    .audio_callback = rack_audio_callback,
    .midi_callback = rack_midi_callback,
    .settings_render = envelope_settings_render,
    .rack_render = rack_rack_render,
    .render = envelope_render,
    .mount = rack_mount,
    .unmount = rack_unmount,
    .current = rack_current,
    .move_cursor = rack_move_cursor,
};
